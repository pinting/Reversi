#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "list.h"
#include "jam_ai.h"

#define CONFIG "config.cfg"

typedef enum Config
{
	TYPE = 0,
	SIZE = 1,
	LEVEL = 2,
	DEMO = 3,
	PASS = 4,
	RANDOM = 5
} Config;

typedef struct Game
{
	Board board;
	List *config;
} Game;

Game game_new(void);
Bool game_start(Game *game, Bool(*coords)(int *, int *), void(*before)(Bool), void(*dump)(Board *), void(*passed)(void));
Bool game_export(Game *game, char *path);
Bool game_import(Game *game, char *path);
Bool game_load(Game *game, char *path);
Bool game_save(Game *game, char *path);
Cell game_end(Game *game);
void game_init(Game *game);
void game_free(Game *game);

#endif
