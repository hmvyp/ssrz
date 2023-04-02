#ifndef ssrz_pp_h
#define ssrz_pp_h

// applies one-parameter macro  m##_M to item
#define deF(m, item) m##_M(item)

// applies two-parameters macro  m##_M to 2 items
#define deF2(m, item1, item2) m##_M(item1, item2)

// etc. :
#define deF3(m, item1, item2, item3) m##_M(item1, item2, item3)
#define deF4(m, item1, item2, item3, item4) m##_M(item1, item2, item3, item4)
#define deF5(m, item1, item2, item3, item4, item5) m##_M(item1, item2, item3, item4, item5)


// helper macros to do nothing in deF macro:
#define SSRZ_EMPTY1_M(p1)
#define SSRZ_EMPTY2_M(p1,p2)
#define SSRZ_EMPTY3_M(p1,p2,p3)
#define SSRZ_EMPTY4_M(p1,p2,p3,p4)
#define SSRZ_EMPTY5_M(p1,p2,p3,p4,p5)


#endif
