/**
 * \brief 
 */

#include <stdint.h>
#include "board.h"

static int move_dirs[8][2] = {
	{-1,-1},
	{-1, 0},
	{-1, 1},
	{ 0,-1},
	{ 0, 1},
	{ 1,-1},
	{ 1, 0},
	{ 1, 1},
};

size_t get_move_size(void)
{
	return sizeof(move_t);
}

size_t get_board_size(void)
{
	return sizeof(board_t);
}

/**
 * \brief initialize the board
 * \details
 * This function sets up the starting position on the board.
 * \param[in,out] brd pointer to board structure to be filled
 */
void init_board(board_t *brd)
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; ++i)
	{
		for (j = 0; j < BOARD_SIZE; ++j)
		{
			if ((i == BOARD_SIZE/2-1 || i == BOARD_SIZE/2) &&
			    (j == BOARD_SIZE/2-1 || j == BOARD_SIZE/2))
			{
				if ((i+j)%2 == 0)
					brd->position[i][j] = WHITE;
				else
					brd->position[i][j] = BLACK;
			}
			else
				brd->position[i][j] = EMPTY;
		}
	}
	/* Black starts */
	brd->turn = BLACK;
}


/**
 * \brief check if game has ended 
 * \param[in] brd the current position
 * \return 1 if game ended, 0 otherwise
 */
int is_game_ended(board_t *brd)
{
	int r,c,n = 0,b=0, w=0;
	for (r = 0; r < BOARD_SIZE; ++r)
		for (c = 0; c < BOARD_SIZE; ++c)
			switch (brd->position[r][c]) 
			{
				case BLACK:
					++b;
				case WHITE:
					++w;
					break;
				case EMPTY:
					++n;
					break;
			}
				
	/* Game ended, if board full or someone has lost all her pieces */
	return (n == 0 || b == 0 || w == 0);
}

game_result_t score_board(board_t *brd, int *b, int *w)
{
	int i, j;
	*b = *w = 0;
	for (i = 0; i < BOARD_SIZE; ++i)
	{
		for (j = 0; j < BOARD_SIZE; ++j)
		{
			switch (brd->position[i][j])
			{
				case BLACK: (*b)++; break;
				case WHITE: (*w)++; break;
				default: break;
			}
		}
	}
	if (*w == 0) return BLACK_WON;
	if (*b == 0) return WHITE_WON;
	if (*w + *b == BOARD_SIZE * BOARD_SIZE)
	{
		if (*w > *b) return WHITE_WON;
		if (*b > *w) return BLACK_WON;
		return GAME_DRAW;
	}
	return GAME_GOES_ON;
}

static inline int in_board(int r, int c)
{
	return (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE);
}

static inline field_t opponent(field_t f)
{
	return (f == BLACK) ? WHITE : BLACK; 
}


int check_move(board_t *brd, move_t *mv)
{
	field_t curr = brd->turn;
	field_t other = opponent(brd->turn);
	int i;
	
	if (mv->is_pass)
	{
		int r, c;
		for (r = 0; r < BOARD_SIZE; ++r)
		{
			for (c = 0; c < BOARD_SIZE; ++c)
			{
				move_t tmp_mov = {
					.is_pass = 0, 
					.row = r, 
					.col = c
				};
				if (!check_move(brd, &tmp_mov))
					return 1;
			}
		}
		/* Otherwise OK */
		return 0;
	}
	
	/* Check if position is empty */
	if (!(brd->position[mv->row][mv->col] == EMPTY))
		return 1;
	
	/* Check if move turns around pieces */
	/* Go through all directions */
	for (i = 0; i < 8; ++i)
	{
		int r = mv->row + move_dirs[i][0];
		int c = mv->col + move_dirs[i][1]; 
		int s = 0;
		while (in_board(r,c) && s >= 0)
		{
			switch (s) 
			{
				case 0:
					if (brd->position[r][c] == other)
						s = 1;
					else
						s = -1;
					break;
				case 1:
					if (brd->position[r][c] == EMPTY)
						s = -1;
					else if (brd->position[r][c] == curr)
						return 0;
					break;
			}
			r += move_dirs[i][0];
			c += move_dirs[i][1];
		}
	}
	return 1;
}


/**
 * \brief performs a move on the board
 * \param[in,out] brd current position
 * \param[in] mv move
 */
int make_move(board_t *brd, move_t *mv)
{
	field_t curr = brd->turn;
	field_t other = opponent(brd->turn);
	
	int i;
	int ret = 0;
	
	if (mv->is_pass)
	{
		brd->turn = other;
		return 0;
	}
	
	/* Go through all directions */
	for (i = 0; i < 8; ++i)
	{
		int r = mv->row + move_dirs[i][0];
		int c = mv->col + move_dirs[i][1]; 
		int s = 0;
		int n = 0;
		int j;
		while (in_board(r,c) && s >= 0 && s < 2)
		{
			switch (s) 
			{
				case 0:
					if (brd->position[r][c] == other)
					{
						s = 1;
						n = 1;
					}
					else
						s = -1;
					break;
				case 1:
					if (brd->position[r][c] == EMPTY)
						s = -1;
					else 
					{	
						if (brd->position[r][c] == curr)
							s = 2;
						else /* == other */
							n++;
					}
					break;
			}
			r += move_dirs[i][0];
			c += move_dirs[i][1];
		}
		
		r = mv->row + move_dirs[i][0];
		c = mv->col + move_dirs[i][1];
		
		/* Turn pieces */
		/** \todo check why s == 2 condition is needed */
		for (j = 0; j < n && s == 2; ++j)
		{
			brd->position[r][c] = curr;
			r += move_dirs[i][0];
			c += move_dirs[i][1];
		}
		ret += n;
	}
	brd->position[mv->row][mv->col] = curr;
	brd->turn = other;
	return ret;
}


int board_symmetry(board_t *brd)
{
	/* Check for all types of symmetry */
	int r, c, ok;
	int ret = 0;
	
	/* N-S symmetry */
	for (r = 0, ok = 1; r < BOARD_SIZE/2 && ok; ++r)
		for (c = 0; c < BOARD_SIZE && ok; ++c)
		{
			if (brd->position[r][c] != brd->position[BOARD_SIZE-1-r][c])
				ok = 0;
		}
	ret |= (BOARD_SYMM_FLAG_NS * ok);
	
	/* W-E symmetry */
	for (r = 0, ok = 1; r < BOARD_SIZE && ok; ++r)
		for (c = 0; c < BOARD_SIZE/2 && ok; ++c)
		{
			if (brd->position[r][c] != brd->position[r][BOARD_SIZE-1-c])
				ok = 0;
		}
	ret |= (BOARD_SYMM_FLAG_WE * ok);
	
	/* NW-SE symmetry */
	for (r = 0, ok = 1; r < BOARD_SIZE && ok; ++r)
		for (c = 0; c < BOARD_SIZE - r && ok; ++c)
		{
			if (brd->position[r][c] != brd->position[BOARD_SIZE-1-c][BOARD_SIZE-1-r])
				ok = 0;
		}
	ret |= (BOARD_SYMM_FLAG_NWSE * ok);
	
	/* NE-SW symmetry */
	for (r = 0, ok = 1; r < BOARD_SIZE && ok; ++r)
		for (c = 0; c < r && ok; ++c)
		{
			if (brd->position[r][c] != brd->position[c][r])
				ok = 0;
		}
	ret |= (BOARD_SYMM_FLAG_NESW * ok);
	
	return ret;
}
