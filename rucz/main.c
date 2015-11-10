#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "alpha_beta.h"
#include "board.h"
#include "console_interface.h"
#include "reversi_ai.h"

int main()
{
	int b, w;
	board_t brd;
	move_t mv;
	
	valid_moves_t val;
	game_result_t result;
	
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
	
	alpha_beta_stats_t abst;
	
	reversi_ai_data_t aid = {
		.own = WHITE,
	};
	
	srand((unsigned int) time(NULL));

	alpha_beta_init(&abd);

	init_board(&brd);
	
	while (!is_game_ended(&brd))
	{
		print_board(&brd);
		get_valid_moves(&brd, &val);
		print_valid_moves(&val);
		
		read_move(&mv);
		print_move(&mv);
		
		printf("\n");
		
		if (!check_move(&brd,&mv))
		{
			double value;
			printf("Move valid!\n");
			make_move(&brd, &mv);
			print_board(&brd);
			if (is_game_ended(&brd))
				break;
			get_valid_moves(&brd, &val);
			print_valid_moves(&val);
			printf("Evaluation: %.2f (dens: %.3f)\n", eval_position(&brd, &aid), board_density(&brd));
			value = alpha_beta_move(&abd, &brd, &aid, &mv, &abst);
			printf("Value: %.2f (depth: %d)  [ #pos: %d, #alpha: %d, #beta: %d ]\n", 
				value, abst.max_depth, abst.n_eval_pos, abst.n_alpha_cuts, abst.n_beta_cuts);
			
			printf("Comp's move: "); print_move(&mv); printf("\n");
			make_move(&brd, &mv);
		}
		else
		{
			printf("Invalid move!\n");
		}
	
	}
	
	print_board(&brd);
	printf("Game ended!\n");
	result = score_board(&brd, &b, &w);
	print_score(b, w);
	print_game_result(result);
	
	alpha_beta_finish(&abd);
	
	return 0;
}