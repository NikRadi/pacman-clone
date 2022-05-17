#include "Maze.hpp"


u8 maze[MAZE_HEIGHT][MAZE_WIDTH] = {
// These numbers just help identify the cell easier
//  1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W, // 0
    W,d,d,d,d,d,d,d,d,d,d,d,d,W,W,d,d,d,d,d,d,d,d,d,d,d,d,W, // 1
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W, // 2
    W,D,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,D,W, // 3
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W, // 4
    W,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,W, // 5
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W, // 6
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W, // 7
    W,d,d,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,d,d,W, // 8
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W, // 9
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W, // 0
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W, // 1
    W,W,W,W,W,W,d,W,W,E,W,W,W,E,E,W,W,W,E,W,W,d,W,W,W,W,W,W, // 2
    W,W,W,W,W,W,d,W,W,E,W,E,E,E,E,E,E,W,E,W,W,d,W,W,W,W,W,W, // 3
    E,E,E,E,E,E,d,E,E,E,W,E,E,E,E,E,E,W,E,E,E,d,E,E,E,E,E,E, // 4
    W,W,W,W,W,W,d,W,W,E,W,E,E,E,E,E,E,W,E,W,W,d,W,W,W,W,W,W, // 5
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


void
SetEmpty(Vector2Int cell) {
    maze[cell.y][cell.x] = E;
}

bool
IsCell(Vector2Int cell, u8 type) {
    return maze[cell.y][cell.x] == type;
}

bool
IsWall(Vector2Int cell) {
    return maze[cell.y][cell.x] == W;
}

bool
IsIntersection(Vector2Int cell) {
    return IS_INTERSECTION[cell.y][cell.x];
}
