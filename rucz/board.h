/**
 * \file board.h
 * \brief interface of the board types and functions
 */

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <stddef.h>

#define BOARD_SIZE (8)

#define BOARD_SYMM_NO_SYMMETRY (0)
#define BOARD_SYMM_FLAG_NS (1 << 0)
#define BOARD_SYMM_FLAG_WE (1 << 1)
#define BOARD_SYMM_FLAG_NWSE (1 << 2)
#define BOARD_SYMM_FLAG_NESW (1 << 3)

typedef enum {
	EMPTY = 0,
	BLACK = 1,
	WHITE = 2
} field_t;

typedef enum {
	GAME_GOES_ON = 0,
	BLACK_WON = 1,
	WHITE_WON = 2,
	GAME_DRAW = 3
} game_result_t;

typedef struct {
	int is_pass;
	int row;
	int col;
} move_t;

typedef struct {
	field_t turn;
	field_t position[BOARD_SIZE][BOARD_SIZE];
} board_t;

size_t get_board_size(void);
size_t get_move_size(void);

void init_board(board_t *);
int is_game_ended(board_t *brd);
game_result_t score_board(board_t *, int *, int *);

int check_move(board_t *, move_t *);
int make_move(board_t *, move_t *);

int board_symmetry(board_t *brd);

#endif /* BOARD_H */