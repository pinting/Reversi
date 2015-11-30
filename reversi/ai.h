#ifndef JAM_AI_H
#define JAM_AI_H

#include <limits.h>
#include "board.h"

#define INF INT_MAX

typedef struct AI
{
	Bool pass;
	Bool random;
	int level;
} AI;

Bool ai_test(AI *ai, Board *board, Cell type, int *x, int *y);
AI ai_init(Bool pass, Bool random, int level);

#endif
