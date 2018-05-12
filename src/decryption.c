#include <stdlib.h>
#include <stdbool.h>

#ifdef _CRYPTO_OPENSSL
#include <openssl/evp.h>
#include <openssl/ssl.h>
#endif

#ifdef _CRYPTO_CC
#include <CommonCrypto/CommonCryptor.h>
#endif

#include "decryption.h"

#ifdef _CRYPTO_OPENSSL
char* itlp_decrypt_openssl(const char *input, int input_len, size_t *decrypted_size) {
  EVP_CIPHER_CTX *ctx;
  int outlen = 0, ret;
  char *output = NULL;
  unsigned char *key = (unsigned char*)DECRYPT_KEY;

  *decrypted_size = input_len;

  /* Allocatng output buffer */
  output = malloc(sizeof(char)*input_len);
  if (output == NULL) {
    fprintf(stderr, "iTunes parser: Error while allocating memory (in decryption)");
    return NULL;
  }
  
  /* OpenSSL init */
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);
  EVP_CIPHER_CTX_set_padding(ctx, false);

  /* Decrypting the input */
  ret = EVP_DecryptUpdate(ctx,
			  (unsigned char*)output,
			  &outlen,
			  (unsigned char*)input,
			  input_len);
  if (ret != 1)
    return NULL;
  
  ret = EVP_DecryptFinal_ex(ctx,
			    (unsigned char*)(output+outlen),
			    &outlen);
  if (ret != 1)
    return NULL;
  
  /* Cleanup */
  EVP_CIPHER_CTX_free(ctx);
  return output;
}
#endif

#ifdef _CRYPTO_CC
char* itlp_decrypt_commoncrypto(const char *input, size_t inputSize, size_t *decryptedSize) {
  CCCryptorRef cryptorRef = NULL;
  void *buffer;
  size_t bufferSize, bufferUsed = 0;
  const char *key = DECRYPT_KEY;

  /* CCCryptor initialisation */
  if (CCCryptorCreateWithMode(kCCDecrypt,
			      kCCModeECB,
			      kCCAlgorithmAES128,
			      ccNoPadding,
			      NULL,
			      (const void*)key,
			      kCCKeySizeAES128,
			      NULL,
			      0,
			      0,
			      0,
			      &cryptorRef) != kCCSuccess) {
    return NULL;
  }

  /* Allocating buffer */
  bufferSize = CCCryptorGetOutputLength(cryptorRef,
					inputSize,
					true);
  *decryptedSize = bufferSize;
  buffer = malloc(bufferSize);

  if (CCCryptorUpdate(cryptorRef,
		      (const void*)input,
		      inputSize,
		      buffer,
		      bufferSize,
		      &bufferUsed) != kCCSuccess) {
    return NULL;
  }
  
  if (CCCryptorFinal(cryptorRef,
		     (char*)buffer+bufferUsed,
		     bufferSize-bufferUsed,
		     &bufferUsed) != kCCSuccess) {
    return NULL;
  }

  /* Clean-up */
  CCCryptorRelease(cryptorRef);

  return (char*)buffer;
}
#endif
