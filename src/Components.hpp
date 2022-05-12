#ifndef PACMAN_COMPONENTS_HPP
#define PACMAN_COMPONENTS_HPP
#include "Math.hpp"
#include "OpenGL.hpp"


enum {
    MASK_NONE       = 0,
    MASK_TRANSFORM  = 1 << 0,
    MASK_SPRITE     = 1 << 1,
    MASK_ANIMATION  = 1 << 2,
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
    f32 seconds_since_last_frame;
    f32 seconds_between_frames;
};

#endif // PACMAN_COMPONENTS_HPP
