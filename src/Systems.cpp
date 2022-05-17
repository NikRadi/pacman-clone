#include "Systems.hpp"
#include "OpenGL.hpp"


static Vector2Int
ToCellCoordinates(Vector2 translate, Vector2 cell_size) {
    Vector2Int cell;
    cell.x = static_cast<s32>(translate.x / cell_size.x);
    cell.y = static_cast<s32>(translate.y / cell_size.y);
    return cell;
}

static Vector2Int
Move(Vector2Int cell, u32 direction) {
    switch (direction) {
        case DIRECTION_LEFT:  { cell.x -= 1; } break;
        case DIRECTION_RIGHT: { cell.x += 1; } break;
        case DIRECTION_DOWN:  { cell.y += 1; } break;
        case DIRECTION_UP:    { cell.y -= 1; } break;
    }

    return cell;
}

// When comparing two distances, there is no need to use sqrt
static u32
EuclideanDistanceSquared(Vector2Int cell1, Vector2Int cell2) {
    Vector2Int difference = cell1 - cell2;
    return difference.x * difference.x + difference.y * difference.y;
}

static u32
ReverseDirection(u32 direction) {
    switch (direction) {
        case DIRECTION_LEFT:  return DIRECTION_RIGHT;
        case DIRECTION_RIGHT: return DIRECTION_LEFT;
        case DIRECTION_DOWN:  return DIRECTION_UP;
        case DIRECTION_UP:    return DIRECTION_DOWN;
    }

    ASSERT(false);
    return 0;
}

static bool
AreAlmostEquals(f32 a, f32 b) {
    constexpr f32 MAX_DIFFERENCE = 2.0f;
    return Abs(a - b) <= MAX_DIFFERENCE;
}

static bool
AreAlmostEquals(Vector2 a, Vector2 b) {
    return AreAlmostEquals(a.x, b.x) && AreAlmostEquals(a.y, b.y);
}

static bool
IsHorizontal(u32 direction) {
    return direction == DIRECTION_RIGHT || direction == DIRECTION_LEFT;
}

static bool
IsVertical(u32 direction) {
    return direction == DIRECTION_DOWN || direction == DIRECTION_UP;
}

void
UpdateAnimationSystem(World *world, AnimationSystem *system) {
    constexpr u32 MASK = MASK_SPRITE | MASK_ANIMATION;
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        if ((world->entity_masks[entity] & MASK) != MASK) {
            continue;
        }

        Animation *animation = &world->animations[entity];
        if (animation->is_finished) {
            continue;
        }

        animation->seconds_since_last_frame += world->delta_time;
        if (animation->seconds_between_frames <= animation->seconds_since_last_frame) {
            animation->seconds_since_last_frame = 0.0f;
            Sprite *sprite  = &world->sprites[entity];

            sprite->vertex_array = system->vertex_arrays[animation->base_sprite_id + animation->current_sprite_id];
            if (!animation->is_looped && animation->is_reversed) {
                animation->is_finished = true;
            }
            else if (animation->is_reversed) {
                animation->current_sprite_id -= 1;
                animation->is_reversed = animation->current_sprite_id != 0;
            }
            else {
                animation->current_sprite_id += 1;
                animation->is_reversed = animation->current_sprite_id == animation->num_frames - 1;
            }
        }
    }
}

void
UpdateRenderSystem(World *world) {
    constexpr u32 MASK = MASK_TRANSFORM | MASK_SPRITE;
    glClear(GL_COLOR_BUFFER_BIT);
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        if ((world->entity_masks[entity] & MASK) != MASK) {
            continue;
        }

        Transform *transform = &world->transforms[entity];
        Sprite *sprite = &world->sprites[entity];

        glBindTexture(GL_TEXTURE_2D, sprite->texture.handle);
        glBindVertexArray(sprite->vertex_array.id);

        // We want (0, 0) to be the top left corner
        Vector2 translate;
        translate.x = transform->translate.x;
        translate.y = world->window_size.y - transform->translate.y;

        Matrix4 model = IDENDITY_MATRIX4;
        model = Scale(model, transform->scale);
        model = Translate(model, translate);
        SetMatrix4Uniform("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void
UpdateMovementSystem(World *world) {
    constexpr u32 MASK = MASK_TRANSFORM | MASK_MOTION;
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        if ((world->entity_masks[entity] & MASK) != MASK) {
            continue;
        }

        Transform *transform = &world->transforms[entity];
        Motion *motion = &world->motions[entity];

        f32 speed = motion->speed * world->delta_time;
        switch (motion->direction) {
            case DIRECTION_LEFT:  { transform->translate.x -= speed; } break;
            case DIRECTION_RIGHT: { transform->translate.x += speed; } break;
            case DIRECTION_DOWN:  { transform->translate.y += speed; } break;
            case DIRECTION_UP:    { transform->translate.y -= speed; } break;
        }
    }
}

void
UpdatePlayerInputSystem(World *world, PlayerInputSystem *system) {
    if (system->is_dead) {
        return;
    }

    // Register which direction the player wants to move in next
    switch (system->input.last_pressed_key) {
        case KEY_A: { system->next_direction = DIRECTION_LEFT;  } break;
        case KEY_D: { system->next_direction = DIRECTION_RIGHT; } break;
        case KEY_S: { system->next_direction = DIRECTION_DOWN;  } break;
        case KEY_W: { system->next_direction = DIRECTION_UP;    } break;
    }

    Animation *animation = &world->animations[system->pacman];
    Motion *motion = &world->motions[system->pacman];

    // This if-statement is required so that the player can move in a new
    // direction instantly if the new direction is on the same line as
    // the current direction. Basically, the controls feel more smooth.
    if ((IsHorizontal(system->next_direction) && IsHorizontal(motion->direction)) ||
        (IsVertical(system->next_direction) && IsVertical(motion->direction))) {
        motion->direction = system->next_direction;
        animation->base_sprite_id = system->base_sprite_ids[motion->direction];
        world->entity_masks[system->pacman] |= MASK_ANIMATION;
    }

    Vector2 translate = world->transforms[system->pacman].translate;
    Vector2Int cell = ToCellCoordinates(translate, world->cell_size);
    Vector2 cell_center = cell * world->cell_size + world->half_cell_size;

    // The left hand side of the || operator is only true when the game starts
    // because Pacman is located in the middle of two cells. If it is removed
    // then the player cannot move when the game starts.
    if (AreAlmostEquals(translate, cell_center) || (motion->direction == DIRECTION_NONE && system->next_direction != DIRECTION_NONE)) {
        Vector2Int possible_next_cell = Move(cell, system->next_direction);
        if (!IsWall(possible_next_cell)) {
            motion->direction = system->next_direction;
            animation->base_sprite_id = system->base_sprite_ids[motion->direction];
            world->entity_masks[system->pacman] |= MASK_ANIMATION;
        }
        else {
            Vector2Int next_cell = Move(cell, motion->direction);
            if (IsWall(next_cell)) {
                motion->direction = DIRECTION_NONE;
                world->entity_masks[system->pacman] &= ~MASK_ANIMATION;
            }
        }
    }

    u8 cell_content = MAZE[cell.y][cell.x];
    if (cell_content == d || cell_content == D) {
        for (u32 entity = 0; entity < MAX_ENTITIES; ++entity) {
            Vector2 entity_translate = world->transforms[entity].translate;
            Vector2Int entity_cell = ToCellCoordinates(entity_translate, world->cell_size);
            // TODO: Ensure that this entity is a dot. We might remove a ghost or pacman
            if (entity_cell == cell) {
                world->entity_masks[entity] = MASK_NONE;
                break;
            }
        }

        if (cell_content == D) {
            for (u32 i = 0; i < GHOST_COUNT; ++i) {
                system->ghosts[i]->state = STATE_FRIGHTENED;
            }
        }
    }

    for (u32 i = 0; i < GHOST_COUNT; ++i) {
        Ghost *ghost = system->ghosts[i];
        Vector2 ghost_translate = world->transforms[ghost->id].translate;
        Vector2Int ghost_cell = ToCellCoordinates(ghost_translate, world->cell_size);
        if (ghost_cell == cell) {
            if (ghost->state == STATE_FRIGHTENED) {
                ghost->state = STATE_EATEN;
            }
            else {
                animation->base_sprite_id = SPRITE_ID_PACMAN_DEAD1;
                animation->num_frames = 11;
                animation->is_looped = false;
                animation->is_reversed = false;
                animation->seconds_between_frames = 0.1f;
                system->is_dead = true;
                world->entity_masks[system->pacman] &= ~MASK_MOTION;
                for (u32 j = 0; j < GHOST_COUNT; ++j) {
                    world->entity_masks[system->ghosts[j]->id] &= ~MASK_TRANSFORM;
                }
            }
        }
    }
}

void
UpdateGhostAiSystem(World *world, GhostAiSystem *system) {
    constexpr u32 MASK = MASK_TRANSFORM;
    for (u32 i = 0; i < GHOST_COUNT; ++i) {
        Ghost *ghost = &system->ghosts[i];
        if ((world->entity_masks[ghost->id] & MASK) != MASK) {
            continue;
        }

        ghost->seconds_in_current_state += world->delta_time;
        switch (ghost->state) {
            case STATE_CHASE: {
                Vector2 pacman_translate = world->transforms[system->pacman].translate;
                ghost->target_cell = ToCellCoordinates(pacman_translate, world->cell_size);
                if (ghost->seconds_in_current_state >= 20.0f) {
                    ghost->state = STATE_SCATTER;
                    ghost->is_state_init = false;
                }
            } break;
            case STATE_SCATTER: {
                if (!ghost->is_state_init) {
                    ghost->is_state_init = true;
                    ghost->target_cell = ghost->scatter_target_cell;
                }
                
                if (ghost->seconds_in_current_state >= 7.0f) {
                    ghost->state = STATE_CHASE;
                    ghost->is_state_init = false;
                }
            } break;
            case STATE_FRIGHTENED: {

            } break;
            case STATE_EATEN: {

            } break;
        }

        Motion *motion = &world->motions[ghost->id];

        Vector2 translate = world->transforms[ghost->id].translate;
        Vector2Int cell = ToCellCoordinates(translate, world->cell_size);
        Vector2 cell_center = cell * world->cell_size + world->half_cell_size;
        // The left hand side of the || operator is only true when a ghost is
        // moving out of the game. It is required since they are not in the
        // center of the cell but in between two cells.
        if (AreAlmostEquals(translate, cell_center) || (IsVertical(motion->direction) && AreAlmostEquals(translate.y, cell_center.y))) {
            Vector2Int next_cell = Move(cell, motion->direction);
            u32 next_direction = DIRECTION_NONE;
            if (IsWall(next_cell)) {
                if (IsHorizontal(motion->direction)) {
                    Vector2Int cell_up = Move(cell, DIRECTION_UP);
                    next_direction = (IsWall(cell_up)) ? DIRECTION_DOWN : DIRECTION_UP;
                }
                else {
                    Vector2Int cell_left = Move(cell, DIRECTION_LEFT);
                    next_direction = (IsWall(cell_left)) ? DIRECTION_RIGHT : DIRECTION_LEFT;
                }
            }

            if (IsIntersection(cell) && cell != ghost->last_intersection_cell) {
                ghost->last_intersection_cell = cell;
                u32 best_distance = 9999;
                u32 best_direction = DIRECTION_NONE;
                u32 reverse_direction = ReverseDirection(motion->direction);
                u32 distance;
                for (u32 direction = DIRECTION_UP; direction <= DIRECTION_RIGHT; ++direction) {
                    Vector2Int possible_next_cell = Move(cell, direction);
                    distance = EuclideanDistanceSquared(possible_next_cell, ghost->target_cell);
                    if (distance < best_distance && !IsWall(possible_next_cell) && direction != reverse_direction) {
                        best_distance = distance;
                        best_direction = direction;
                    }
                }

                next_direction = best_direction;
            }

            if (next_direction != DIRECTION_NONE) {
                motion->direction = next_direction;
                Animation *animation = &world->animations[ghost->id];
                animation->base_sprite_id = ghost->base_sprite_ids_for_direction[next_direction];
            }
        }
    }
}
