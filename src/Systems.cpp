#include "Systems.hpp"
#include "OpenGL.hpp"


static Vector2Int
ToMazeCoordinates(Vector2 translate, Vector2 cell_size) {
    Vector2Int maze_coordinates;
    maze_coordinates.x = static_cast<s32>(translate.x / cell_size.x);
    maze_coordinates.y = static_cast<s32>(translate.y / cell_size.y);
    return maze_coordinates;
}

void
UpdateAnimationSystem(World *world, AnimationSystem *system) {
    constexpr u32 MASK = MASK_SPRITE | MASK_ANIMATION;
    for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        if ((world->entity_masks[i] & MASK) != MASK) {
            continue;
        }

        Animation *animation = &world->animations[i];
        animation->seconds_since_last_frame += system->delta_time;
        if (animation->seconds_between_frames <= animation->seconds_since_last_frame) {
            animation->seconds_since_last_frame = 0.0f;
            Sprite *sprite  = &world->sprites[i];

            sprite->vertex_array = system->vertex_arrays[animation->base_sprite_id + animation->current_sprite_id];
            animation->current_sprite_id = (animation->current_sprite_id + 1) % animation->num_frames;
        }
    }
}

void
UpdateRenderSystem(World *world, RenderSystem *system) {
    constexpr u32 MASK = MASK_TRANSFORM | MASK_SPRITE;
    glClear(GL_COLOR_BUFFER_BIT);
    for (u32 i = 0; i < MAX_ENTITIES; ++i) {
        if ((world->entity_masks[i] & MASK) != MASK) {
            continue;
        }

        Transform *transform = &world->transforms[i];
        Sprite *sprite = &world->sprites[i];

        glBindTexture(GL_TEXTURE_2D, sprite->texture.handle);
        glBindVertexArray(sprite->vertex_array.id);

        // We want (0, 0) to be the top left corner
        Vector2 translate;
        translate.x = transform->translate.x;
        translate.y = system->window_height - transform->translate.y;

        Matrix4 model = IDENDITY_MATRIX4;
        model = Scale(model, transform->scale);
        model = Translate(model, translate);
        SetMatrix4Uniform("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void
UpdatePacmanSystem(World *world, PacmanSystem *system) {
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
    Vector2 *translate = &world->transforms[system->pacman.id].translate;
    Vector2Int cell = ToMazeCoordinates(*translate, system->cell_size);
    Vector2Int next_cell = cell;
    Vector2 cell_center = cell * system->cell_size + system->half_cell_size;
    bool is_in_center_of_cell = false;
    u8 new_base_sprite_id = 0;
    switch (system->next_direction) {
        case DIRECTION_LEFT: {
            next_cell.x -= 1;
            is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_LEFT_1;
        } break;
        case DIRECTION_RIGHT: {
            next_cell.x += 1;
            is_in_center_of_cell = Abs(translate->y - cell_center.y) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_RIGHT_1;
        } break;
        case DIRECTION_DOWN: {
            next_cell.y += 1;
            is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_DOWN_1;
        } break;
        case DIRECTION_UP: {
            next_cell.y -= 1;
            is_in_center_of_cell = Abs(translate->x - cell_center.x) < MOVEMENT_PRECISION;
            new_base_sprite_id = SPRITE_ID_PACMAN_UP_1;
        } break;
    }

    if (is_in_center_of_cell && MAZE[next_cell.y][next_cell.x] != W) {
        system->current_direction = system->next_direction;
        Animation *animation = &world->animations[system->pacman.id];
        animation->base_sprite_id = new_base_sprite_id;
    }

    // Move in pacmans current direction
    constexpr u32 PACMAN_SPEED = 150;
    f32 speed = PACMAN_SPEED * system->delta_time;
    switch (system->current_direction) {
        case DIRECTION_LEFT: {
            if (MAZE[cell.y][cell.x - 1] != W || Abs(translate->x - cell_center.x) > MOVEMENT_PRECISION) {
                translate->x -= speed;
            }
        } break;
        case DIRECTION_RIGHT: {
            if (MAZE[cell.y][cell.x + 1] != W || Abs(translate->x - cell_center.x) > MOVEMENT_PRECISION) {
                translate->x += speed;
            }
        } break;
        case DIRECTION_DOWN: {
            if (MAZE[cell.y + 1][cell.x] != W || Abs(translate->y - cell_center.y) > MOVEMENT_PRECISION) {
                translate->y += speed;
            }
        } break;
        case DIRECTION_UP: {
            if (MAZE[cell.y - 1][cell.x] != W || Abs(translate->y - cell_center.y) > MOVEMENT_PRECISION) {
                translate->y -= speed;
            }
        } break;
    }

    // Eat dots
    if (MAZE[cell.y][cell.x] == d) {
        for (u32 i = 0; i < MAX_ENTITIES; ++i) {
            Vector2 dot_translate = world->transforms[i].translate;
            Vector2Int dot_cell = ToMazeCoordinates(dot_translate, system->cell_size);
            if (dot_cell == cell) {
                world->entity_masks[i] = MASK_NONE;
                break;
            }
        }
    }
}
