#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "console_interface.h"

static char field_chars[] = {' ', 'X', 'O'};
static char* field_names[] = {
	"Empty", "X", "O"};

static char linebuf[256];

void print_symmetry(int s)
{
	if (s & BOARD_SYMM_FLAG_NS) printf("NS ");
	if (s & BOARD_SYMM_FLAG_WE) printf("WE ");
	if (s & BOARD_SYMM_FLAG_NWSE) printf("NWSE ");
	if (s & BOARD_SYMM_FLAG_NESW) printf("NESW ");
}

void print_score(int b, int w)
{
	printf("Score: %s %d - %s %d\n", 
		field_names[BLACK], b, field_names[WHITE], w);
}

void print_game_result(game_result_t result)
{
	printf("Game result: ");
	switch (result)
	{
		case BLACK_WON:
			printf("%s has won!", field_names[BLACK]);
			break;
		case WHITE_WON:
			printf("%s has won!", field_names[WHITE]);
			break;
		case GAME_DRAW:
			printf("the game ended in a draw.");
			break;
		default:
			break;
	}
	printf("\n");
}

void print_board(board_t *brd)
{
	int i, j;
	int b, w;
	int s;
	
	printf("   |");
	for (j = 0; j < BOARD_SIZE; ++j)
		printf(" %c |", 'A' + j);
	printf("\n");
	for (i = 0; i < BOARD_SIZE; ++i)
	{
		printf(" %d |", i+1);
		for (j = 0; j < BOARD_SIZE; ++j)
			printf(" %c |", field_chars[brd->position[i][j]]);
		printf("\n");
	}
	score_board(brd, &b, &w);
	printf("%s's turn, score: %s %d - %s %d\n", 
		field_names[brd->turn], field_names[BLACK], b, field_names[WHITE], w);
	s = board_symmetry(brd);
	printf("Symmetry: %d, ", s); print_symmetry(s); printf("\n");
}

/**
 * \brief case insensitive string comparison
 * \return 1 if the two strings are the same
 */
int strcmp_ci(char *a, char *b)
{
	for (;*a && *b; ++a, ++b) {
		int d = tolower(*a) - tolower(*b);
		if (d != 0)
			return 0;
	}
	if (*a || *b) 
		return 0;
	else 
		return 1;
}

char *strstrip(char *s)
{
	size_t size;
	char *end;

	size = strlen(s);

	if (!size)
		return s;

	end = s + size - 1;
	while (end >= s && isspace(*end))
		end--;
	*(end + 1) = '\0';

	while (*s && isspace(*s))
		s++;

	return s;
}

void print_move(move_t * mv)
{
	if (mv->is_pass)
		printf("PASS");
	else
		printf("%c %d", mv->col+'A', mv->row+1);
}





int read_move(move_t *mv)
{
	char *start;

	fgets(linebuf, sizeof(linebuf), stdin);
	
	start = strstrip(linebuf);
	
	if (strcmp_ci(start, "PASS"))
	{
		mv->is_pass = 1;
		return 0;
	}
	else
	{
		char c;
		int r;
		mv->is_pass = 0;
		sscanf(start, "%c%d", &c, &r);
		c = toupper(c);
		if (c < 'A' || c >= 'A' + BOARD_SIZE)
		{
			printf("Invalid move read: %s\n", start);
			return -1;
		}
		mv->col = c - 'A';
		if (r < 1 || r > BOARD_SIZE)
		{
			printf("Invalid move read: %s\n", start);
			return -1;
		}
		mv->row = r - 1;
	}
	return 0;
}