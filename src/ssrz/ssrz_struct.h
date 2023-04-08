#ifndef ssrz_struct_h
#define ssrz_struct_h

#include "ssrz_int.h"


#define SSRZ_ID_M(p1) p1
/*
#define SSRZ_ID2_1_M(p1, p2) p1
#define SSRZ_ID2_2_M(p1, p2) p2
*/

#define SSRZ_STRUCT_FIELD_M(typ, fieldname) typ fieldname;
#define SSRZ_STRUCT_ARRAY_FIELD_M(typ, fieldname, card) typ fieldname[card];

#define SSRZ_STRUCT_MK_TYPE(structdef) \
    typedef struct structdef##_M(SSRZ_ID, SSRZ_EMPTY2, SSRZ_EMPTY3) { \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD, SSRZ_STRUCT_ARRAY_FIELD) \
} structdef##_M(SSRZ_ID, SSRZ_EMPTY2, SSRZ_EMPTY3) ;

// for both Read and Write action for a given field:
#define  SSRZ_STRUCT_PROCESS_FIELD(action, typ, nam) { \
    int res = ssrz##action##_##typ(bs, &pval->nam); \
    if(res != 0){ \
      return res; \
    }\
}

// for both Read and Write action for a given array field:
#define  SSRZ_STRUCT_PROCESS_ARRAY_FIELD(action, typ, nam, card) { \
  int i; \
  for(i = 0; i < card; ++i){ \
    int res = ssrz##action##_##typ(bs, &pval->nam[i]); \
    if(res != 0){ \
      return res; \
    }\
  }\
}

#define  SSRZ_STRUCT_FIELD_READ_M(typ, nam) SSRZ_STRUCT_PROCESS_FIELD(Read, typ, nam)
#define  SSRZ_STRUCT_FIELD_WRITE_M(typ, nam) SSRZ_STRUCT_PROCESS_FIELD(Write, typ, nam)

#define  SSRZ_STRUCT_ARRAY_FIELD_READ_M(typ, nam, card) SSRZ_STRUCT_PROCESS_ARRAY_FIELD(Read, typ, nam, card)
#define  SSRZ_STRUCT_ARRAY_FIELD_WRITE_M(typ, nam, card) SSRZ_STRUCT_PROCESS_ARRAY_FIELD(Write, typ, nam, card)

#define SSRZ_STRUCT_FUNC_HEADER(action, struct_name)   static inline int \
  ssrz##action##_##struct_name (ssrzByteStream* bs, struct_name* pval) {

#define SSRZ_STRUCT_READ_FUNC_HEADER_M(struct_name) SSRZ_STRUCT_FUNC_HEADER(Read, struct_name)
#define SSRZ_STRUCT_WRITE_FUNC_HEADER_M(struct_name) SSRZ_STRUCT_FUNC_HEADER(Write, struct_name)

#define SSRZ_STRUCT_MK_READER(structdef) \
    structdef##_M(SSRZ_STRUCT_READ_FUNC_HEADER, SSRZ_EMPTY2, SSRZ_EMPTY3) \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD_READ, SSRZ_STRUCT_ARRAY_FIELD_READ) \
    return 0; \
}

#define SSRZ_STRUCT_MK_WRITER(structdef) \
    structdef##_M(SSRZ_STRUCT_WRITE_FUNC_HEADER, SSRZ_EMPTY2, SSRZ_EMPTY3) \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD_WRITE, SSRZ_STRUCT_ARRAY_FIELD_WRITE) \
    return 0; \
}

#define SSRZ_STRUCT_WIRESIZE_ENUM_NAME_M(structname) ssrz_wire_length_##structname = 0
#define SSRZ_STRUCT_STRUCT_FIELD_WIRELENGTH_M(typ, nam) + ssrz_wire_length_##typ
#define SSRZ_STRUCT_STRUCT_AFIELD_WIRELENGTH_M(typ, nam, card) + (ssrz_wire_length_##typ * card)

#define SSRZ_STRUCT_MK_WIRELENGTH(structdef) \
  enum { \
    structdef##_M(SSRZ_STRUCT_WIRESIZE_ENUM_NAME, SSRZ_EMPTY2, SSRZ_EMPTY3) \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_STRUCT_FIELD_WIRELENGTH, SSRZ_STRUCT_STRUCT_AFIELD_WIRELENGTH)\
  };


#define SSRZ_DEFINE_STRUCT(structdef) \
SSRZ_STRUCT_MK_TYPE(structdef)        \
SSRZ_STRUCT_MK_READER(structdef)      \
SSRZ_STRUCT_MK_WRITER(structdef)      \
SSRZ_STRUCT_MK_WIRELENGTH(structdef)


// example
#ifdef SSRZ_TEST

#include <string.h>

#define MY_INNER_STRUCT_M(struct_name, field, array_field) \
  deF(struct_name, my_inner_struct_t)  \
    deF3(array_field, int8_t, ar, 4)  \
    deF2(field, int8_t, a)  \
    deF2(field, int16_t, b) \
    deF2(field, int32_t, c) \
    deF2(field, int64_t, d) \

#define MY_STRUCT_M(struct_name, field, array_field) \
  deF(struct_name, my_struct_t)  \
    deF2(field, int8_t, a)  \
    deF2(field, my_inner_struct_t, istruct) \
    deF2(field, int16_t, b) \



SSRZ_DEFINE_STRUCT(MY_INNER_STRUCT)
SSRZ_DEFINE_STRUCT(MY_STRUCT)

#define SSRZ_TEST_BUFSIZE 200

static inline int
ssrzTestStruct(){
  uint8_t b1[SSRZ_TEST_BUFSIZE] = {0};
  uint8_t b2[SSRZ_TEST_BUFSIZE] = {0};
  my_struct_t src = {1 ,{{50,51,52,53}, -1, 300, 10000000, (((uint64_t)0x7A) << 56) | 1000  }, -1000};
  my_struct_t dst = {0};

  ssrzByteStream bs1 = {b1, sizeof(b1)};

  int res_w = ssrzWrite_my_struct_t(&bs1, &src); // serialize test struct into buffer b1
  {
    size_t data_sz = sizeof(b1) - bs1.length;
    ssrzByteStream bs2 = {b1, data_sz};
    ssrzByteStream bs3 = {b2, data_sz};
    int res_r = ssrzRead_my_struct_t(&bs2, &dst); // deserialize from buffer to dst struct
    int res_w2 = ssrzWrite_my_struct_t(&bs3, &dst); // serialize result (dst) to another buffer b2

    int res_cmp = memcmp(b1,b2, SSRZ_TEST_BUFSIZE); // compare buffers

    int wiresize =  SSRZ_WIRE_LENGTH(my_struct_t);

    if(res_w == 0 && res_r == 0 && res_w2 == 0 && res_cmp == 0 && wiresize == SSRZ_TEST_BUFSIZE - bs1.length){
      return 0; //Ok
    }
  }
  return -1;
}

#endif //SSRZ_TEST


#endif


