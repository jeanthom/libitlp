#include <stdint.h>
#include <string.h>

uint8_t read_uint8t_offset(char *pointer, int offset) {
  uint8_t buffer;
  memcpy(&buffer, pointer+offset, sizeof(uint8_t));
  return buffer;
}

uint16_t read_uint16t_offset(char *pointer, int offset) {
  uint16_t buffer;
  memcpy(&buffer, pointer+offset, sizeof(uint16_t));
  return buffer;
}

uint32_t read_uint32t_offset(char *pointer, int offset) {
  uint32_t buffer;
  memcpy(&buffer, pointer+offset, sizeof(uint32_t));
  return buffer;
}

uint64_t read_uint64t_offset(char *pointer, int offset) {
  uint64_t buffer;
  memcpy(&buffer, pointer+offset, sizeof(uint64_t));
  return buffer;
}
