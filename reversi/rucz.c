#include "rucz.h"

static alpha_beta_stats_t abst;
static alpha_beta_data_t abd = {
	.funs = {
		.get_move_size = (alpha_beta_get_size_fun_t)get_move_size,
		.get_board_size = (alpha_beta_get_size_fun_t)get_board_size,
		.make_move = (alpha_beta_make_move_fun_t)make_move,
		.is_game_ended = (alpha_beta_is_game_ended_fun_t)is_game_ended,
		.get_valid_moves_size = (alpha_beta_get_size_fun_t)get_valid_moves_size,
		.get_valid_moves = (alpha_beta_get_valid_moves_fun_t)get_valid_moves,
		.eval_position = (alpha_beta_eval_position_fun_t)eval_position,
		.get_move = (alpha_beta_get_move_fun_t)get_move,
	},
	.enable_alpha_cuts = 1,
	.enable_beta_cuts = 1,
};

// Convert cell to field.
static field_t rucz_field(Cell source)
{
	switch (source)
	{
		case MAX:
			return BLACK;
		case MIN:
			return WHITE;
		case BLANK:
			return EMPTY;
		default:
			return EMPTY;
	}
}

// Convert the board to the rucz format.
static void rucz_board(Board *source, Cell type, board_t *dest)
{
	int x, y;

	for (y = 0; y < BOARD_SIZE; y++)
	{
		for (x = 0; x < BOARD_SIZE; x++)
		{
			dest->position[y][x] = rucz_field(board_get(source, x, y));
		}
	}

	dest->turn = rucz_field(type);
}

// Search for the best move.
Bool rucz_test(Board *board, Cell type, int *x, int *y)
{
	reversi_ai_data_t aid;
	board_t brd;
	move_t mv;

	// Sadly rucz only supports fixed sized boards - for now
	if (board->size != BOARD_SIZE)
	{
		return FALSE;
	}

	aid.own = rucz_field(type);
	mv.col = -1;
	mv.row = -1;

	rucz_board(board, type, &brd);
	alpha_beta_move(&abd, &brd, &aid, &mv, &abst);

	if (mv.col < 0 || mv.row < 0)
	{
		return FALSE;
	}

	*x = mv.col;
	*y = mv.row;

	return TRUE;
}

// Init the rucz AI.
void rucz_init(int level)
{
	alpha_beta_init(&abd);
	abd.n_levels = level;
}

// Free it up.
void rucz_free()
{
	alpha_beta_finish(&abd);
}