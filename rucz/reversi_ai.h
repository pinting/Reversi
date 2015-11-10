#ifndef REVERSI_AI_H
#define REVERSI_AI_H

#include <stdint.h>
#include "board.h"

typedef struct {
	int n;
	int s;
	move_t moves[BOARD_SIZE*BOARD_SIZE];
} valid_moves_t;

typedef struct {
	field_t own;
} reversi_ai_data_t;

size_t get_valid_moves_size(void);

int get_valid_moves(board_t *, valid_moves_t *);
void print_valid_moves(valid_moves_t *vm);

double board_density(board_t * brd);
double eval_position(board_t *brd, reversi_ai_data_t *data);

int get_move(int, valid_moves_t *, move_t *);

#endif /* REVERSI_AI_H */