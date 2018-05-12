#include <string.h>
#include <stdlib.h>
#include <zlib.h>

char* itlp_inflate(char *input_buffer, size_t input_buffer_len,
		   size_t *output_len) {
  z_stream stream;
  unsigned int chunk_size;
  Bytef *chunk_buffer;
  char *output;

  chunk_size = 8192;
  *output_len = 0;

  chunk_buffer = malloc(chunk_size);
  output = malloc(0);
  
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  stream.avail_in = input_buffer_len;
  stream.next_in = (Bytef *)input_buffer;

  inflateInit(&stream);
  
  do {
    stream.avail_out = chunk_size;
    stream.next_out = chunk_buffer;
    inflate(&stream, Z_NO_FLUSH);

    if (stream.avail_out != chunk_size) {
      output = realloc(output, *output_len+chunk_size-stream.avail_out);
      memcpy(output+*output_len, chunk_buffer, chunk_size-stream.avail_out);
      *output_len += chunk_size-stream.avail_out;
    }
  } while (stream.avail_out == 0);

  inflateEnd(&stream);

  /* Cleanup */
  free(chunk_buffer);
  
  return output;
}
