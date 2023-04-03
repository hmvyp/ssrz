#ifndef ssrz_pp_h
#define ssrz_pp_h

// applies one-parameter macro  m##_M to item p
#define deF(m, p) m##_M(p)

// applies two-parameters macro  m##_M to 2 items p1, p2
#define deF2(m, p1, p2) m##_M(p1, p2)

// etc. :
#define deF3(m, p1, p2, p3) m##_M(p1, p2, p3)
#define deF4(m, p1, p2, p3, p4) m##_M(p1, p2, p3, p4)
#define deF5(m, p1, p2, p3, p4, p5) m##_M(p1, p2, p3, p4, p5)


// helper macros to do nothing in deF macro:
#define SSRZ_EMPTY1_M(p1)
#define SSRZ_EMPTY2_M(p1,p2)
#define SSRZ_EMPTY3_M(p1,p2,p3)
#define SSRZ_EMPTY4_M(p1,p2,p3,p4)
#define SSRZ_EMPTY5_M(p1,p2,p3,p4,p5)


#endif
