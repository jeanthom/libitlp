/**
 * @file decryption.h
 * @brief This file holds all the decryption functions
 */

/**
 * @fn char* itlp_decrypt_openssl(const char *input, int input_len, size_t *decrypted_size)
 * @brief AES128-ECB decryption function, based on OpenSSL
 * @param input Crypted data input
 * @param inputSize Size of crypted data input
 * @param decryptedSize Pointer to size_t, 
 * @return Decrypted input (or NULL if errors happened while decrypting)
 * @internal
 */
#ifdef _CRYPTO_OPENSSL
char* itlp_decrypt_openssl(const char *input, int input_len, size_t *decrypted_size);
#endif

/**
 * @fn char* itlp_decrypt_commoncrypto(const char *input, size_t inputSize, size_t *decryptedSize)
 * @brief AES128-ECB decryption function, based on CommonCrypto
 * @param input Crypted data input
 * @param inputSize Size of crypted data input
 * @param decryptedSize Pointer to size_t, 
 * @return Decrypted input (or NULL if errors happened while decrypting)
 * @internal
 *
 * This function uses macOS's CommonCrypto cryptography
 * library to decrypt AES128-ECB crypted data. It is inspired by
 * https://github.com/AlanQuatermain/aqtoolkit.
 */

#ifdef _CRYPTO_CC
char* itlp_decrypt_commoncrypto(const char *input, size_t inputSize, size_t *decryptedSize);
#endif
