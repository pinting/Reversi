#include "rucz.h"

/// Convert cell to field.
/// @param source Cell type.
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

/// Convert the board to the rucz format.
/// @param source Board struct.
/// @param type Cell type.
/// @param dest Rucz board_t struct.
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

Bool rucz_test(Rucz *rucz, Board *board, Cell type, int *x, int *y)
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
	alpha_beta_move(&rucz->abd, &brd, &aid, &mv, &rucz->abst);

	if (mv.col < 0 || mv.row < 0)
	{
		return FALSE;
	}

	*x = mv.col;
	*y = mv.row;

	return TRUE;
}

void rucz_free(Rucz *rucz)
{
	alpha_beta_finish(&rucz->abd);
}

Rucz rucz_init(int level)
{
	Rucz rucz;

	rucz.abd.funs.get_move_size = (alpha_beta_get_size_fun_t)get_move_size;
	rucz.abd.funs.get_board_size = (alpha_beta_get_size_fun_t)get_board_size;
	rucz.abd.funs.make_move = (alpha_beta_make_move_fun_t)make_move;
	rucz.abd.funs.is_game_ended = (alpha_beta_is_game_ended_fun_t)is_game_ended;
	rucz.abd.funs.get_valid_moves_size = (alpha_beta_get_size_fun_t)get_valid_moves_size;
	rucz.abd.funs.get_valid_moves = (alpha_beta_get_valid_moves_fun_t)get_valid_moves;
	rucz.abd.funs.eval_position = (alpha_beta_eval_position_fun_t)eval_position;
	rucz.abd.funs.get_move = (alpha_beta_get_move_fun_t)get_move;
	rucz.abd.enable_alpha_cuts = 1;
	rucz.abd.enable_beta_cuts = 1;
	rucz.abd.n_levels = level;

	alpha_beta_init(&rucz.abd);

	return rucz;
}