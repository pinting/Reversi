#ifndef TUI_H
#define TUI_H

#include <ctype.h>
#include "game.h"

/// Pause them after each AI move.
#define PAUSE 0

/// Show last move when dumping.
#define LAST_MOVE 1

/// Clear screen size.
#define CLEAR_SIZE 30

/// Show the main menu.
/// @param game Game structure.
void tui_start(Game *game);

#endif
