#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CONFIG "config.cfg"
#define DEBUG 0

#define INF 32767

#define minus(t) -1 * t
#define sum(a, b) abs(a + b) > INF ? INF : a + b
#define debug(fmt, ...) if (DEBUG) { printf("DEBUG: "); printf(fmt, __VA_ARGS__); }

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

typedef enum Config
{
	TYPE = 0,
	SIZE = 1,
	LEVEL = 2,
	DEMO = 3,
	PASS = 4,
	RANDOM = 5
} Config;

typedef struct Board
{
	int size;
	Cell *cells;
} Board;

typedef struct List
{
	int key;
	int value;
	struct List *next;
	struct List *prev;
} List;

typedef struct Game
{
	Board board;
	List *config;
} Game;

// Board
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

// AI
void ai_init(Bool pass, Bool random, int level);
Bool ai_test(Board *board, Cell type, int *x, int *y);

// Game
Game game_new(void);
Bool game_start(Game *game, Bool(*coords)(int *, int *), void(*before)(Bool), void(*dump)(Board *), void(*passed)(void));
Bool game_export(Game *game, char *path);
Bool game_import(Game *game, char *path);
Bool game_load(Game *game, char *path);
Bool game_save(Game *game, char *path);
Cell game_end(Game *game);
void game_init(Game *game);
void game_free(Game *game);

// List
List *list_new(int key, int value);
List *list_get(List *list, int key);
int list_value(List *list, int key);
void list_add(List *list, int key, int value);
void list_seek_start(List **list);
void list_seek_end(List **list);
void list_free(List *list);

// Tui
void tui_start(Game *game);

// Main
int main(void);