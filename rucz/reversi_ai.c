#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "board.h"
#include "console_interface.h"
#include "reversi_ai.h"

size_t get_valid_moves_size(void)
{
	return sizeof(valid_moves_t);
}

static int _min(int a, int b)
{
	return a > b ? b : a;
}

int get_valid_moves(board_t *brd, valid_moves_t *vm)
{
	int r, c;
	int max_r0, max_c0;
	
	vm->s = board_symmetry(brd);
	vm->n = 0;
	
	max_r0 = vm->s & BOARD_SYMM_FLAG_NS ? BOARD_SIZE/2 : BOARD_SIZE;
	max_c0 = vm->s & BOARD_SYMM_FLAG_WE ? BOARD_SIZE/2 : BOARD_SIZE;
	
	for (r = 0; r < max_r0; ++r)
	{
		int max_c = max_c0;
		if (vm->s & BOARD_SYMM_FLAG_NWSE) max_c = min(max_c0, BOARD_SIZE - r);
		if (vm->s & BOARD_SYMM_FLAG_NESW) max_c = min(max_c, r);
		
		for (c = 0; c < max_c; ++c)
		{
			move_t tmp_move = {
				.is_pass = 0,
				.row = r,
				.col = c,
			};
			if (!check_move(brd, &tmp_move))
			{
				memcpy(&vm->moves[vm->n], &tmp_move, sizeof(move_t));
				vm->n++;
			}
		}
	}
	
	/* Pass, if no options */
	if (vm->n == 0)
	{
		vm->n = 1;
		vm->moves[0].is_pass = 1;
	}
	return vm->n;
}

double board_density(board_t *brd)
{
	int n = 0;
	int r, c;
	for (r = 0; r < BOARD_SIZE; ++r)
		for (c = 0; c < BOARD_SIZE; ++c)
			if (brd->position[r][c] != EMPTY)
				++n;
	return (double)n / (double)(BOARD_SIZE * BOARD_SIZE);
}

static field_t opponent(field_t f)
{
	return (f == BLACK) ? WHITE : BLACK; 
}

double eval_position(board_t *brd, reversi_ai_data_t *data)
{
	int r, c;
	int mine, opps;
	double ret = 0.0;
	field_t own = data->own;
	
	field_t opp = opponent(data->own);
	mine = opps = 0;
	for (r = 0; r < BOARD_SIZE; ++r)
	{
		for (c = 0; c < BOARD_SIZE; ++c)
		{
			if (brd->position[r][c] == own)
			{
				ret += 1.0;
				mine++;
			}
			else if (brd->position[r][c] == opp)
			{
				ret -= 1.0;
				opps++;
			}
		}
	}
	
	/* If the board is full */
	if (mine + opps == BOARD_SIZE * BOARD_SIZE)
	{
		if (mine > opps) ret = (INT_MAX); // +Inf
		else { 
			if (opps > mine) ret = (INT_MIN); // -Inf
			else ret = 0.0;
		}
	}
	
	return ret;
}

void print_valid_moves(valid_moves_t *vm)
{
	int i;
	printf("Valid moves: %d (", vm->n);
	for (i = 0; i < vm->n; ++i)
	{
		print_move(&vm->moves[i]);
		if (i < vm->n -1)
			printf(", ");
	}
	printf(")\n");
}


/**
 * \brief get a move from the valid moves
 * \details
 * Returns the \b i th move from the valid moves array. If the position is 
 * symmetric, the move is rotated or mirrored randomly, according to the 
 * symmetry.
 */
int get_move(int i, valid_moves_t *vm, move_t *move)
{
	/* Check validity of index */
	if (i < 0 || i >= vm->n)
		return -1;
	else
	{
		memcpy(move, &vm->moves[i], sizeof(move_t));
		/* If the move is not pass, and the position is symmetric */
		if (!vm->moves[i].is_pass && vm->s != BOARD_SYMM_NO_SYMMETRY)
		{
			if (vm->s & BOARD_SYMM_FLAG_NS)
				if (rand() % 2) 
					move->row = BOARD_SIZE - 1 - vm->moves[i].row;
			if (vm->s & BOARD_SYMM_FLAG_WE)
				if (rand() % 2) 
					move->col = BOARD_SIZE - 1 - vm->moves[i].col;
			if (vm->s & BOARD_SYMM_FLAG_NWSE)
				if (rand() % 2)
				{
					move->row = BOARD_SIZE - 1 - vm->moves[i].col;
					move->col = BOARD_SIZE - 1 - vm->moves[i].row;
				}
			if (vm->s & BOARD_SYMM_FLAG_NESW)
				if (rand() % 2)
				{
					move->row = vm->moves[i].col;
					move->col = vm->moves[i].row;
				}
		}
	}
	return 0;
}