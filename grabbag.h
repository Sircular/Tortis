/* The random grab bag that is used to choose the next pieces. Works by
 * repeating all the enums n times, shuffling them, and then traversing them in
 * order. */

#ifndef TETRIS_GRABBAG
#define TETRIS_GRABBAG

#include "piece.h"

typedef struct GrabBag {
    enum PieceType* contents;
    size_t length;
    size_t current;
} GrabBag;

/* Initialize a grab bag with n repetitions. */
GrabBag* grabbag_init(size_t n);

/* Free a grab bag. */
void grabbag_free(GrabBag* bag);

/* Get the next piece type from a grab bag. */
void grabbag_next(GrabBag* bag, enum PieceType* type);
#endif
