# Tortis

Tortis is a simple clone of Tetris written in C. It copies basic mechanics such
as scoring, but modifies timing, difficulty levels etc. based on how well I play
Tetris (i.e. not well.)

## Controls

WASD keys move the piece (W rotates). Use Q for quick drop. Use enter to choose
the active menu item. P pauses the game. R toggles whether a projection of where
the piece will be is shown.

## Installation

There isn't an option to install it system-wide at the moment. To compile it,
simply run `make`. Then you can run it by running `./tortis`.

## Known Issues

Rotation doesn't work like in the original Tetris games, so several strategies
involving rotating a piece into place don't currently work. Saving a block is
not implemented. Needs more information on controls.
