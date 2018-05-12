/**
 * @file memtools.h
 */

/**
 * Extracts an uint8_t from a char array at a specific offset.
 *
 * @fn uint8_t read_uint8t_offset(char *pointer, int offset)
 * @param pointer Pointer to the string
 * @param offset Reading offset
 * @internal
 */
uint8_t read_uint8t_offset(char *pointer, int offset);

/**
 * Extracts an uint16_t from a char array at a specific offset.
 *
 * @fn uint16_t read_uint16t_offset(char *pointer, int offset)
 * @param pointer Pointer to the string
 * @param offset Reading offset
 * @internal
 */
uint16_t read_uint16t_offset(char *pointer, int offset);

/**
 * Extracts an uint32_t from a char array at a specific offset.
 *
 * @fn uint32_t read_uint32t_offset(char *pointer, int offset)
 * @param pointer Pointer to the string
 * @param offset Reading offset
 * @internal
 */
uint32_t read_uint32t_offset(char *pointer, int offset);

/**
 * Extracts an uint64_t from a char array at a specific offset
 *
 * @fn uint64_t read_uint64t_offset(char *pointer, int offset)
 * @param pointer Pointer to the string
 * @param offset Reading offset
 */
uint64_t read_uint64t_offset(char *pointer, int offset);
