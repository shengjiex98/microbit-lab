#include "blessed/simple_ringbuf.h"
#include <stdio.h>
#include <string.h>

void ringBuf_init(ringBuf* rb, char* buf, uint32_t size){
    rb->buf = buf;
    rb->size = size;
    rb->posWr = 0;
    rb->posRd = 0;
    rb->nBytes =0 ;    
}

uint32_t ringBuf_write(ringBuf *rb, char* buf,uint32_t nBytes){
  if(nBytes > rb->size - rb->nBytes){
    //insufficient space => drop
    return 0;
  }

  uint32_t nBytesNoWrap = rb->size - rb->posWr - 1;
  if(nBytes < nBytesNoWrap){
    nBytesNoWrap = nBytes;
  }
  uint32_t nBytesWrap = nBytes - nBytesNoWrap;

  memcpy(rb->buf + rb->posWr, buf, nBytesNoWrap);               //no wrap
  if(nBytesWrap > 0){
  memcpy(rb->buf, buf + nBytesNoWrap, nBytesWrap);               //wrap
  }

  //update pointer
  if(nBytesWrap == 0){
    rb->posWr += nBytesNoWrap;
  }else{
    rb->posWr = nBytesWrap;
  }
  rb->nBytes += nBytes;
  return nBytes;
}

uint32_t ringBuf_read(ringBuf* rb, char* buf, uint32_t nBytes){
if(nBytes > rb->nBytes){
    //insufficient number of bytes in Buffer => drop
    return 0;
  }

  uint32_t nBytesNoWrap = rb->size - rb->posRd - 1;
  if(nBytes < nBytesNoWrap){
    nBytesNoWrap = nBytes;
  }
  uint32_t nBytesWrap = nBytes - nBytesNoWrap;
  memcpy(buf,rb->buf + rb->posRd, nBytesNoWrap);               //no wrap
  if(nBytesWrap > 0){
  memcpy(buf + nBytesNoWrap,rb->buf, nBytesWrap);               //wrap
  }

  //update pointer
  if(nBytesWrap == 0){
    rb->posRd += nBytesNoWrap;
  }else{
    rb->posRd = nBytesWrap;
  }

  rb->nBytes -= nBytes;
  return nBytes;
}


