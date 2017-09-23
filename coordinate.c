#include "coordinate.h"

Coordinate coordinate_add(Coordinate a, Coordinate b) {
    Coordinate c = COORDINATE(a.x+b.x, a.y+b.y);
    return c;
}
