#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

void init_buffer(im_buffer_t *buffer, size_t size) {
  buffer->buffer = malloc(size);
  buffer->buffer_capacity = size;
  buffer->buffer_end = 0;
  buffer->buffer_start = 0;
}

void reset_buffer_start(im_buffer_t *buffer) {
  if (buffer->buffer_start == 0) return;
  memcpy(buffer->buffer, buffer->buffer + buffer->buffer_start,
         buffer->buffer_end - buffer->buffer_start);
  buffer->buffer_end -= buffer->buffer_start;
  buffer->buffer_start = 0;
}
