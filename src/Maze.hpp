#ifndef PACMAN_MAZE_HPP
#define PACMAN_MAZE_HPP


enum {
    d, // Small dot
    D, // Big dot
    E, // Empty
    W, // Wall
};


constexpr static s32 MAZE_WIDTH = 28;
constexpr static s32 MAZE_HEIGHT = 31;
constexpr static u8 MAZE[MAZE_HEIGHT][MAZE_WIDTH] = {
// These numbers just help identify the cell easier
//  1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,W,W,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,D,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,D,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W,
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W,
    W,d,d,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,d,d,W,
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,E,E,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,E,E,E,E,E,E,W,E,W,W,d,W,W,W,W,W,W,
    E,E,E,E,E,E,d,E,E,E,W,E,E,E,E,E,E,W,E,E,E,d,E,E,E,E,E,E,
    W,W,W,W,W,W,d,W,W,E,W,E,E,E,E,E,E,W,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,W,W,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,W,W,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,W,W,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,W,W,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,D,d,d,W,W,d,d,d,d,d,d,d,E,E,d,d,d,d,d,d,d,W,W,d,d,D,W,
    W,W,W,d,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,d,W,W,W,
    W,W,W,d,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,d,W,W,W,
    W,d,d,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,d,d,W,
    W,d,W,W,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,W,W,d,W,
    W,d,W,W,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,W,W,d,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W
};

// Used for ghost movement
constexpr static u8 IS_INTERSECTION[MAZE_HEIGHT][MAZE_WIDTH] = {
// These numbers just help identify the cell easier
//  1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#endif // PACMAN_MAZE_HPP
