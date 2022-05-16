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
    if (MAZE[cell.y][cell.x] == d || MAZE[cell.y][cell.x] == D) {
        for (u32 i = 0; i < MAX_ENTITIES; ++i) {
            Vector2 dot_translate = world->transforms[i].translate;
            Vector2Int dot_cell = ToMazeCoordinates(dot_translate, world->cell_size);
            if (dot_cell == cell) {
                world->entity_masks[i] = MASK_NONE;
                break;
            }
        }

        if (MAZE[cell.y][cell.x] == D) {
            for (u32 i = 0; i <= GHOST_PINKY; ++i) {
                system->ghosts[i]->state = STATE_FRIGHTENED;
            }
        }
    }
}

void
UpdateGhostMovementSystem(World *world, GhostMovementSystem *system) {
    for (u8 ghost_id = 0; ghost_id <= GHOST_PINKY; ++ghost_id) {
        Ghost *ghost = &system->ghosts[ghost_id];
        Animation *animation = &world->animations[ghost->id];
        ghost->current_seconds += world->delta_time;
        switch (ghost->state) {
            case STATE_CHASE: {
                Vector2 pacman_translate = world->transforms[system->pacman].translate;
                ghost->target_cell = ToMazeCoordinates(pacman_translate, world->cell_size);
                if (ghost->current_seconds >= ghost->seconds_in_state[STATE_CHASE]) {
                    ghost->state = STATE_SCATTER;
                    ghost->current_seconds = 0.0f;
                }
            } break;
            case STATE_SCATTER: {
                ghost->target_cell = ghost->scatter_cell;
                if (ghost->current_seconds >= ghost->seconds_in_state[STATE_SCATTER]) {
                    ghost->state = STATE_CHASE;
                    ghost->current_seconds = 0.0f;
                }
            } break;
            case STATE_FRIGHTENED: {
                animation->base_sprite_id = SPRITE_ID_GHOST_FRIGHTENED1;
                switch (ghost->current_direction) {
                    case DIRECTION_LEFT:  { ghost->current_direction = DIRECTION_RIGHT; } break;
                    case DIRECTION_RIGHT: { ghost->current_direction = DIRECTION_LEFT; } break;
                    case DIRECTION_DOWN:  { ghost->current_direction = DIRECTION_DOWN; } break;
                    case DIRECTION_UP:    { ghost->current_direction = DIRECTION_UP; } break;
                }
            } break;
            case STATE_EATEN: { NOT_IMPLEMENTED } break;
        }

        constexpr f32 MOVEMENT_PRECISION = 2.0f;
        Vector2 *translate = &world->transforms[ghost->id].translate;
        Vector2Int cell = ToMazeCoordinates(*translate, world->cell_size);
        Vector2Int next_cell = cell;
        Vector2 cell_center = cell * world->cell_size + world->half_cell_size;
        bool is_in_center_of_cell = false;
        switch (ghost->current_direction) {
            case DIRECTION_LEFT: {
                next_cell.x -= 1;
                is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            } break;
            case DIRECTION_RIGHT: {
                next_cell.x += 1;
                is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            } break;
            case DIRECTION_DOWN: {
                next_cell.y += 1;
                is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            } break;
            case DIRECTION_UP: {
                next_cell.y -= 1;
                is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            } break;
        }

        if (is_in_center_of_cell && IS_INTERSECTION[cell.y][cell.x] && cell != ghost->last_intersection_cell) {
            ghost->last_intersection_cell = cell;
            Vector2Int cell_left = cell;
            cell_left.x -= 1;
            Vector2Int cell_right = cell;
            cell_right.x += 1;
            Vector2Int cell_down = cell;
            cell_down.y += 1;
            Vector2Int cell_up = cell;
            cell_up.y -= 1;

            u8 next_direction = DIRECTION_NONE;
            u32 best_distance = 9999;
            switch (ghost->current_direction) {
                case DIRECTION_LEFT: {
                    if (MAZE[cell_up.y][cell_up.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_up, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_UP;
                        }
                    }

                    if (MAZE[cell_left.y][cell_left.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_left, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_LEFT;
                        }
                    }

                    if (MAZE[cell_down.y][cell_down.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_down, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_DOWN;
                        }
                    }
                } break;
                case DIRECTION_RIGHT: {
                    if (MAZE[cell_up.y][cell_up.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_up, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_UP;
                        }
                    }

                    if (MAZE[cell_down.y][cell_down.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_down, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_DOWN;
                        }
                    }

                    if (MAZE[cell_right.y][cell_right.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_right, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_RIGHT;
                        }
                    }
                } break;
                case DIRECTION_DOWN: {
                    if (MAZE[cell_left.y][cell_left.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_left, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_LEFT;
                        }
                    }

                    if (MAZE[cell_down.y][cell_down.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_down, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_DOWN;
                        }
                    }

                    if (MAZE[cell_right.y][cell_right.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_right, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_RIGHT;
                        }
                    }
                } break;
                case DIRECTION_UP: {
                    if (MAZE[cell_up.y][cell_up.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_up, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_UP;
                        }
                    }

                    if (MAZE[cell_left.y][cell_left.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_left, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_LEFT;
                        }
                    }

                    if (MAZE[cell_right.y][cell_right.x] != W) {
                        u32 distance = SquaredDistanceTo(cell_right, ghost->target_cell);
                        if (distance < best_distance) {
                            best_distance = distance;
                            next_direction = DIRECTION_RIGHT;
                        }
                    }
                } break;
            }

            ghost->current_direction = next_direction;
            animation->base_sprite_id = ghost->base_sprite_ids[next_direction];
        }
        else if (is_in_center_of_cell && MAZE[next_cell.y][next_cell.x] == W) {
            if (ghost->current_direction == DIRECTION_LEFT || ghost->current_direction == DIRECTION_RIGHT) {
                if (MAZE[cell.y + 1][cell.x] != W) {
                    ghost->current_direction = DIRECTION_DOWN;
                    animation->base_sprite_id = ghost->base_sprite_ids[DIRECTION_DOWN];
                }
                else {
                    ghost->current_direction = DIRECTION_UP;
                    animation->base_sprite_id = ghost->base_sprite_ids[DIRECTION_UP];
                }
            }
            else {
                if (MAZE[cell.y][cell.x + 1] != W) {
                    ghost->current_direction = DIRECTION_RIGHT;
                    animation->base_sprite_id = ghost->base_sprite_ids[DIRECTION_RIGHT];
                }
                else {
                    ghost->current_direction = DIRECTION_LEFT;
                    animation->base_sprite_id = ghost->base_sprite_ids[DIRECTION_LEFT];
                }
            }
        }
        
        f32 speed = 150 * world->delta_time;
        switch (ghost->current_direction) {
            case DIRECTION_LEFT: { translate->x -= speed; } break;
            case DIRECTION_RIGHT: { translate->x += speed; } break;
            case DIRECTION_DOWN: { translate->y += speed; } break;
            case DIRECTION_UP: { translate->y -= speed; } break;
        }
    }
}
