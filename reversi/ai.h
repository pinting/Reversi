#ifndef AI_H
#define AI_H

#include "board.h"

#define INF 32767
#define sum(a, b) abs(a + b) > INF ? INF : a + b

void ai_init(Bool pass, Bool random, int level);
Bool ai_test(Board *board, Cell type, int *x, int *y);

#endif
