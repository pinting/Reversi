#include "ai.h"

/// Calculate the best next move of a type.
/// @param ai AI struct.
/// @param board Board structure.
/// @param type Cell type.
/// @param init Means the start value, which should be 0.
/// @param level Means the maximum search level.
/// @param alpha Means the deafult alpha size (-INF).
/// @param beta Means the default beta size (+INF).
/// @param res_x Result X coord.
/// @param res_y Result Y coord.
static int ai_calc(AI *ai, Board *board, Cell type, int init, int level, int alpha, int beta, int *res_x, int *res_y)
{
	int value, x, y, count, count_rev, best_x, best_y;
	Board next;
	Bool cut;

	// Return the init value if the max level is reached
	if (level == 0)
	{
		return init;
	}

	debug(3, "Step in level %d (searching %d)\n", level, type);

	best_x = -1;
	best_y = -1;
	cut = FALSE;

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			value = init + (int)type * board_move(board, x, y, type, TRUE);

			if (value == 0)
			{
				continue;
			}
			
			debug(3, "Valid move found: %d\n", value);

			// Make the move on a new board
			next = board_new(board->size);
			board_copy(&next, board);
			board_move(&next, x, y, type, FALSE);

			// Check if there are any moves left for the enemy
			if((ai->pass && !board_count(&next, BLANK)) || (!ai->pass && !board_moves_left(&next, minus(type))))
			{
				count = board_count(&next, type);
				count_rev = board_count(&next, minus(type));

				if (count > count_rev)
				{
					debug(3, "Board is full: win\n");
					value = type * INF / 2;
				}
				else if (count == count_rev)
				{
					debug(3, "Board is full: tie\n");
					value = minus(type) * INF / 3;
				}
				else
				{
					debug(3, "Board is full: loss\n");
					value = minus(type) * INF / 2;
				}
			}
			else
			{
				value = ai_calc(ai, &next, minus(type), value, level - 1, alpha, beta, NULL, NULL);
			}

			board_free(&next);

			// Max
			if (type == MAX && value > alpha || (ai->random && rand() % 2 && value == alpha))
			{
				debug(3, "New alpha: %d\n", value);
				alpha = value;
				best_x = x;
				best_y = y;
			}

			// Min
			else if (type == MIN && value < beta || (ai->random && rand() % 2 && value == beta))
			{
				debug(3, "New beta: %d\n", value);
				beta = value;
				best_x = x;
				best_y = y;
			}

			// Cut
			if (alpha >= beta)
			{
				debug(3, "New alpha-beta cut (%d >= %d)\n", alpha, beta);
				cut = 1;
				break;
			}
		}

		if (cut)
		{
			break;
		}
	}

	if (best_x >= 0 && best_y >= 0 && res_x != NULL && res_y != NULL)
	{
		*res_x = best_x;
		*res_y = best_y;
	}

	debug(3, "Step out from layer %d with %d\n", level, type == MAX ? alpha : beta);

	return type == MAX ? alpha : beta;
}

Bool ai_test(AI *ai, Board *board, Cell type, int *x, int *y)
{
	*x = -1;
	*y = -1;

	debug(3, "Calculating AI move in %d layers...\n", ai->level);
	ai_calc(ai, board, type, 0, ai->level, minus(INF), INF, x, y);
	debug(3, "End of search (%d, %d)\n", *x, *y);

	if (*x >= 0 && *y >= 0)
	{
		return TRUE;
	}

	return FALSE;
}

AI ai_init(Bool pass, Bool random, int level)
{
	AI ai;

	ai.pass = pass;
	ai.random = random;
	ai.level = level;

	return ai;
}