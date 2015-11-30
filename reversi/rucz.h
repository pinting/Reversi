#ifndef RUCZ_H
#define RUCZ_H

#include "board.h"
#include "../rucz/board.h"
#include "../rucz/alpha_beta.h"
#include "../rucz/reversi_ai.h"

typedef struct Rucz
{
	alpha_beta_stats_t abst;
	alpha_beta_data_t abd;
} Rucz;

Bool rucz_test(Rucz *rucz, Board *board, Cell type, int *x, int *y);
void rucz_free(Rucz *rucz);
Rucz rucz_init(int level);

#endif