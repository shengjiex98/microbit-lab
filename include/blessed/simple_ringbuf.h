#ifndef SIMPLE_RINGBUF_H_
#define SIMPLE_RINGBUF_H_
#include <inttypes.h>

typedef struct _ringBuf{ 
 uint32_t posWr;
 uint32_t posRd;
 uint32_t nBytes;
 char* buf;
 uint32_t size;
} ringBuf;

void ringBuf_init(ringBuf* rb, char* buf, uint32_t size);
uint32_t ringBuf_write(ringBuf *rb, char* buf,uint32_t nBytes);
uint32_t ringBuf_read(ringBuf* rb, char* buf, uint32_t nBytes);
#endif
