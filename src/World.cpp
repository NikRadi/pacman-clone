#include "World.hpp"


Entity
CreateEntity(World *world) {
    for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        if (world->entity_masks[i] == MASK_NONE) {
            return { i };
        }
    }

    INVALID_CODE_PATH;
    return { MAX_ENTITIES };
}

void
CreateGhost(World *world, Transform transform, Sprite sprite) {
    Entity ghost = CreateEntity(world);
    world->entity_masks[ghost.id] = MASK_TRANSFORM | MASK_SPRITE;
    world->transforms[ghost.id] = transform;
    world->sprites[ghost.id] = sprite;
}
