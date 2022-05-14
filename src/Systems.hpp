#ifndef PACMAN_SYSTEMS_HPP
#define PACMAN_SYSTEMS_HPP
#include "Common.hpp"
#include "Math.hpp"
#include "Maze.hpp"
#include "Platform.hpp"
#include "World.hpp"


enum {
    SPRITE_ID_BIG_DOT1,
    SPRITE_ID_BIG_DOT2,
    SPRITE_ID_PACMAN_RIGHT1,
    SPRITE_ID_PACMAN_RIGHT2,
    SPRITE_ID_PACMAN_RIGHT3, // Also used when Pacman is standing still
    SPRITE_ID_PACMAN_LEFT1,
    SPRITE_ID_PACMAN_LEFT2,
    SPRITE_ID_PACMAN_LEFT3,
    SPRITE_ID_PACMAN_UP1,
    SPRITE_ID_PACMAN_UP2,
    SPRITE_ID_PACMAN_UP3,
    SPRITE_ID_PACMAN_DOWN1,
    SPRITE_ID_PACMAN_DOWN2,
    SPRITE_ID_PACMAN_DOWN3,
    SPRITE_ID_BLINKY_RIGHT1,
    SPRITE_ID_BLINKY_RIGHT2,
    SPRITE_ID_BLINKY_LEFT1,
    SPRITE_ID_BLINKY_LEFT2,
    SPRITE_ID_BLINKY_UP1,
    SPRITE_ID_BLINKY_UP2,
    SPRITE_ID_BLINKY_DOWN1,
    SPRITE_ID_BLINKY_DOWN2,
    SPRITE_ID_PINKY_RIGHT1,
    SPRITE_ID_PINKY_RIGHT2,
    SPRITE_ID_PINKY_LEFT1,
    SPRITE_ID_PINKY_LEFT2,
    SPRITE_ID_PINKY_UP1,
    SPRITE_ID_PINKY_UP2,
    SPRITE_ID_PINKY_DOWN1,
    SPRITE_ID_PINKY_DOWN2,
    SPRITE_ID_INKY_RIGHT1,
    SPRITE_ID_INKY_RIGHT2,
    SPRITE_ID_INKY_LEFT1,
    SPRITE_ID_INKY_LEFT2,
    SPRITE_ID_INKY_UP1,
    SPRITE_ID_INKY_UP2,
    SPRITE_ID_INKY_DOWN1,
    SPRITE_ID_INKY_DOWN2,
    SPRITE_ID_CLYDE_RIGHT1,
    SPRITE_ID_CLYDE_RIGHT2,
    SPRITE_ID_CLYDE_LEFT1,
    SPRITE_ID_CLYDE_LEFT2,
    SPRITE_ID_CLYDE_UP1,
    SPRITE_ID_CLYDE_UP2,
    SPRITE_ID_CLYDE_DOWN1,
    SPRITE_ID_CLYDE_DOWN2,

    SPRITE_ID_COUNT
};

enum {
    DIRECTION_NONE,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_UP
};

struct AnimationSystem {
    VertexArray vertex_arrays[SPRITE_ID_COUNT];
};


// No need to make a 'PacmanMovementComponent'.
// We just store the needed information here.
struct PacmanMovementSystem {
    Input input;
    Entity pacman;
    u32 current_direction;
    u32 next_direction;
};

enum {
    GHOST_BLINKY,
    GHOST_PINKY,
    GHOST_INKY,
    GHOST_CLYDE,

    GHOST_COUNT
};

enum {
    STATE_CHASE,
    STATE_SCATTER,
    STATE_FRIGHTENED,
    STATE_EATEN
};

// This is technically a component, but since
// we know there are always 4 ghosts we add 4
// Ghost components to the GhostMovementSystem
struct Ghost {
    Entity id;
    Vector2Int target_cell;
    u8 state;
    u8 has_reached_target_cell;
    u8 current_direction;

    // Each ghost scatters around different areas.
    // We define the area with 4 cells.
    u8 scatter_cell_idx;
    Vector2Int scatter_cells[4];
};

struct GhostMovementSystem {
    Entity pacman;
    Ghost ghosts[GHOST_COUNT];
};


void
UpdateAnimationSystem(World *world, AnimationSystem *system);

void
UpdateRenderSystem(World *world);

void
UpdatePacmanMovementSystem(World *world, PacmanMovementSystem *system);

void
UpdateGhostMovementSystem(World *world, GhostMovementSystem *system);

#endif // PACMAN_SYSTEMS_HPP
