#include "ai.h"

static Bool pass;
static Bool random;
static int level;

// Calculate the best next move of a type.
// @init means the start value, which should be 0.
// @level means the maximum search level.
// @alpha means the deafult alpha size (-INF).
// @beta means the default beta size (+INF).
static int ai_calc(Board *board, Cell type, int init, int level, int alpha, int beta, int *res_x, int *res_y)
{
	int value, x, y, count, count_rev, best_x, best_y;
	Board next;
	Bool cut;

	// Return the init value if the max level is reached
	if (level <= 0)
	{
		return init;
	}

	debug("Step in level %d (searching %d)\n", level, type);

	best_x = -1;
	best_y = -1;
	cut = FALSE;

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			value = sum(init, (int)type * board_move(board, x, y, type, TRUE));

			if (value == 0)
			{
				continue;
			}
			
			debug("Valid move found: %d\n", value);

			// Make the move on a new board
			next = board_new(board->size);
			board_copy(&next, board);
			board_move(&next, x, y, type, FALSE);

			// Check if there are any moves left for the enemy
			if((pass && !board_count(&next, BLANK)) || (!pass && !board_moves_left(&next, minus(type))))
			{
				count = board_count(&next, type);
				count_rev = board_count(&next, minus(type));

				if (count > count_rev)
				{
					debug("Board is full: win\n");
					value = type * INF / 2;
				}
				else if (count == count_rev)
				{
					debug("Board is full: tie\n");
					value = minus(type) * INF / 3;
				}
				else
				{
					debug("Board is full: loss\n");
					value = minus(type) * INF / 2;
				}
			}
			else
			{
				value = ai_calc(&next, minus(type), value, level - 1, alpha, beta, NULL, NULL);
			}

			board_free(&next);

			// Max
			if (type == MAX && value > alpha || (random && rand() % 2 && value == alpha))
			{
				debug("New alpha: %d\n", value);
				alpha = value;
				best_x = x;
				best_y = y;
			}

			// Min
			else if (type == MIN && value < beta || (random && rand() % 2 && value == beta))
			{
				debug("New beta: %d\n", value);
				beta = value;
				best_x = x;
				best_y = y;
			}

			// Cut
			if (alpha >= beta)
			{
				debug("New alpha-beta cut (%d >= %d)\n", alpha, beta);
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

	debug("Step out from layer %d with %d\n", level, type == MAX ? alpha : beta);

	return type == MAX ? alpha : beta;
}

// Init the AI.
void ai_init(Bool _pass, Bool _random, int _level)
{
	pass = _pass;
	random = _random;
	level = _level;
}

// Get the best move coords with a type.
Bool ai_test(Board *board, Cell type, int *x, int *y)
{
	*x = -1;
	*y = -1;

	debug("Calculating AI move in %d layers...\n", level);
	ai_calc(board, type, 0, level, minus(INF), INF, x, y);
	debug("End of search (%d, %d)\n", *x, *y);

	if (*x >= 0 && *y >= 0)
	{
		return TRUE;
	}

	return FALSE;
}