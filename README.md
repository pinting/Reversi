# Reversi

This is the first stage of a university project I worked on between 2015 and 2016.

1. The game was created in C. Peter Rucz created his own generic board game AI implementation using min-max cut algorithm which is integrated into the project. The two AIs can play against each other. (Can be found in the Reversi repository.)
2. It was ported to Java. (Can be found under the `master` branch in the Jeversi repository.)
3. The Java logic was used in part of an Android application. (Can be found under the `android-port` branch in the Jeversi repository.)

## Rules

The game was originally constructed on a 8 x 8 board, however, the same rules can be applied to any N x N board if N is even and greater than 2. The cells have 3 states - being black (symbolized with an X), being white (symbolized with an O) and being empty (symbolized with a dot). In the first round everything but the inner 2x2 is empty. This center location is initialized with even number of black and white cells.

```
. . . .                . . . .
. O X .                . X O .
. X O .       or       . O X .
. . . .                . . . .
```

Players take turns filling cells on the board with their assigned color. During a play, any cells of the opponent's color that are in a straight line and bounded by the cell which was just filled to the current player's color and another cell of the current player's color are turned over to the current player's color.

```
X . . .               X . . .
. O O .               . X O .
. O X .  turns into   . O X .
. . . .               . . . .
```


The object of the game is to have the majority of the cells filled to display your color when the last playable empty square is filled.

## License

Copyright (c) 2015 Tornyi Dénes. Licensed under the MIT license.