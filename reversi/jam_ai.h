#ifndef JAM_AI_H
#define JAM_AI_H

#include <limits.h>
#include "board.h"

#define INF INT_MAX

Bool ai_test(Board *board, Cell type, int *x, int *y);
void ai_init(Bool pass, Bool random, int level);

#endif
