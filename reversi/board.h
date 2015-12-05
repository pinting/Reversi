#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/// Debug mode switch.
#define DEBUG 0

/// Debug macro - a simple printf.
#define debug(n, fmt, ...) if (DEBUG >= n) { printf("DEBUG: "); printf(fmt, __VA_ARGS__); }

/// Macro for negation.
#define minus(t) -1 * t

/// Bool enum.
typedef enum Bool
{
	/// True, blabla...
	FALSE = 0,
	/// And yeah, false.
	TRUE = 1
} Bool;

/// Cell enum which is used as the data of 
/// the Board struct.
typedef enum Cell
{
	/// Min type - O in the TUI.
	MIN = -1,
	/// Max type - X in the TUI.
	MAX = 1,
	/// Blank.
	BLANK = 0,
	/// Error type if something has gone terribly wrong.
	ERROR = 2
} Cell;

/// The Board structure which stores the size and
/// and the content of the board.
typedef struct Board
{
	/// Size of the board.
	int size;
	/// 1D array with Cells.
	Cell *cells;
} Board;

/// Create a new board.
/// @param size Size of the board.
Board board_new(int size);

/// Load board from a file.
/// @param path Path of the saved board.
Board board_load(char *path);

/// Get the value of a cell.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
Cell board_get(Board *board, int x, int y);

/// Check the type of a cell.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
/// @param type Cell type.
Bool board_check(Board *board, int x, int y, Cell type);

/// Set a type for a cell.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
/// @param type Cell type.
Bool board_set(Board *board, int x, int y, Cell type);

/// Check if the given coords are inside the board.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
Bool board_inside(Board *board, int x, int y);

/// Save board to a file.
/// @param board Board structure.
/// @param path Path of the save file.
Bool board_save(Board *board, char *path);

/// Make a move on the board with the given type and coordinates. If the test parameter is true,
/// the move will not be made - just tested.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
/// @param type Cell type.
/// @param test Just test the move value?
int board_move(Board *board, int x, int y, Cell type, Bool test);

/// Count the number of moves left with the given type.
/// @param board Board structure.
/// @param type Cell type.
int board_moves_left(Board *board, Cell type);

/// Check if cell is in the corner.
/// @param board Board structure.
/// @param x X coord.
/// @param y Y coord.
int board_corner(Board *board, int x, int y);

/// Count the number of types on the board.
/// @param board Board structure.
/// @param type Cell type.
int board_count(Board *board, Cell type);

/// Copy the board from one array to another.
/// @param target To Board structure.
/// @param source From Board structure.
void board_copy(Board *target, Board *source);

/// Init a board.
/// @param board Board structure.
/// @param random Enable random starting positions.
void board_init(Board *board, Bool random);

/// Free up a board.
/// @param board Board structure.
void board_free(Board *board);

#endif