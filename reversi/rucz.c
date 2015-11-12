#include "rucz.h"

static field_t rucz_field(Cell source)
{
	switch (source)
	{
		case MAX:
			return BLACK;
			break;
		case MIN:
			return WHITE;
			break;
		case BLANK:
			return EMPTY;
			break;
	}

	return EMPTY;
}

static void rucz_board(Board *source, board_t *board)
{
	int x, y;

	init_board(board);
	
	for (y = 0; y < SIZE; y++)
	{
		for (x = 0; y < SIZE; x++)
		{
			board->position[y][x] = rucz_field(board_get(source, x, y));
		}
	}
}

Bool rucz_test(Board *board, Cell type, int *x, int *y)
{
    alpha_beta_data_t abd = {
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
		.n_levels = 5,
		.enable_alpha_cuts = 1,
		.enable_beta_cuts = 1,
	};
	reversi_ai_data_t aid = {
		.own = WHITE,
	};
	move_t mv = {
		.is_pass = 0,
		.row = 0,
		.col = 0
	};
	alpha_beta_stats_t abst;
	board_t brd;

	alpha_beta_move(&abd, &brd, &aid, &mv, &abst);

	return TRUE;
}