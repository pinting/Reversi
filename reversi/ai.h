#ifndef AI_H
#define AI_H

#include <limits.h>
#include "board.h"

Bool ai_test(Board *board, Cell type, int *x, int *y);
void ai_init(Bool pass, Bool random, int level);

#endif
