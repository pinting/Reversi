#ifndef RUCZ_H
#define RUCZ_H

#include "board.h"
#include "../rucz/board.h"
#include "../rucz/alpha_beta.h"
#include "../rucz/reversi_ai.h"

/// The Rucz struct which describes how the AI should act.
typedef struct Rucz
{
	/// Stats.
	alpha_beta_stats_t abst;
	/// Configurations.
	alpha_beta_data_t abd;
} Rucz;

/// Search for the best move.
/// @param rucz Rucz struct.
/// @param board Board struct.
/// @param type Cell type.
/// @param x Result X coord.
/// @param y Result Y coord.
Bool rucz_test(Rucz *rucz, Board *board, Cell type, int *x, int *y);

/// Free it up.
/// @param rucz Rucz struct.
void rucz_free(Rucz *rucz);

/// Init the rucz AI.
/// @param level Maximum search level.
Rucz rucz_init(int level);

#endif