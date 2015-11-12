#ifndef RUCZ_H
#define RUCZ_H

#include "board.h"
#include "../rucz/board.h"
#include "../rucz/alpha_beta.h"
#include "../rucz/reversi_ai.h"

#define SIZE 8

Bool rucz_test(Board *board, Cell type, int *x, int *y);

#endif