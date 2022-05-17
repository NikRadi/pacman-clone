#ifndef PACMAN_COMPONENTS_HPP
#define PACMAN_COMPONENTS_HPP
#include "Math.hpp"
#include "OpenGL.hpp"


enum {
    MASK_NONE       = 0,
    MASK_TRANSFORM  = 1 << 0,
    MASK_SPRITE     = 1 << 1,
    MASK_ANIMATION  = 1 << 2,
    MASK_MOTION     = 1 << 3,
};

struct Transform {
    Vector2 scale;
    Vector2 translate;
};

struct Sprite {
    Texture2D texture;
    VertexArray vertex_array;
};

struct Animation {
    u8 base_sprite_id;
    u8 current_sprite_id;
    u8 num_frames;

    // Only used when num_frames > 2. If we have 3 sprites we should
    // animate sprites 1, 2, 3, 2, 1, 2 and not 1, 2, 3, 1, 2, 3
    u8 is_reversed;
    bool is_looped;
    bool is_finished;
    f32 seconds_since_last_frame;
    f32 seconds_between_frames;
};

struct Motion {
    f32 speed;
    u32 direction;
};

// This component is not added to the World struct,
// but only to the GhostAiSystem
struct Ghost {
    u32 id;
    u32 state;
    u32 is_state_init;
    f32 seconds_in_current_state;
    u8 base_sprite_ids_for_direction[4]; // 4, one for each direction // 4, one for each direction
    Vector2Int target_cell;
    Vector2Int scatter_target_cell;
    Vector2Int last_intersection_cell;
};

#endif // PACMAN_COMPONENTS_HPP
