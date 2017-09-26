#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "grabbag.h"
#include "piece.h"

/* Shuffles the contents of the grab bag. Used whenever a grab bag is
 * initialized the end of the shuffled list is reached. */
void shuffle(GrabBag* bag);

GrabBag* grabbag_init(size_t n) {
    GrabBag* bag = malloc(sizeof(GrabBag));
    if (bag == NULL) {
        return NULL;
    }
    bag->contents = malloc(sizeof(enum PieceType)*n*PIECE_TYPES_COUNT);
    if (bag->contents == NULL) {
        free(bag);
        return NULL;
    }

    // initialize the random number generator for later use
    srand((unsigned int)time(NULL));

    bag->length = PIECE_TYPES_COUNT*n;
    bag->current = 0;

    size_t i;
    for (i = 0; i < n; i++) {
        size_t j;
        for (j = L_L_PIECE; j <= LONG_PIECE; j++) {
            size_t index = (i*PIECE_TYPES_COUNT) + (j-1);
            bag->contents[index] = (enum PieceType)j;
        }
    }
    shuffle(bag);
    return bag;
}

void grabbag_next(GrabBag* bag, enum PieceType* type) {
    if (bag == NULL || bag->contents == NULL) {
        return;
    }
    if (bag->current >= bag->length) {
        shuffle(bag);
        bag->current = 0;
    }
    *type = bag->contents[bag->current];
    bag->current++;
}

void shuffle(GrabBag* bag) {
    if (bag == NULL || bag->contents == NULL) {
        return;
    }
    // Fisher-Yates shuffle
    size_t i;
    for (i = 0; i < bag->length-1; i++) {
        size_t maxRange = bag->length-(i+1);
        size_t swapI = i + ((size_t)rand())%maxRange;
        if (i != swapI) {
            enum PieceType temp = bag->contents[i];
            bag->contents[i] = bag->contents[swapI];
            bag->contents[swapI] = temp;
        }
    }
}
