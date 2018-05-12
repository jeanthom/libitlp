#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../src/decryption.h"
#include "../../src/inflate.h"

int main(int argc, char *argv[]) {
  FILE *fh, *fo;
  char header[4];
  char *encrypted_buffer;
  char *decrypted_buffer = NULL;
  char *decompressed_buffer = NULL;
  uint32_t header_length, file_length;
  size_t max_crypt_size, crypt_size,
    decompressed_buffer_size = 0, decrypted_size;

  /* Checking command parameters */
  if (argc != 3) {
    printf("Usage: %s iTunes\\ Library.itl output.bin\n", argv[0]);
    return EXIT_SUCCESS;
  }

  /* Opening iTunes library file */
  fh = fopen(argv[1], "rb");
  if (!fh) {
    fprintf(stderr, "Can't open iTunes Library file.\n");
    return EXIT_FAILURE;
  }

  /* Opening output file */
  fo = fopen(argv[2], "wb");
  if (!fo) {
    fprintf(stderr, "Can't open output file");
    return EXIT_FAILURE;
  }

  /* Checking for iTunes header */
  fread(&header, sizeof(char), 4, fh);
  if (memcmp(&header, "hdfm", 4) != 0) {
    fprintf(stderr, "iTunes Library header is incorrect.\n");
    return EXIT_FAILURE;
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
    fprintf(stderr, "File length not matching.");
    return EXIT_FAILURE;
  }

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
    fprintf(stderr, "Decryption failed.\n");
    return EXIT_FAILURE;
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
    fprintf(stderr, "Decompression failed.\n");
    return EXIT_FAILURE;
  }

  fwrite(decompressed_buffer, 1, decompressed_buffer_size, fo);
  fclose(fo);

  /* Cleanup */
  free(decrypted_buffer);
  free(decompressed_buffer);
  
  return EXIT_SUCCESS;
}
