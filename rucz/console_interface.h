#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include "board.h"

void print_board(board_t *);
void print_game_result(game_result_t);
void print_score(int, int);
void print_symmetry(int);

int read_move(move_t *);
void print_move(move_t *);

#endif /* CONSOLE_INTERFACE_H */
