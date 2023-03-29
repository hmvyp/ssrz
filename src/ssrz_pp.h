#ifndef ssrz_pp_h
#define ssrz_pp_h

// applies one-parameter macro  m##_M to item
#define deF(m, item) m##_M(item)

// applies two-parameters macro  m##_M to items
#define deF2(m, item1, item2) m##_M(item1, item2)


// helper macros to do nothing:
#define SSRZ_EMPTY1_M(p1)
#define SSRZ_EMPTY2_M(p1,p2)

// helper macros to just extract macro argument:
#define SSRZ_ID_M(p1) p1
#define SSRZ_ID2_1_M(p1, p2) p1
#define SSRZ_ID2_2_M(p1, p2) p2



#endif
