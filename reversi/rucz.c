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
	.n_levels = 5,
	.enable_alpha_cuts = 1,
	.enable_beta_cuts = 1,
};

static field_t get_field(Cell source)
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

static void convert_board(Board *source, Cell type, board_t *dest)
{
	int x, y;

	dest->turn = get_field(type);

	for (y = 0; y < BOARD_SIZE; y++)
	{
		for (x = 0; x < BOARD_SIZE; x++)
		{
			dest->position[y][x] = get_field(board_get(source, x, y));
		}
	}
}

Bool rucz_test(Board *board, Cell type, int *x, int *y)
{
	reversi_ai_data_t aid = { .own = get_field(type) };
	board_t brd;
	move_t mv;

	convert_board(board, type, &brd);
	alpha_beta_move(&abd, &brd, &aid, &mv, &abst);

	*x = mv.col;
	*y = mv.row;

	return TRUE;
}