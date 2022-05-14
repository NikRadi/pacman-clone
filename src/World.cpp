#include "World.hpp"


Entity
CreateEntity(World *world) {
    for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        if (world->entity_masks[i] == MASK_NONE) {
            return { i };
        }
    }

    NOT_IMPLEMENTED;
    return { MAX_ENTITIES };
}

Entity
CreateGhost(World *world, Transform transform, Sprite sprite, u8 sprite_id) {
    Entity ghost = CreateEntity(world);
    world->entity_masks[ghost] = MASK_TRANSFORM | MASK_SPRITE | MASK_ANIMATION;
    world->transforms[ghost] = transform;
    world->sprites[ghost] = sprite;

    Animation *animation = &world->animations[ghost];
    animation->base_sprite_id = sprite_id;
    animation->num_frames = 2;
    animation->seconds_between_frames = 0.15f;
    return ghost;
}
