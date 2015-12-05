#include <time.h>
#include "game.h"
#include "tui.h"
#include "rucz.h"

/// \mainpage Reversi
/// Reversi realization in pure C.
///
/// \section user Introductions
///
/// \subsection user_new New
/// You can start a new game by pressing [1] and [Enter] in the main menu.
///
/// \subsection user_load Load
/// You can load a saved game by pressing [2] and [Enter] in the main menu.
/// Then you need to type the name of the save file into the terminal,
/// which will be loaded after you hit enter.
///
/// \subsection user_config Config
/// You can edit the game configuration by pressing [3] and [Enter] in the
/// main menu. After that, you can modify an option by typing its index and
/// the new value seperated by a space. To confirm your modification,
/// you need to press enter.
///
/// \subsection user_play Gameplay
/// When the game is started you need to make a move. To do this, you need
/// to type valid coords to the terminal seperated by a space. If you wish
/// to quit or save you need to press [q] or [s] and then [Enter].
///
/// \section test Tests
///
/// \subsection test_coords Coords
/// If invalid coords were given, the game will ask again for new ones.
///
/// \subsection test_config Configuration
/// The config page will not allow any invalid value to be set. However,
/// the configuration file can be manipulated manually. If the file
/// is missing or corrupted, the program will recreate it.
///
/// \subsection test_path Path
/// If the game receives a path that cannot be read or written, it will
/// fall back to the previous page.
///
/// \subsection test_load Load
/// If a save file is corrupted, but readable, the board will contain "E" 
/// cells.

int main(void)
{
	Game game;

	srand((unsigned int)time(NULL));

	game = game_new();
	game_import(&game, CONFIG);
	tui_start(&game);
	game_free(&game);

	return 0;
}