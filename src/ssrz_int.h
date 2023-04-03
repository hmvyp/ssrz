/**
 * Stream encoding is little endian
 */

#ifndef ssrz_int_h
#define ssrz_int_h


#include <stdint.h>

#include "ssrz_pp.h"


// allow duck-typing for user-defined stream class:
#ifndef ssrzByteStream

typedef struct ssrzByteStream{
  unsigned char* data;
  size_t length;
}ssrzByteStream;

#endif

// Define expression that maps number of a byte in the byte stream
// to bit number in some integral type (counting from the least significant bit with number 0)
// The mapping depends on endianness.
// In the code below "nb" denotes sizeof(integral type)

#ifndef SSRZ_ENDIAN
# error "Please define SSRZ_ENDIAN macro: 0 for little endian,  1 for big endian"
#elif SSRZ_ENDIAN == 1
// big endian case:
# define SSRZ_BIT4BYTE(i) ((nb - 1 - (i)) << 3)
#elif SSRZ_ENDIAN == 0
// little endian case:
# define SSRZ_BIT4BYTE(i) ((i) <<  3)
#else
# error "SSRZ_ENDIAN shall be  0 (for little endian) or 1 (for big endian)"
#endif



// define reader for unsigned version of signed type typ (returns error code):
#define SSRZ_UREADER_M(typ) static inline int \
ssrzRead_u##typ (ssrzByteStream* bs, u##typ* pval){ \
  unsigned nb = sizeof(u##typ); \
  u##typ res = 0; \
  if(nb > bs->length){ \
    return -1; \
  }else{ \
    unsigned i; \
    for(i = 0; i < nb;  ++i){ \
      res = res | (((u##typ)bs->data[i]) << SSRZ_BIT4BYTE(i)); \
    } \
    bs->data += nb; \
    bs->length -= nb; \
    *pval = res; \
    return 0; \
  } \
}

// define writer for unsigned version of signed type typ (returns error code):
#define SSRZ_UWRITER_M(typ) static inline int \
ssrzWrite_u##typ (ssrzByteStream* bs, u##typ* pval){ \
  unsigned nb = sizeof(u##typ); \
  u##typ val = *pval; \
  if(nb > bs->length){ \
    return -1; \
  }else{ \
    unsigned i; \
    for(i = 0; i < nb;  ++i){ \
      bs->data[i] = (uint8_t)(val >> SSRZ_BIT4BYTE(i)); \
    } \
    bs->data += nb; \
    bs->length -= nb; \
    return 0; \
  } \
}

#define SSRZ_SREADER_M(typ)  \
  static inline int ssrzRead_##typ (ssrzByteStream* bs, typ* pval) {\
    return  ssrzRead_u##typ (bs, (u##typ*)pval); \
  }

#define SSRZ_SWRITER_M(typ)  \
  static inline int ssrzWrite_##typ (ssrzByteStream* bs, typ* pval) {\
    return ssrzWrite_u##typ (bs, (u##typ*)pval); \
  }

#define SET_OF_SIGNED_INTEGRALS(int_type) \
    deF(int_type, int8_t)  \
    deF(int_type, int16_t) \
    deF(int_type, int32_t) \
    deF(int_type, int64_t)

#define MK_INT_READERS_WRITERS \
    SET_OF_SIGNED_INTEGRALS(SSRZ_UREADER) \
    SET_OF_SIGNED_INTEGRALS(SSRZ_SREADER) \
    SET_OF_SIGNED_INTEGRALS(SSRZ_UWRITER) \
    SET_OF_SIGNED_INTEGRALS(SSRZ_SWRITER) \


MK_INT_READERS_WRITERS











#endif
