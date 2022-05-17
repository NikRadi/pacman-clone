#include "Maze.hpp"


bool
IsWall(Vector2Int cell) {
    return MAZE[cell.y][cell.x] == W;
}

bool
IsIntersection(Vector2Int cell) {
    return IS_INTERSECTION[cell.y][cell.x];
}
