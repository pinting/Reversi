#include "board.h"

Board board_new(int size)
{
	Board board;

	board.cells = (Cell *)malloc(size * sizeof(Cell) * size * sizeof(Cell));
	board.size = size;

	return board;
}

Board board_load(char *path)
{
	Bool first = FALSE;
	Board board;
	FILE *file;
	char buffer[8];
	int value;
	int pos = 0;
	int x = 0;
	int y = 0;

	if ((file = fopen(path, "r")) == NULL)
	{
		debug(1, "Could not load the board!\n");
		board.size = 0;
		return board;
	}

	while (fgets(buffer, 8, file) != NULL)
	{
		if (sscanf(buffer, "%d", &value) != 1)
		{
			break;
		}

		if (!first)
		{
			first = TRUE;
			board = board_new(value);
		}
		else
		{
			board_set(&board, x, y, (Cell)value);
			
			if (++x >= board.size)
			{
				x = 0;
				y++;
			}
		}
	}

	fclose(file);
	return board;
}

Cell board_get(Board *board, int x, int y)
{
	if (board_inside(board, x, y))
	{
		return *(board->cells + board->size * sizeof(Cell) * y + sizeof(Cell) * x);
	}

	debug(1, "Coords are outside the board!\n");
	return 0;
}

Bool board_check(Board *board, int x, int y, Cell type)
{
	if (board_inside(board, x, y) && board_get(board, x, y) == type)
	{
		return TRUE;
	}

	return FALSE;
}

Bool board_set(Board *board, int x, int y, Cell type)
{
	if (board_inside(board, x, y))
	{
		*(board->cells + board->size * sizeof(Cell) * y + sizeof(Cell) * x) = type;
		return TRUE;
	}

	debug(1, "Coords are outside the board!\n");
	return FALSE;
}

Bool board_inside(Board *board, int x, int y)
{
	if (x < board->size && y < board->size && x >= 0 && y >= 0)
	{
		return TRUE;
	}

	debug(3, "Cell (%d, %d) was not inside board (sized %d)!\n", x, y, board->size);
	return FALSE;
}

Bool board_save(Board *board, char *path)
{
	FILE *file;
	int x, y;

	if ((file = fopen(path, "w")) == NULL)
	{
		debug(1, "Could not save the board!\n");
		return FALSE;
	}

	fprintf(file, "%d\n", board->size);

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			fprintf(file, "%d\n", board_get(board, x, y));
		}
	}

	fclose(file);
	return TRUE;
}

/// Draw a line on the board with the given type - only 8 directions are supported.
/// @param board Board structure.
/// @param type Cell type.
/// @param x1 From X coord.
/// @param y1 From Y coord.
/// @param x2 To X coord.
/// @param y2 To Y coord.
static void board_line(Board *board, Cell type, int x1, int y1, int x2, int y2)
{
	int step_x = x1 == x2 ? 0 : x1 < x2 ? 1 : -1;
	int step_y = y1 == y2 ? 0 : y1 < y2 ? 1 : -1;

	while (1)
	{
		board_set(board, x1, y1, type);

		if (x1 == x2 && y1 == y2)
		{
			return;
		}

		x1 += step_x;
		y1 += step_y;
	}
}

int board_move(Board *board, int x, int y, Cell type, Bool test)
{
	int step_x, step_y, last_x, last_y, i;
	int count = 0;

	if (!board_check(board, x, y, BLANK))
	{
		return count;
	}

	for (step_x = -1; step_x <= 1; step_x += 1)
	{
		for (step_y = -1; step_y <= 1; step_y += 1)
		{
			if (step_y == 0 && step_x == 0)
			{
				continue;
			}

			last_x = x;
			last_y = y;
			i = 0;

			while(1)
			{
				last_x += step_x;
				last_y += step_y;

				// Check if the cell is an enemy cell
				if (board_check(board, last_x, last_y, minus(type)))
				{
					i += board_corner(board, last_x, last_y) + 1;
					continue;
				}

				// If it is not, then
				if (board_check(board, last_x, last_y, type))
				{
					// If there were no enemy cell(s) before, break it
					if (i == 0)
					{
						break;
					}

					// Else, reverse them
					if (!test)
					{
						board_line(board, type, x, y, last_x, last_y);
						debug(3, "Place type %d from (%d, %d) to (%d, %d)\n", type, x, y, last_x, last_y);
					}

					count += i;
				}

				break;
			}
		}
	}

	return count;
}

int board_moves_left(Board *board, Cell type)
{
	int count = 0;
	int x, y;

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			if (board_move(board, x, y, type, TRUE) > 0)
			{
				count++;
			}
		}
	}

	return count;
}

int board_corner(Board *board, int x, int y)
{
	if ((x == 0 && y == 0) ||
		(x == 0 && y == board->size) ||
		(x == board->size && y == 0) ||
		(x == board->size && y == board->size))
	{
		return 2;
	}

	if (x == 0 || y == board->size || x == board->size || y == 0)
	{
		return 1;
	}

	return 0;
}

int board_count(Board *board, Cell type)
{
	int count = 0;
	int x, y;

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			if (board_check(board, x, y, type))
			{
				count++;
			}
		}
	}

	return count;
}

void board_copy(Board *target, Board *source)
{
	int x, y;

	for (y = 0; y < target->size; y++)
	{
		for (x = 0; x < target->size; x++)
		{
			if (source->size > x && source->size > y)
			{
				board_set(target, x, y, board_get(source, x, y));
			}
			else
			{
				board_set(target, x, y, ERROR);
			}
		}
	}

	debug(3, "Board copied!\n");
}

void board_init(Board *board, Bool random)
{
	int x, y, c;
	Cell type;

	debug(2, "Init a %d sized board...\n", board->size);

	for (y = 0; y < board->size; y++)
	{
		for (x = 0; x < board->size; x++)
		{
			board_set(board, x, y, BLANK);
		}
	}

	type = random && rand() % 2 ? MIN : MAX;
	c = board->size / 2;

	board_set(board, c - 1, c - 1, type);
	board_set(board, c - 1, c, minus(type));
	board_set(board, c, c - 1, minus(type));
	board_set(board, c, c, type);
}

void board_free(Board *board)
{
	free(board->cells);
	board->size = EOF;
}