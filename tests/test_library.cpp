#include <gtest/gtest.h>
#include "library.h"

TEST(DirectionToVectorTest, BasicAssertions) {
    Vec2i north = direction_to_vector(NORTH);
    EXPECT_EQ(north.x, 0);
    EXPECT_EQ(north.y, -1);

    Vec2i east = direction_to_vector(EAST);
    EXPECT_EQ(east.x, 1);
    EXPECT_EQ(east.y, 0);

    Vec2i south = direction_to_vector(SOUTH);
    EXPECT_EQ(south.x, 0);
    EXPECT_EQ(south.y, 1);

    Vec2i west = direction_to_vector(WEST);
    EXPECT_EQ(west.x, -1);
    EXPECT_EQ(west.y, 0);
}







// #pragma once

// typedef struct{
//     int x;
//     int y;
// } Vec2i;

// typedef enum {
//     NORTH, EAST, WEST, SOUTH
// } Direction; 

// /**
//  * @brief Transform a direction to a vector
//  */
// Vec2i direction_to_vector(Direction dir)
// {
//     switch(dir) {
//         case NORTH:
//             return (Vec2i){0, -1};
//         case EAST:
//             return (Vec2i){1, 0};
//         case SOUTH:
//             return (Vec2i){0, 1};
//         case WEST:
//             return (Vec2i){-1, 0};
//         default:
//             return (Vec2i){0, 0}; // Should never happen
//     }
// }