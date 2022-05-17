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
    SPRITE_ID_PACMAN_DEAD1,
    SPRITE_ID_PACMAN_DEAD2,
    SPRITE_ID_PACMAN_DEAD3,
    SPRITE_ID_PACMAN_DEAD4,
    SPRITE_ID_PACMAN_DEAD5,
    SPRITE_ID_PACMAN_DEAD6,
    SPRITE_ID_PACMAN_DEAD7,
    SPRITE_ID_PACMAN_DEAD8,
    SPRITE_ID_PACMAN_DEAD9,
    SPRITE_ID_PACMAN_DEAD10,
    SPRITE_ID_PACMAN_DEAD11,
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
    SPRITE_ID_GHOST_FRIGHTENED1,
    SPRITE_ID_GHOST_FRIGHTENED2,
    SPRITE_ID_GHOST_EATEN_LEFT,
    SPRITE_ID_GHOST_EATEN_RIGHT,
    SPRITE_ID_GHOST_EATEN_UP,
    SPRITE_ID_GHOST_EATEN_DOWN,

    SPRITE_ID_COUNT
};

// Do not change the order of these.
// They are used in UpdateGhostAiSystem in the 'IsIntersection' part.
enum {
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_NONE
};

enum {
    STATE_CHASE,
    STATE_SCATTER,
    STATE_FRIGHTENED,
    STATE_EATEN
};

enum {
    GHOST_BLINKY,
    GHOST_PINKY,
    GHOST_INKY,
    GHOST_CLYDE,

    GHOST_COUNT
};

struct AnimationSystem {
    VertexArray vertex_arrays[SPRITE_ID_COUNT];
};

// No need to make a 'PlayerMovementComponent'.
// We just store the needed information here.
struct PlayerInputSystem {
    Input input;
    Entity pacman;
    u32 next_direction;
    u8 base_sprite_ids[4]; // 4, one for each direction
    Ghost *ghosts[GHOST_COUNT];
    bool is_dead;
};

struct GhostAiSystem {
    Entity pacman;
    Ghost ghosts[GHOST_COUNT];
};


void
UpdateAnimationSystem(World *world, AnimationSystem *system);

void
UpdateRenderSystem(World *world);

void
UpdateMovementSystem(World *world);

void
UpdatePlayerInputSystem(World *world, PlayerInputSystem *system);

void
UpdateGhostAiSystem(World *world, GhostAiSystem *system);

#endif // PACMAN_SYSTEMS_HPP
