/**
 * @file inflate.h
 */

/**
 * @fn char* itlp_inflate(char *input_buffer, size_t input_buffer_len, size_t *output_buffer_len)
 * @param input_buffer Compressed input buffer
 * @param input_buffer_len Length of the input buffer
 * @param output_buffer_len Pointer to the length of the output buffer. The pointer must be provided by the caller function.
 * @return Decompressed output buffer
 * @internal
 */
char* itlp_inflate(char *input_buffer, size_t input_buffer_len, size_t *output_buffer_len);
