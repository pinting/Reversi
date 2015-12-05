#ifndef GAME_H
#define GAME_H

#include <time.h>
#include "board.h"
#include "list.h"
#include "ai.h"
#include "rucz.h"

/// Config file where the options will be held.
#define CONFIG "config.cfg"

/// Save the board when the game ends.
#define END_SAVE 0

/// Shorthand config search.
#define cfg(key) list_value(game->config, key)

/// Config types.
typedef enum Config
{
	/// Type of the player.
	TYPE,
	/// Size of the board.
	SIZE,
	/// Demo mode.
	DEMO,
	/// Enable pass.
	PASS,
	/// Enable random.
	RANDOM,
	/// Maximum level.
	LEVEL,
	/// Enable rucz AI.
	RUCZ
} Config;

/// Game structure which holds a Board struct 
/// and a config linked list.
typedef struct Game
{
	/// Board struct.
	Board board;
	/// Config list.
	List *config;
} Game;

/// Create a new game with the default options.
Game game_new(void);

/// Start the gameplay.
/// @param game Game struct.
/// @param coords A function which give back the user specified coords.
/// @param before A function which will be called before every turn.
/// @param dump A function which dumps the board.
/// @param passed Called when a player passes.
Bool game_start(Game *game, Bool(*coords)(int *, int *), void(*before)(Bool, Cell), void(*dump)(Board *, int, int), void(*passed)(Cell));

/// Calculate the best move with the chosen AI.
/// @param game Game struct.
/// @param type Cell type.
/// @param x X coord.
/// @param y Y coord.
/// @param rucz Enable rucz AI?
Bool game_ai(Game *game, Cell type, int *x, int *y, Bool rucz);

/// Export game options to a file.
/// @param game Game struct.
/// @param path File path.
Bool game_export(Game *game, char *path);

/// Import options of a game from a file.
/// @param game Game struct.
/// @param path File path.
Bool game_import(Game *game, char *path);

/// Init the game and load the board from a save file.
/// @param game Game struct.
/// @param path Path of the save file.
Bool game_load(Game *game, char *path);

/// Save the board to a file.
/// @param game Game struct.
/// @param path Path of the save file.
Bool game_save(Game *game, char *path);

/// End the game and get the winner cell.
/// @param game Game struct.
Cell game_end(Game *game);

/// Init the game with the given options.
/// @param game Game struct.
void game_init(Game *game);

/// Free up the game.
/// @param game Game struct.
void game_free(Game *game);

#endif
