#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>

#include "decryption.h"
#include "memtools.h"
#include "inflate.h"
#include "itlp/types.h"
#include "blocks.h"

struct msdh** itlp_open_library(char *library_file) {
  FILE *fh;
  char header[4];
  char *encrypted_buffer;
  char *decrypted_buffer = NULL;
  char *decompressed_buffer = NULL;
  uint32_t header_length, file_length;
  size_t max_crypt_size, crypt_size,
    decompressed_buffer_size = 0, decrypted_size;
  size_t num_msdh;
  unsigned int i;
  struct msdh **msdh_array = NULL;

  /* Opening iTunes library file */
  fh = fopen(library_file, "rb");
  if (!fh) {
    fprintf(stderr, "iTunes parser: Can't open iTunes Library file.");
    return NULL;
  }

  /* Checking for iTunes header */
  fread(&header, sizeof(char), 4, fh);
  if (memcmp(&header, "hdfm", 4) != 0) {
    fprintf(stderr, "iTunes parser: iTunes Library header is incorrect.");
    return NULL;
  }

  /* Reading header length */
  fread(&header_length, sizeof(uint32_t), 1, fh);
  header_length = ntohl(header_length);

  /* Reading file length */
  fread(&file_length, sizeof(uint32_t), 1, fh);
  file_length = ntohl(file_length);
  
  /* Comparing actual file length and file length annouced by header */
  fseek(fh, 0, SEEK_END);
  if (ftell(fh) != file_length) {
    fprintf(stderr, "iTunes parser: File length not matching.");
    return NULL;
  }

  /* Reading msdh numbers */
  fseek(fh, 0x30, SEEK_SET);
  fread(&num_msdh, sizeof(uint32_t), 1, fh);
  num_msdh = ntohl(num_msdh);

  /* Reading max crypt size */
  fseek(fh, 92, SEEK_SET);
  fread(&max_crypt_size, sizeof(uint32_t), 1, fh);
  max_crypt_size = ntohl(max_crypt_size);

  /* On iTunes ≥ 10, the first 100k bytes are crypted */
  crypt_size = file_length-header_length;
  if (crypt_size > max_crypt_size) {
    crypt_size = max_crypt_size;
  }

  /* I don't know why, but without this it won't work */
  crypt_size -= crypt_size % 16;

  /* Reading encrypted part of the library */
  fseek(fh, header_length, SEEK_SET);
  encrypted_buffer = malloc(sizeof(char)*crypt_size);
  fread(encrypted_buffer, sizeof(char), crypt_size, fh);

  /* Decrypting library */
#ifdef _CRYPTO_OPENSSL
  decrypted_buffer = itlp_decrypt_openssl(encrypted_buffer,
					  crypt_size,
					  &decrypted_size);
#endif

#ifdef _CRYPTO_CC
  decrypted_buffer = itlp_decrypt_commoncrypto(encrypted_buffer,
					       crypt_size,
					       &decrypted_size);
#endif
  
  if (!decrypted_buffer) {
    fprintf(stderr, "iTunes parser: Decryption failed. ");
    fprintf(stderr, "Aborting iTunes Library parsing.\n");
    return NULL;
  }

  /* Appending remaining unencrypted data */
  if ((file_length - header_length) > max_crypt_size) {
    decrypted_buffer = realloc(decrypted_buffer, file_length-header_length);
    crypt_size = file_length-header_length;
  }
 
  /* Decompression decrypted library */
  decompressed_buffer = itlp_inflate(decrypted_buffer, crypt_size,
				     &decompressed_buffer_size);
  if (!decompressed_buffer) {
    fprintf(stderr, "iTunes parser: Decompression failed. Aborting iTunes Library parsing.\n");
    return NULL;
  }

  char *pos_buffer;
  pos_buffer = decompressed_buffer;

  /* Creating a NULL-ended array with all the msdh blocks */
  msdh_array = malloc((num_msdh+1) * sizeof(struct msdh*));
  for (i = 0; i < num_msdh; i++) {
    msdh_array[i] = itlp_parse_msdh(&pos_buffer);

    if (!msdh_array[i]) {
      return NULL;
    }
  }
  msdh_array[num_msdh] = NULL;

  /* Cleanup */
  free(decrypted_buffer);
  free(decompressed_buffer);
  
  return msdh_array;
}

