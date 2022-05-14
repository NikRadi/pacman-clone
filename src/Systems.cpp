#include "Systems.hpp"
#include "OpenGL.hpp"


static Vector2Int
ToMazeCoordinates(Vector2 translate, Vector2 cell_size) {
    Vector2Int maze_coordinates;
    maze_coordinates.x = static_cast<s32>(translate.x / cell_size.x);
    maze_coordinates.y = static_cast<s32>(translate.y / cell_size.y);
    return maze_coordinates;
}

// When comparing two distances, there is no need to use sqrt
// because if sqrt(x) > sqrt(y) then x > y
static u32
SquaredDistanceTo(Vector2Int cell_from, Vector2Int cell_to) {
    Vector2Int diff = cell_from - cell_to;
    return diff.x * diff.x + diff.y * diff.y;
}

static Vector2Int
MoveInDirection(Vector2Int cell, u8 direction) {
    switch (direction) {
        case DIRECTION_LEFT:  { cell.x -= 1; } break;
        case DIRECTION_RIGHT: { cell.x += 1; } break;
        case DIRECTION_DOWN:  { cell.y += 1; } break;
        case DIRECTION_UP:    { cell.y -= 1; } break;
    }

    return cell;
}

void
UpdateAnimationSystem(World *world, AnimationSystem *system) {
    constexpr u32 MASK = MASK_SPRITE | MASK_ANIMATION;
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        if ((world->entity_masks[entity] & MASK) != MASK) {
            continue;
        }

        Animation *animation = &world->animations[entity];
        animation->seconds_since_last_frame += world->delta_time;
        if (animation->seconds_between_frames <= animation->seconds_since_last_frame) {
            animation->seconds_since_last_frame = 0.0f;
            Sprite *sprite  = &world->sprites[entity];

            sprite->vertex_array = system->vertex_arrays[animation->base_sprite_id + animation->current_sprite_id];
            if (animation->is_reversed) {
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
UpdatePacmanMovementSystem(World *world, PacmanMovementSystem *system) {
    // Register which direction pacman should move next
    switch (system->input.last_pressed_key) {
        case KEY_A: { system->next_direction = DIRECTION_LEFT; } break;
        case KEY_D: { system->next_direction = DIRECTION_RIGHT; } break;
        case KEY_S: { system->next_direction = DIRECTION_DOWN; } break;
        case KEY_W: { system->next_direction = DIRECTION_UP; } break;
    }

    // Check if we can move in pacmans next direction
    // The MOVEMENT_PRECISION is needed because Pacman cannot be
    // exactly in the center of a cell. If Pacman is within
    // MOVEMENT_PRECISION of the center then we accept it.
    constexpr f32 MOVEMENT_PRECISION = 2.0f;
    Vector2 *translate = &world->transforms[system->pacman].translate;
    Vector2Int cell = ToMazeCoordinates(*translate, world->cell_size);
    Vector2Int next_cell = cell;
    Vector2 cell_center = cell * world->cell_size + world->half_cell_size;
    bool is_in_center_of_cell = false;
    u8 new_base_sprite_id = 0;
    switch (system->next_direction) {
        case DIRECTION_LEFT: {
            next_cell.x -= 1;
            is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_LEFT1;
        } break;
        case DIRECTION_RIGHT: {
            next_cell.x += 1;
            is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_RIGHT1;
        } break;
        case DIRECTION_DOWN: {
            next_cell.y += 1;
            is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_DOWN1;
        } break;
        case DIRECTION_UP: {
            next_cell.y -= 1;
            is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_UP1;
        } break;
    }

    if (is_in_center_of_cell && MAZE[next_cell.y][next_cell.x] != W) {
        system->current_direction = system->next_direction;
        Animation *animation = &world->animations[system->pacman];
        animation->base_sprite_id = new_base_sprite_id;
    }

    // Move in pacmans current direction
    constexpr u32 PACMAN_SPEED = 150;
    f32 speed = PACMAN_SPEED * world->delta_time;
    switch (system->current_direction) {
        case DIRECTION_NONE: {
            world->entity_masks[system->pacman] &= ~MASK_ANIMATION;
        } break;
        case DIRECTION_LEFT: {
            if (MAZE[cell.y][cell.x - 1] != W || Abs(translate->x - cell_center.x) > MOVEMENT_PRECISION) {
                translate->x -= speed;
                world->entity_masks[system->pacman] |= MASK_ANIMATION;
            }
            else {
                system->current_direction = DIRECTION_NONE;
            }
        } break;
        case DIRECTION_RIGHT: {
            if (MAZE[cell.y][cell.x + 1] != W || Abs(translate->x - cell_center.x) > MOVEMENT_PRECISION) {
                translate->x += speed;
                world->entity_masks[system->pacman] |= MASK_ANIMATION;
            }
            else {
                system->current_direction = DIRECTION_NONE;
            }
        } break;
        case DIRECTION_DOWN: {
            if (MAZE[cell.y + 1][cell.x] != W || Abs(translate->y - cell_center.y) > MOVEMENT_PRECISION) {
                translate->y += speed;
                world->entity_masks[system->pacman] |= MASK_ANIMATION;
            }
            else {
                system->current_direction = DIRECTION_NONE;
            }
        } break;
        case DIRECTION_UP: {
            if (MAZE[cell.y - 1][cell.x] != W || Abs(translate->y - cell_center.y) > MOVEMENT_PRECISION) {
                translate->y -= speed;
                world->entity_masks[system->pacman] |= MASK_ANIMATION;
            }
            else {
                system->current_direction = DIRECTION_NONE;
            }
        } break;
    }

    // Eat dots
    if (MAZE[cell.y][cell.x] == d) {
        for (u32 i = 0; i < MAX_ENTITIES; ++i) {
            Vector2 dot_translate = world->transforms[i].translate;
            Vector2Int dot_cell = ToMazeCoordinates(dot_translate, world->cell_size);
            if (dot_cell == cell) {
                world->entity_masks[i] = MASK_NONE;
                break;
            }
        }
    }
}

void
UpdateGhostMovementSystem(World *world, GhostMovementSystem *system) {
    for (u8 ghost_id = 0; ghost_id <= GHOST_BLINKY; ++ghost_id) {
        Ghost *ghost = &system->ghosts[ghost_id];
        if (ghost->has_reached_target_cell) {
            switch (ghost->state) {
                case STATE_CHASE: { NOT_IMPLEMENTED } break;
                case STATE_SCATTER: {
                    ghost->target_cell = ghost->scatter_cells[ghost->scatter_cell_idx];
                    PlatformShowErrorAndExit("Why here");
                } break;
                case STATE_FRIGHTENED: { NOT_IMPLEMENTED } break;
                case STATE_EATEN: { NOT_IMPLEMENTED } break;
            }
        }
        else {
            Vector2 *translate = &world->transforms[ghost->id].translate;
            Vector2Int cell = ToMazeCoordinates(*translate, world->cell_size);
            Vector2Int next_cell = MoveInDirection(cell, ghost->current_direction);
            if (IS_INTERSECTION[cell.y][cell.x]) {
                Vector2Int up_cell = MoveInDirection(cell, DIRECTION_UP);
                u32 distance_up = SquaredDistanceTo(up_cell, ghost->target_cell);
                Vector2Int left_cell = MoveInDirection(cell, DIRECTION_LEFT);
                u32 distance_left = SquaredDistanceTo(left_cell, ghost->target_cell);
                Vector2Int down_cell = MoveInDirection(cell, DIRECTION_DOWN);
                u32 distance_down = SquaredDistanceTo(down_cell, ghost->target_cell);
                Vector2Int right_cell = MoveInDirection(cell, DIRECTION_RIGHT);
                u32 distance_right = SquaredDistanceTo(right_cell, ghost->target_cell);
            }
            else if (MAZE[next_cell.y][next_cell.x] == W) {
                // We are not in an intersection and the next cell is a wall.
                // Since we cannot move back either, we can only move in one
                // of 2 possible directions.
                if (ghost->current_direction == DIRECTION_RIGHT || ghost->current_direction == DIRECTION_LEFT) {
                    Vector2Int up = MoveInDirection(cell, DIRECTION_UP);
                    if (MAZE[up.y][up.x] != W) {
                        ghost->current_direction = DIRECTION_UP;
                    }
                    else {
                        ghost->current_direction = DIRECTION_DOWN;
                    }
                }
                else if (ghost->current_direction == DIRECTION_DOWN || ghost->current_direction == DIRECTION_UP) {
                    Vector2Int right = MoveInDirection(cell, DIRECTION_RIGHT);
                    if (MAZE[right.y][right.x] != W) {
                        ghost->current_direction = DIRECTION_RIGHT;
                    }
                    else {
                        ghost->current_direction = DIRECTION_LEFT;
                    }
                }
            }
            else {
                constexpr u32 GHOST_SPEED = 150;
                f32 speed = GHOST_SPEED * world->delta_time;
                switch (ghost->current_direction) {
                    case DIRECTION_LEFT: {
                        translate->x -= speed;
                    } break;
                    case DIRECTION_RIGHT: {
                        translate->x += speed;
                    } break;
                    case DIRECTION_DOWN: {
                        translate->y += speed;
                    } break;
                    case DIRECTION_UP: {
                        translate->y -= speed;
                    } break;
                }
            }
        }
    }
}
