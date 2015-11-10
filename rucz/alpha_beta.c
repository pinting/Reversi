#include <stdlib.h>
#include <string.h>

#include "alpha_beta.h"



void alpha_beta_init(alpha_beta_data_t *abd)
{
	int l;
	
	alpha_beta_funs_t *funs = &(abd->funs);
	
	abd->move_size = funs->get_move_size();
	abd->board_size = funs->get_board_size();
	abd->valid_moves_size = funs->get_valid_moves_size();
	
	/* Allocate memories */
	abd->tmp_move = malloc(abd->move_size);
	abd->board_array = malloc(abd->board_size * ALPHA_BETA_MAX_LEVELS);
	abd->valid_moves_array = malloc(abd->valid_moves_size * ALPHA_BETA_MAX_LEVELS);
	
	/* Set pointers to level data */
	for (l = 0; l < ALPHA_BETA_MAX_LEVELS; ++l)
	{
		abd->level_data[l].depth = l;
		abd->level_data[l].board = (char*)abd->board_array + l*abd->board_size;
		abd->level_data[l].valid_moves = (char*)abd->valid_moves_array + l*abd->valid_moves_size;
	}
}

void alpha_beta_finish(alpha_beta_data_t *abd)
{
	if (abd->tmp_move) free(abd->tmp_move);
	if (abd->board_array) free(abd->board_array);
	if (abd->valid_moves_array) free(abd->valid_moves_array);
}


/**
 * \param[in] abd pointer to alpha beta data structure
 * \param[in] brd pointer to board data
 * \param[in] aid pointer to data to be passed to AI's eval_position function
 * \param[out] mv the resulting move
 * \param[out] st statistics of the alpha beta process
 */
double alpha_beta_move(alpha_beta_data_t *abd, void *brd, void* aid, void *mv, alpha_beta_stats_t *st)
{
	int curr_level = 0;
	alpha_beta_level_data_t * ld0 = &abd->level_data[0];
	
	st->n_eval_pos = 0;
	st->max_depth = 0;
	st->n_alpha_cuts = 0;
	st->n_beta_cuts = 0;
	
	/* Copy the board data */
	memcpy(ld0->board, brd, abd->board_size);
	/* Get possible moves */
	ld0->n_valid_moves = abd->funs.get_valid_moves(ld0->board, ld0->valid_moves);
	
	/* If there's only one possibility at top level */
	if (abd->level_data[0].n_valid_moves == 1)
	{
		abd->funs.get_move(0, abd->level_data[0].valid_moves, mv);
		return (0); // NaN
	}
	
	/* Go throgh the possibilities */
	ld0->current_move_idx = 0;
	ld0->selected_move_value = (INT_MIN); // -Inf
	while (curr_level >= 0)
	{
		alpha_beta_level_data_t * cld = &abd->level_data[curr_level];
		
		if (curr_level > (int)(st->max_depth))
			st->max_depth = curr_level;
		
		/* At leaf level or at a position where the game ends */
		if (curr_level == abd->n_levels || abd->funs.is_game_ended(cld->board)
			|| (cld->current_move_idx == cld->n_valid_moves)
		)
		{
			double value;
			alpha_beta_level_data_t *pld = &abd->level_data[curr_level-1];
			
			if (curr_level == 0)
			{
				curr_level--;
				continue;
			}
			
			/* Evaluate position */
			if (curr_level == abd->n_levels)
			{
				value= abd->funs.eval_position(cld->board, aid);
				/* Increase statistics counter */
				st->n_eval_pos++;
			}
			else
			{
				value = cld->selected_move_value;
			}
				
			/* TODO: implement cuts */
			if (pld->depth % 2 == 0)  /* Parent is MAX level */
			{
				/* Update selected move as needed */
				if (pld->selected_move_value < value || 
					(pld->selected_move_value == value && rand() % 2))
				{
					pld->selected_move_value = value;
					pld->selected_move_idx = cld->parent_move_idx;
					
					/* Check if alpha cut is possible */
					if (abd->enable_alpha_cuts && curr_level >= 2)
					{
						alpha_beta_level_data_t *gpld = &abd->level_data[curr_level-2];
	
						/* If grandparent value is smaller, we can cut */
						if (gpld->selected_move_value <= pld->selected_move_value)
						{
							curr_level -= 2;
							st->n_alpha_cuts++;
							continue;
						}
					}	
				}
			}
			else /* Parent is MIN level */
			{
				if (pld->selected_move_value > value || 
					(pld->selected_move_value == value && rand() % 2))
				{
					pld->selected_move_value = value;
					pld->selected_move_idx = cld->parent_move_idx;
					
					/* Check if beta cut is possible */
					if (abd->enable_beta_cuts && curr_level >= 2)
					{
						alpha_beta_level_data_t *gpld = &abd->level_data[curr_level-2];
	
						/* If grandparent value is smaller, we can cut */
						if (gpld->selected_move_value >= pld->selected_move_value)
						{
							curr_level -= 2;
							st->n_beta_cuts++;
							continue;
						}
					}	
				}
			}
			
			/* Decrease the level counter */
			curr_level--;
		}
		/* At higher levels */
		else
		{
			alpha_beta_level_data_t * nld = &abd->level_data[curr_level+1];
		
			/* Initialize the next level */
			/* 1. copy the board */
			memcpy(nld->board, cld->board, abd->board_size);
			/* 2. make the chosen move on child board */
			abd->funs.get_move(cld->current_move_idx, cld->valid_moves, abd->tmp_move);
			abd->funs.make_move(nld->board, abd->tmp_move);
			/* 3. get the possible moves from the given position */
			nld->n_valid_moves = abd->funs.get_valid_moves(nld->board, nld->valid_moves);
			/* 4. reset the current move counter */
			nld->current_move_idx = 0;
			/* 5. set the current value */
			nld->selected_move_value = (nld->depth % 2 == 0) ? (INT_MIN) : (INT_MAX); // -Inf, +Inf
			/* Increase the current move index */
			nld->parent_move_idx = cld->current_move_idx;
			cld->current_move_idx++;
			/* Increas current level index */
			curr_level++;
		}
	}
	
	/* Copy the selected move */
	abd->funs.get_move(ld0->selected_move_idx, ld0->valid_moves, mv); 
	
	return abd->level_data[0].selected_move_value;
}