#include "game.h"

// Create a new game with the default options.
Game game_new(void)
{
	List *config;
	Game game;
	
	config = list_new(TYPE, MAX);
	list_add(config, SIZE, 8);
	list_add(config, LEVEL, 3);
	list_add(config, DEMO, TRUE);
	list_add(config, PASS, TRUE);
	list_add(config, RANDOM, TRUE);
	list_add(config, RUCZ, FALSE);

	game.config = config;

	return game;
}

// Start the gameplay.
Bool game_start(Game *game, Bool (*input)(int *, int *), void (*before)(Bool), void (*dump)(Board *), void (*passed)(void))
{
	int x, y, count, count_rev;
	Bool pass, demo;
	Cell type;

	type = list_value(game->config, TYPE);
	pass = list_value(game->config, PASS);
	demo = list_value(game->config, DEMO);

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
			if (demo)
			{
				before(TRUE);
				ai_test(&game->board, type, &x, &y);
				board_move(&game->board, x, y, type, FALSE);
				dump(&game->board);
			}
			else if (input(&x, &y))
			{
				before(FALSE);

				while (board_move(&game->board, x, y, type, FALSE) == 0)
				{
					input(&x, &y);
				}
				
				dump(&game->board);
			}
			else
			{
				return FALSE;
			}
		}
		else if(!pass)
		{
			return TRUE;
		}
		else
		{
			passed();
		}

		// Reverse type
		if (count_rev > 0)
		{
			before(TRUE);
			ai_test(&game->board, minus(type), &x, &y);
			board_move(&game->board, x, y, minus(type), FALSE);
			dump(&game->board);
		}
		else if (!pass)
		{
			return TRUE;
		}
		else
		{
			passed();
		}
	}
}

// Export game options to a file.
Bool game_export(Game *game, char *path)
{
	List *config;
	FILE *file;

	if ((file = fopen(path, "w")) == NULL)
	{
		debug("ERROR: Could not export game!\n");
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
		debug("ERROR: Could not import game!\n");
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
	int random, level;
	Bool pass;

	random = list_value(game->config, RANDOM);
	level = list_value(game->config, LEVEL);
	pass = list_value(game->config, PASS);

	ai_init(pass, random, level);
	game->board = board_load(path);

	if (game->board.size > 0)
	{
		debug("ERROR: Could not load game!\n");
		return TRUE;
	}

	return FALSE;
}

// Save the board to a file.
Bool game_save(Game *game, char *path)
{
	return board_save(&game->board, path);
}

// End the game and get the winner cell.
Cell game_end(Game *game)
{
	int min, max;

	max = board_count(&game->board, MAX);
	min = board_count(&game->board, MIN);

	board_free(&game->board);

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
	int random, level, size;
	Bool pass;
	Cell type;

	type = list_value(game->config, TYPE);
	level = list_value(game->config, LEVEL);
	size = list_value(game->config, SIZE);
	pass = list_value(game->config, PASS);
	random = list_value(game->config, RANDOM);

	ai_init(pass, random, level);
	game->board = board_new(size);
	board_init(&game->board, random);
}

void game_free(Game *game)
{
	list_free(game->config);
}