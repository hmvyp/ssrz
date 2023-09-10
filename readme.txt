Simple serialization library allows to generate automatiically serializer/deserializer functions for C and C++ structures. It also contains serializer/deserializer functions for fixed size integral types (as uint16_t, int32_t, etc.). User can also choose an appropriate endian for serialized integrals by defining macro SSRZ_ENDIAN.

Serializer/deserializer functions for integrals are already contained in the library and are ready to use, for example:


int ssrzWrite_int64_t (ssrzByteStream* bs, int64_t* pval) // serializer

int ssrzRead_uint32_t (ssrzByteStream* bs, uint32_t* pval); // deserializer

The functions return zero on success or non-zero on error (typically buffer overflow).

Serialization of user-defined structures we'll explain by example.

// first structure definition:

#define MY_INNER_STRUCT_M(struct_name, field, array_field) \
  deF(struct_name, my_inner_struct_t)  \
    deF3(array_field, int8_t, ar, 4)  \
    deF2(field, int8_t, a)  \
    deF2(field, int16_t, b) \
    deF2(field, int32_t, c) \
    deF2(field, int64_t, d) \

// Second structure definition:

#define MY_STRUCT_M(struct_name, field, array_field) \
  deF(struct_name, my_struct_t)  \
    deF2(field, int8_t, a)  \
    deF2(field, my_inner_struct_t, istruct) \
    deF2(field, int16_t, b) \

In this example the first struct is nested as a field in the second one.

The following macro calls generate "typdef struct" definitions and serialization functions for both structure types described above:

SSRZ_DEFINE_STRUCT(MY_INNER_STRUCT)
SSRZ_DEFINE_STRUCT(MY_STRUCT)

INPORTANT NOTE:

Structure definition macro name is a name of your choice, but it shall always end with "_M" suffix  (e.g. MY_INNER_STRUCT_M). However, when passing the name to generator macro SSRZ_DEFINE_STRUCT pass the name without(!) "_M" suffix.


What exactly will be generated?

The generated definition of my_struct_t will look as follows:

    typedef struct my_struct_t
    {
       int8_t a;
       my_inner_struct_t istruct;
       int16_t, b
    } my_struct_t;

Generated serializer for struct my_struct_t will have the following prototype:

    int ssrzWrite_my_struct_t(ssrzByteStream* bs, my_struct_t* pval);

For C++ the appropriate template specialization will be also generated:

    template<> 
    inline int 
    ssrzWrite<my_struct_t> (ssrzByteStream* bs, my_struct_t* pval);


See working example in the test code at the end of ssrz_struct.h file.

In this example suffix ..._WITH_WIRE_LENGTH in generator macro cause to define enums

     ssrz_wire_length_my_inner_struct_t
     ssrz_wire_length_my_struct_t
 
Do not use ..._WITH_WIRE_LENGTH variant of generator macro if you do not need to define wire length statically as compile time constant.
 
 You can also write your own serializers by hand (using naming convention described above) even for data types with variable wire length (e.g. C++ string or vector). Such types can be used as fields in other serializable structures (of course, except of those generated with ..._WITH_WIRE_LENGTH generator since their wire length can not be calculated at compile time).
 
 
 
  
