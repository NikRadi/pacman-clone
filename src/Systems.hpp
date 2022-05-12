#ifndef PACMAN_SYSTEMS_HPP
#define PACMAN_SYSTEMS_HPP
#include "Common.hpp"
#include "Math.hpp"
#include "Platform.hpp"
#include "World.hpp"


enum {
    SPRITE_ID_BIG_DOT_1,
    SPRITE_ID_BIG_DOT_2,
    SPRITE_ID_PACMAN_RIGHT_1,
    SPRITE_ID_PACMAN_RIGHT_2,
    SPRITE_ID_PACMAN_RIGHT_3,
    SPRITE_ID_PACMAN_LEFT_1,
    SPRITE_ID_PACMAN_LEFT_2,
    SPRITE_ID_PACMAN_LEFT_3,
    SPRITE_ID_PACMAN_UP_1,
    SPRITE_ID_PACMAN_UP_2,
    SPRITE_ID_PACMAN_UP_3,
    SPRITE_ID_PACMAN_DOWN_1,
    SPRITE_ID_PACMAN_DOWN_2,
    SPRITE_ID_PACMAN_DOWN_3,

    SPRITE_ID_COUNT
};

enum {
    DIRECTION_NONE,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_UP
};

enum {
    d, // Small dot
    D, // Big dot
    E, // Empty
    W, // Wall
};

struct AnimationSystem {
    f32 delta_time;
    VertexArray vertex_arrays[SPRITE_ID_COUNT];
};

struct RenderSystem {
    s32 window_height;
};

struct PacmanSystem {
    Input input;
    Entity pacman;
    u32 current_direction;
    u32 next_direction;
    f32 delta_time;
    Vector2 cell_size;
    Vector2 half_cell_size;
};


constexpr static s32 MAZE_WIDTH = 28;
constexpr static s32 MAZE_HEIGHT = 31;
constexpr static u8 MAZE[MAZE_HEIGHT][MAZE_WIDTH] = {
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
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    E,E,E,E,E,E,d,E,E,E,E,E,E,E,E,E,E,E,E,E,E,d,E,E,E,E,E,E,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
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
UpdateAnimationSystem(World *world, AnimationSystem *system);

void
UpdateRenderSystem(World *world, RenderSystem *system);

void
UpdatePacmanSystem(World *world, PacmanSystem *system);

#endif // PACMAN_SYSTEMS_HPP
