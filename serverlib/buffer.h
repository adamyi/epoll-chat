#ifndef IM_BUFFER_H__
#define IM_BUFFER_H__

#include <stdlib.h>
#include <stdint.h>

#define OUT_BUFFER_DEFAULT_SIZE BUFSIZ

typedef struct im_buffer {
  uint8_t *buffer;
  size_t buffer_capacity;
  size_t buffer_start;
  size_t buffer_end;
} im_buffer_t;

void init_buffer(im_buffer_t *buffer, size_t size);

void reset_buffer_start(im_buffer_t *buffer);

#endif
