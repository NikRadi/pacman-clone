#ifndef PACMAN_WORLD_HPP
#define PACMAN_WORLD_HPP
#include "Common.hpp"
#include "Components.hpp"


constexpr u32 MAX_ENTITIES = 256;

typedef u32 Entity;


struct World {
    // We store these values here instead of in the
    // systems that might need them.
    Vector2 cell_size;
    Vector2 half_cell_size;
    Vector2Int window_size;
    f32 delta_time;

    u32 entity_masks[MAX_ENTITIES];
    Transform transforms[MAX_ENTITIES];
    Sprite sprites[MAX_ENTITIES];
    Animation animations[MAX_ENTITIES];
    Motion motions[MAX_ENTITIES];
};


Entity
CreateEntity(World *world);

Entity
CreateGhost(World *world, Transform transform, Sprite sprite, u8 sprite_id);

#endif // PACMAN_WORLD_HPP
