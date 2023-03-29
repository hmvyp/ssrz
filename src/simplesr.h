#ifndef simplesr.h
#define simplesr.h



#include <stdint.h>
#include "simplepp.h"


// allow duck-typing for user-defined stream class:
#ifndef ssrzByteStream

typedef struct ssrzByteStream{
  unsigned char* data;
  size_t length;
}ssrzByteStream;

#endif

// define raeder for unsigned version of signed type typ:
#define SSRZ_UREADER_M(typ) static inline u##typ \
ssrzRead_u##typ##_M (ssrzByteStream* bs, int* perr){ \
  unsigned nb = sizeof(u##typ); \
  u##typ res = 0; \
  if(nb > bs->length){ \
    *perr |= 1; \
  }else{ \
    unsigned i; \
    for(i = 0; i < nb;  ++i){ \
      res = (res << 8) | bs->data[i]; /* << 8 is safe even for uint8_t due to integer promotion */ \
       /* (we don't support 8-bits platform) */ \
    } \
    bs->data += nb; \
    bs->length -= nb; \
  } \
  return res; \
}

#define SSRZ_SREADER_M(typ)  \
  static inline typ ssrzRead_##typ##_M (ssrzByteStream* bs, int* perr) {\
    return  (typ) ssrzRead_u##typ##_M (bs, perr); \
  }


#define deF(m, item) m##_M(item)
#define deF_2(m, item1, item2) m##_M(item1, item2)

#define SET_OF_SIGNED_INTEGRALS(int_type) \
    deF(int_type, int8_t)  \
    deF(int_type, int16_t) \
    deF(int_type, int32_t) \
    deF(int_type, int64_t)

#define MK_READERS \
    SET_OF_SIGNED_INTEGRALS(SSRZ_UREADER) \
    SET_OF_SIGNED_INTEGRALS(SSRZ_SREADER)

MK_READERS











#endif
