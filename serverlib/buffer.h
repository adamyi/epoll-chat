#ifndef IM_BUFFER_H__
#define IM_BUFFER_H__

#define OUT_BUFFER_DEFAULT_SIZE BUFSIZ

typedef struct im_buffer {
  uint8_t *buffer;
  size_t buffer_capacity;
  size_t buffer_start;
  size_t buffer_end;
} im_buffer_t;

#endif
