#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 0

#define debug(n, fmt, ...) if (DEBUG >= n) { printf("DEBUG: "); printf(fmt, __VA_ARGS__); }
#define minus(t) -1 * t

typedef enum Bool
{
	FALSE = 0,
	TRUE = 1
} Bool;

typedef enum Cell
{
	MIN = -1,
	MAX = 1,
	BLANK = 0,
	ERROR = 2
} Cell;

typedef struct Board
{
	int size;
	Cell *cells;
} Board;

Board board_new(int size);
Board board_load(char *path);
Cell board_get(Board *board, int x, int y);
Bool board_check(Board *board, int x, int y, Cell type);
Bool board_set(Board *board, int x, int y, Cell type);
Bool board_inside(Board *board, int x, int y);
Bool board_save(Board *board, char *path);
int board_move(Board *board, int x, int y, Cell type, Bool test);
int board_moves_left(Board *board, Cell type);
int board_corner(Board *board, int x, int y);
int board_count(Board *board, Cell type);
void board_copy(Board *target, Board *source);
void board_init(Board *board, Bool random);
void board_free(Board *board);

#endif