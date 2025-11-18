#include "library.h"

Vec2i direction_to_vector(Direction dir)
{
    switch(dir) {
        case NORTH:
            return (Vec2i){0, -1};
        case EAST:
            return (Vec2i){1, 0};
        case SOUTH:
            return (Vec2i){0, 1};
        case WEST:
            return (Vec2i){-1, 0};
        default:
            return (Vec2i){0, 0}; // Should never happen
    }
}


/*

{
    Vec2i vec = {0, 0};
    switch (dir)
    {
    case DIR_UP:
        vec.y = -1;
        break;
    case DIR_DOWN:
        vec.y = 1;
        break;
    case DIR_LEFT:
        vec.x = -1;
        break;
    case DIR_RIGHT:
        vec.x = 1;
        break;
    default:
        break;
    }
    return vec;
}

{
    switch(dir) {
        case NORTH:
            return (Vec2i){0, -1};
        case EAST:
            return (Vec2i){1, 0};
        case SOUTH:
            return (Vec2i){0, 1};
        case WEST:
            return (Vec2i){-1, 0};
        default:
            return (Vec2i){0, 0}; // Should never happen
    }
}
*/