#ifndef PACMAN_WORLD_HPP
#define PACMAN_WORLD_HPP
#include "Common.hpp"
#include "Components.hpp"


constexpr u32 MAX_ENTITIES = 256;


struct Entity {
    u32 id;
};

struct World {
    u32 entity_masks[MAX_ENTITIES];
    Transform transforms[MAX_ENTITIES];
    Sprite sprites[MAX_ENTITIES];
    Animation animations[MAX_ENTITIES];
};


Entity
CreateEntity(World *world);

Entity
MakeGhost(World *world, Transform transform, Sprite sprite);

#endif // PACMAN_WORLD_HPP
