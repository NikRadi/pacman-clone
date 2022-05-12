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