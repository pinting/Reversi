#include "tui.h"

static Game *game;

// Clear the screen.
static void tui_clear(void)
{
	int i;

	for (i = 0; i < 30; i++)
	{
		printf("\n");
	}
}

// Pause the game or eat new lines.
static void tui_pause(void)
{
	while (getchar() != '\n')
	{
		continue;
	}
}

// Dump the board to the stdout.
static void tui_dump(Board *board)
{
	char buffer[32];
	int x, y;

	printf("\n   ");

	// Print column index
	for (y = 0; y < board->size; y++)
	{
		sprintf(buffer, "%d", y + 1);
		printf("%c ", *(strrchr(buffer, 0) - 1));
	}

	printf("\n");

	for (y = 0; y < board->size; y++)
	{
		// Print row index
		sprintf(buffer, "%d", y + 1);
		printf("%c |", *(strrchr(buffer, 0) - 1));

		for (x = 0; x < board->size; x++)
		{
			// Print cells
			switch (board_get(board, x, y))
			{
				case MAX:
					printf("X");
					break;
				case MIN:
					printf("O");
					break;
				case BLANK:
					printf(" ");
					break;
				default:
					printf("E");
					break;
			}

			printf("|");
		}

		printf("\n");
	}

	printf("\n");
}

// Save the board.
static void tui_save(void)
{
	char buffer[32];

	printf("Name: ");

	if (scanf("%s", buffer) == 1)
	{
		tui_pause();
		game_save(game, buffer);
	}

}

// Get an input.
static Bool tuti_input(int *x, int *y)
{
	char buffer[32];

	while (1)
	{
		printf("Your move: ");
		fgets(buffer, 32, stdin);

		// Save the board and quit
		if (buffer[0] == 's')
		{
			tui_save();
			return FALSE;
		}

		// Quit from the game
		if (buffer[0] == 'q')
		{
			return FALSE;
		}

		// Make a move
		if (sscanf(buffer, "%d %d", x, y) == 2)
		{
			*x -= 1;
			*y -= 1;
			return TRUE;
		}
	}
}

// Executed before moves.
static void tui_before(Bool ai)
{
	if (ai)
	{
		printf("AI moving...\n");
	}
}

// Executed on pass.
static void tui_passed(void)
{
	printf("Pass!\n");
}

// End the game and print the winner.
static void tui_end(void)
{
	switch (game_end(game))
	{
		case MAX:
			printf("X won!\n");
			break;
		case MIN:
			printf("O won!\n");
			break;
		default:
			printf("Tie!\n");
			break;
	}

	tui_pause();
}

// Show help message before the gameplay.
static void tui_help(void)
{
	Cell type;

	printf("[s] Save\n");
	printf("[q] Quit\n\n");

	type = list_value(game->config, TYPE);
	printf("Playing as %c!\n", type == MAX ? 'X' : 'O');
}

// Start a new game.
static void tui_new(void)
{
	tui_clear();
	game_init(game);
	tui_help();

	if (game_start(game, &tuti_input, &tui_before, &tui_dump, &tui_passed))
	{
		tui_end();
	}
}

// Load a game.
static void tui_load(void)
{
	char buffer[32];

	printf("Name: ");

	if (scanf("%s", buffer) == 1)
	{
		tui_pause();
		tui_clear();
		tui_help();

		if (!game_load(game, buffer))
		{
			debug("Failed to load the game.\n");
		}
		else if (game_start(game, &tuti_input, &tui_before, &tui_dump, &tui_passed))
		{
			tui_end();
		}
	}
}

// Dump the config to the stdout.
static void tui_config_dump(void)
{
	List *config;

	config = game->config;

	for (; config != NULL; config = config->next)
	{
		printf("[%d] ", config->key);

		switch (config->key)
		{
			case TYPE:
				printf("Type: %c\n", config->value == MAX ? 'X' : 'O');
				break;
			case SIZE:
				printf("Size: %d\n", config->value);
				break;
			case LEVEL:
				printf("Level: %d\n", config->value);
				break;
			case DEMO:
				printf("Demo: %s\n", config->value ? "ON" : "OFF");
				break;
			case PASS:
				printf("Pass: %s\n", config->value ? "ON" : "OFF");
				break;
			case RANDOM:
				printf("Random: %s\n", config->value ? "ON" : "OFF");
				break;
		}
	}

	printf("[q] Quit\n\n");
}

// Show the config screen.
static void tui_config(void)
{
	int key, value, count;
	char buffer[32];
	List *config;

	tui_clear();
	tui_config_dump();

	while (scanf("%d %s", &key, &buffer) == 2)
	{
		tui_pause();
		config = game->config;

		if ((config = list_get(config, key)) == NULL)
		{
			continue;
		}

		switch (key)
		{
			case TYPE:
				value = buffer[0] == 'X' ? MAX : MIN;
				break;
			case SIZE:
			case LEVEL:
				count = sscanf(buffer, "%d", &value);
				value = count == 1 && value > 0 ? value : 4;
				break;
			case DEMO:
			case PASS:
			case RANDOM:
				value = buffer[1] == 'N' ? TRUE : FALSE;
				break;
		}

		config->value = value;
		game_export(game, CONFIG);
		tui_clear();
		tui_config_dump();
	}

	tui_pause();
}

// Show the main menu.
void tui_start(Game *_game)
{
	char buffer;

	game = _game;

	while (1)
	{
		tui_clear();

		printf(" _ __ _____   _____ _ __ ___ _ \n");
		printf("|  __/ _ \\ \\ / / _ \\  __/ __| |\n");
		printf("| | |  __/\\ V /  __/ |  \\__ \\ |\n");
		printf("|_|  \\___| \\_/ \\___|_|  |___/_|\n\n");

		printf("[1] New game\n");
		printf("[2] Load game\n");
		printf("[3] Config\n");
		printf("[q] Quit\n\n");

		scanf("%c", &buffer);
		tui_pause();

		switch (buffer)
		{
			case '1':
				tui_new();
				break;
			case '2':
				tui_load();
				break;
			case '3':
				tui_config();
				break;
			case 'q':
				return;
		}
	}
}