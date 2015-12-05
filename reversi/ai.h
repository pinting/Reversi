#ifndef JAM_AI_H
#define JAM_AI_H

#include <limits.h>
#include "board.h"

/// Fake infinite constant.
#define INF INT_MAX

/// AI struct which contains options for the calculations.
typedef struct AI
{
	/// Enable pass.
	Bool pass;
	/// Enable random.
	Bool random;
	/// Maximum search level.
	int level;
} AI;

/// Get the best move coords with a type.
/// @param ai AI struct.
/// @param board Board structure.
/// @param type Cell type.
/// @param x X coord.
/// @param y Y coord.
Bool ai_test(AI *ai, Board *board, Cell type, int *x, int *y);

/// Init an AI struct.
/// @param pass Enable pass.
/// @param random Enable random calculations.
/// @param level Maximum search level.
AI ai_init(Bool pass, Bool random, int level);

#endif
