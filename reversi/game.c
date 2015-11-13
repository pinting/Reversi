#include "game.h"

// Create a new game with the default options.
Game game_new(void)
{
	List *config;
	Game game;
	
	config = list_new(TYPE, MAX);
	list_add(config, SIZE, 8);
	list_add(config, DEMO, TRUE);
	list_add(config, PASS, TRUE);
	list_add(config, RANDOM, TRUE);
	list_add(config, LEVEL, 3);
	list_add(config, RUCZ, 0);

	game.config = config;

	return game;
}

// Start the gameplay.
Bool game_start(Game *game, Bool (*input)(int *, int *), void (*before)(Bool, Cell), void (*dump)(Board *), void (*passed)(void))
{
	int x, y, count, count_rev;
	Cell type;

	type = cfg(TYPE);
	dump(&game->board);

	while (1)
	{
		count_rev = board_moves_left(&game->board, minus(type));
		count = board_moves_left(&game->board, type);

		if (!count && !count_rev)
		{
			return TRUE;
		}

		// Our type
		if (count > 0)
		{
			if (cfg(DEMO))
			{
				before(TRUE, type);
				game_ai(game, type, &x, &y, cfg(RUCZ) < 0);
				board_move(&game->board, x, y, type, FALSE);
				dump(&game->board);
			}
			else
			{
				before(FALSE, type);

				do
				{
					if (!input(&x, &y))
					{
						return FALSE;
					}
				} while (board_move(&game->board, x, y, type, FALSE) == 0);
				
				dump(&game->board);
			}
		}
		else if(cfg(PASS))
		{
			passed();
		}
		else
		{
			return TRUE;
		}

		// Reverse type
		if (count_rev > 0)
		{
			before(TRUE, minus(type));
			game_ai(game, minus(type), &x, &y, cfg(RUCZ) != 0);
			board_move(&game->board, x, y, minus(type), FALSE);
			dump(&game->board);
		}
		else if (cfg(PASS))
		{
			passed();
		}
		else
		{
			return TRUE;
		}
	}
}

// Calculate the best move with the chosen AI.
Bool game_ai(Game *game, Cell type, int *x, int *y, Bool rucz)
{
	if (rucz && rucz_test(&game->board, type, x, y))
	{
		debug(2, "Using rucz AI\n");
		return TRUE;
	}
	else if (rucz)
	{
		debug(1, "Failed to use rucz AI.");
	}

	// Sometimes the rucz AI finds no more moves while the default one does
	return ai_test(&game->board, type, x, y);
}

// Export game options to a file.
Bool game_export(Game *game, char *path)
{
	List *config;
	FILE *file;

	if ((file = fopen(path, "w")) == NULL)
	{
		debug(1, "Could not export game!\n");
		return FALSE;
	}

	config = game->config;

	for (; config != NULL; config = config->next)
	{
		fprintf(file, "%d\n", config->value);
	}

	fclose(file);
	return TRUE;
}

// Import options of a game from a file.
Bool game_import(Game *game, char *path)
{
	List *config;
	FILE *file;
	char buffer[8];
	int i, value;

	if ((file = fopen(path, "r")) == NULL)
	{
		debug(1, "Could not import game!\n");
		return FALSE;
	}

	config = game->config;

	for (i = 0; config != NULL && fgets(buffer, 8, file) != NULL; i++)
	{
		if (sscanf(buffer, "%d", &value) != 1)
		{
			break;
		}

		config->value = value;
		config = config->next;
	}

	fclose(file);
	return TRUE;
}

// Init the game and load a board from a save file.
Bool game_load(Game *game, char *path)
{
	char buffer[64];

	sprintf(buffer, "%s.sav", path);
	rucz_init(cfg(LEVEL));
	ai_init(cfg(PASS), cfg(RANDOM), cfg(LEVEL));
	game->board = board_load(buffer);

	if (!game->board.size)
	{
		debug(1, "Could not load game!\n");
		return FALSE;
	}

	return TRUE;
}

// Save the board to a file.
Bool game_save(Game *game, char *path)
{
	char buffer[64];

	sprintf(buffer, "%s.sav", path);

	return board_save(&game->board, buffer);
}

// End the game and get the winner cell.
Cell game_end(Game *game)
{
	int min, max, now;
	char buffer[32];

	max = board_count(&game->board, MAX);
	min = board_count(&game->board, MIN);

	if (END_SAVE)
	{
		now = (int)time(NULL);
		sprintf(buffer, "%d", now);
		game_save(game, buffer);
		debug(1, "Saved game to %s\n", buffer);
	}

	board_free(&game->board);
	rucz_free();

	if (max > min)
	{
		return MAX;
	}
	else if (min < max)
	{
		return MIN;
	}
	else
	{
		return BLANK;
	}
}

// Init the game with the given options.
void game_init(Game *game)
{
	rucz_init(abs(cfg(RUCZ)));
	ai_init(cfg(PASS), cfg(RANDOM), cfg(LEVEL));
	game->board = board_new(cfg(SIZE));
	board_init(&game->board, cfg(RANDOM));
}

// Free up the game.
void game_free(Game *game)
{
	list_free(game->config);
}