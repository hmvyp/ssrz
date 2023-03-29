#ifndef ssrz_struct_h
#define ssrz_struct_h

#include "ssrz_int.h"

/*
#define SSRZ_EMPTY1_M(p1)
#define SSRZ_EMPTY2_M(p1,p2)


#define SSRZ_ID_M(p1) p1
#define SSRZ_ID2_1_M(p1, p2) p1
#define SSRZ_ID2_2_M(p1, p2) p2
*/

#define SSRZ_STRUCT_FIELD_M(typ, fieldname) typ fieldname;

#define SSRZ_STRUCT_MK_TYPE(structdef) \
    typedef struct structdef##_M(SSRZ_ID, SSRZ_EMPTY2) { \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD) \
    } structdef##_M(SSRZ_ID, SSRZ_EMPTY2) ;

// for both Read and Write action for a given field:
#define  SSRZ_STRUCT_PROCESS_FIELD(action, typ, nam) { \
    int res = ssrz##action##_##typ(bs, &pval->nam); \
    if(res != 0){ \
      return res; \
    }\
}

#define  SSRZ_STRUCT_FIELD_READ_M(typ, nam) SSRZ_STRUCT_PROCESS_FIELD(Read, typ, nam)

#define  SSRZ_STRUCT_FIELD_WRITE_M(typ, nam) SSRZ_STRUCT_PROCESS_FIELD(Write, typ, nam)

#define SSRZ_STRUCT_FUNC_HEADER(action, struct_name)   static inline int \
  ssrz##action##_##struct_name (ssrzByteStream* bs, struct_name* pval) {

#define SSRZ_STRUCT_READ_FUNC_HEADER_M(struct_name) SSRZ_STRUCT_FUNC_HEADER(Read, struct_name)
#define SSRZ_STRUCT_WRITE_FUNC_HEADER_M(struct_name) SSRZ_STRUCT_FUNC_HEADER(Write, struct_name)

#define SSRZ_STRUCT_MK_READER(structdef) \
    structdef##_M(SSRZ_STRUCT_READ_FUNC_HEADER, SSRZ_EMPTY2) \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD_READ) \
    return 0; \
}

#define SSRZ_STRUCT_MK_WRITER(structdef) \
    structdef##_M(SSRZ_STRUCT_WRITE_FUNC_HEADER, SSRZ_EMPTY2) \
    structdef##_M(SSRZ_EMPTY1, SSRZ_STRUCT_FIELD_WRITE) \
    return 0; \
}

#define SSRZ_DEFINE_STRUCT(structdef) \
SSRZ_STRUCT_MK_TYPE(structdef)        \
SSRZ_STRUCT_MK_READER(structdef)      \
SSRZ_STRUCT_MK_WRITER(structdef)


// example

#define MY_STRUCT_M(struct_name, struct_field) \
    deF(struct_name, my_struct_t)  \
    deF2(struct_field, int8_t, a)  \
    deF2(struct_field, int64_t, b) \

SSRZ_DEFINE_STRUCT(MY_STRUCT)

#endif
