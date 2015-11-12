#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "list.h"
#include "ai.h"
#include "rucz.h"

#define CONFIG "config.cfg"

typedef enum Config
{
	TYPE,
	SIZE,
	DEMO,
	PASS,
	RANDOM,
	LEVEL,
	RUCZ
} Config;

typedef struct Game
{
	Board board;
	List *config;
} Game;

Game game_new(void);
Bool game_start(Game *game, Bool(*coords)(int *, int *), void(*before)(Bool), void(*dump)(Board *), void(*passed)(void));
Bool game_ai(Game *game, Cell type, int *x, int *y, int rucz);
Bool game_export(Game *game, char *path);
Bool game_import(Game *game, char *path);
Bool game_load(Game *game, char *path);
Bool game_save(Game *game, char *path);
Cell game_end(Game *game);
void game_init(Game *game);
void game_free(Game *game);

#endif
