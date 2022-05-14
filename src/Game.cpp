#include "Game.hpp"
#include "OpenGL.hpp"
#include "Systems.hpp"
#include "World.hpp"


static World world;
static AnimationSystem animation_system;
static PacmanMovementSystem pacman_movement_system;
static GhostMovementSystem ghost_movement_system;


void
GameInit(s32 window_width, s32 window_height) {
    OpenGLInit();

    f32 w = static_cast<f32>(window_width);
    f32 h = static_cast<f32>(window_height);
    f32 half_w = w / 2.0f;
    f32 half_h = h / 2.0f;
    Vector2 cell_size = { w / MAZE_WIDTH, h / MAZE_HEIGHT };
    Vector2 half_cell_size = cell_size * 0.5f;

    Matrix4 projection = Orthographic(0.0f, w, 0.0f, h);
    SetMatrix4Uniform("projection", projection);
    Texture2D texture = LoadAndBindTexture("sprites\\spritesheet.bmp");

    world = {};
    ghost_movement_system = {};
    world.cell_size = cell_size;
    world.half_cell_size = half_cell_size;
    world.window_size = { window_width, window_height };
    pacman_movement_system.current_direction = DIRECTION_NONE;

    // These constexpr variables are defined here because they are used later also
    constexpr RectangleInt BIG_DOT_RECT = { 233, 240, 8, 8 };
    constexpr RectangleInt PACMAN_RECT = { 261, 0, 15, 15 };
    animation_system.vertex_arrays[SPRITE_ID_BIG_DOT1]         = MakeVertexArray(texture, BIG_DOT_RECT);
    animation_system.vertex_arrays[SPRITE_ID_BIG_DOT2]         = MakeVertexArray(texture, { 242, 240, 8, 8 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT1]    = MakeVertexArray(texture, { 229, 0, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT2]    = MakeVertexArray(texture, { 245, 0, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT3]    = MakeVertexArray(texture, PACMAN_RECT);
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT1]     = MakeVertexArray(texture, { 229, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT2]     = MakeVertexArray(texture, { 245, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT3]     = MakeVertexArray(texture, { 261, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP1]       = MakeVertexArray(texture, { 229, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP2]       = MakeVertexArray(texture, { 245, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP3]       = MakeVertexArray(texture, { 261, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN1]     = MakeVertexArray(texture, { 229, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN2]     = MakeVertexArray(texture, { 245, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN3]     = MakeVertexArray(texture, { 261, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN3]     = MakeVertexArray(texture, { 261, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_RIGHT1]    = MakeVertexArray(texture, { 229, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_RIGHT2]    = MakeVertexArray(texture, { 245, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_LEFT1]     = MakeVertexArray(texture, { 261, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_LEFT2]     = MakeVertexArray(texture, { 277, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_UP1]       = MakeVertexArray(texture, { 293, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_UP2]       = MakeVertexArray(texture, { 309, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_DOWN1]     = MakeVertexArray(texture, { 325, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_BLINKY_DOWN2]     = MakeVertexArray(texture, { 341, 64, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_RIGHT1]     = MakeVertexArray(texture, { 229, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_RIGHT2]     = MakeVertexArray(texture, { 245, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_LEFT1]      = MakeVertexArray(texture, { 261, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_LEFT2]      = MakeVertexArray(texture, { 277, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_UP1]        = MakeVertexArray(texture, { 293, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_UP2]        = MakeVertexArray(texture, { 309, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_DOWN1]      = MakeVertexArray(texture, { 325, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_PINKY_DOWN2]      = MakeVertexArray(texture, { 341, 80, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_RIGHT1]      = MakeVertexArray(texture, { 229, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_RIGHT2]      = MakeVertexArray(texture, { 245, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_LEFT1]       = MakeVertexArray(texture, { 261, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_LEFT2]       = MakeVertexArray(texture, { 277, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_UP1]         = MakeVertexArray(texture, { 293, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_UP2]         = MakeVertexArray(texture, { 309, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_DOWN1]       = MakeVertexArray(texture, { 325, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_INKY_DOWN2]       = MakeVertexArray(texture, { 341, 96, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_RIGHT1]     = MakeVertexArray(texture, { 229, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_RIGHT2]     = MakeVertexArray(texture, { 245, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_LEFT1]      = MakeVertexArray(texture, { 261, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_LEFT2]      = MakeVertexArray(texture, { 277, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_UP1]        = MakeVertexArray(texture, { 293, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_UP2]        = MakeVertexArray(texture, { 309, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_DOWN1]      = MakeVertexArray(texture, { 325, 112, 16, 16 });
    animation_system.vertex_arrays[SPRITE_ID_CLYDE_DOWN2]      = MakeVertexArray(texture, { 341, 112, 16, 16 });

    Sprite sprite;
    sprite.texture = texture;

    Entity maze = CreateEntity(&world);
    world.entity_masks[maze] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *maze_transform = &world.transforms[maze];
    maze_transform->scale = { half_w, half_h };
    maze_transform->translate = { half_w, half_h };

    sprite.vertex_array = MakeVertexArray(texture, { 1, 0, 224, 248 });
    world.sprites[maze] = sprite;

    for (u32 row = 0; row < MAZE_HEIGHT; ++row) {
        for (u32 col = 0; col < MAZE_WIDTH; ++col) {
            if (MAZE[row][col] == d) {
                Entity small_dot = CreateEntity(&world);
                world.entity_masks[small_dot] = MASK_TRANSFORM | MASK_SPRITE;

                Transform *small_dot_transform = &world.transforms[small_dot];
                Vector2 small_dot_cell = { static_cast<f32>(col), static_cast<f32>(row) };
                small_dot_transform->translate = cell_size * small_dot_cell + half_cell_size;
                small_dot_transform->scale = cell_size * 0.3f;

                sprite.vertex_array = MakeVertexArray(texture, { 227, 242, 4, 4 });
                world.sprites[small_dot] = sprite;
            }
            else if (MAZE[row][col] == D) {
                Entity big_dot = CreateEntity(&world);
                world.entity_masks[big_dot] = MASK_TRANSFORM | MASK_SPRITE | MASK_ANIMATION;

                Transform *big_dot_transform = &world.transforms[big_dot];
                Vector2 big_dot_cell = { static_cast<f32>(col), static_cast<f32>(row) };
                big_dot_transform->translate = cell_size * big_dot_cell + half_cell_size;
                big_dot_transform->scale = half_cell_size;

                sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_BIG_DOT1];
                world.sprites[big_dot] = sprite;

                Animation *big_dot_animation = &world.animations[big_dot];
                big_dot_animation->base_sprite_id = SPRITE_ID_BIG_DOT1;
                big_dot_animation->num_frames = 2;
                big_dot_animation->seconds_between_frames = 0.2f;
            }
        }
    }

    Transform transform;
    transform.scale = cell_size;

    constexpr Vector2 BLINKY_STARTING_CELL = { 14.0f, 11.5f };
    transform.translate = cell_size * BLINKY_STARTING_CELL;
    sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_BLINKY_LEFT1];
    Entity blinky = CreateGhost(&world, transform, sprite, SPRITE_ID_BLINKY_LEFT1);

    Ghost *blinky_ghost = &ghost_movement_system.ghosts[GHOST_BLINKY];
    blinky_ghost->id = blinky;
    blinky_ghost->state = STATE_SCATTER;
    blinky_ghost->current_direction = DIRECTION_LEFT;
    blinky_ghost->scatter_cells[0] = { 22, 5 };
    blinky_ghost->scatter_cells[1] = { 22, 2 };
    blinky_ghost->scatter_cells[2] = { 27, 2 };
    blinky_ghost->scatter_cells[3] = { 27, 5 };


    constexpr Vector2 PINKY_STARTING_CELL = { 14.0f, 14.5f };
    transform.translate = cell_size * PINKY_STARTING_CELL;
    sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_PINKY_UP1];
    CreateGhost(&world, transform, sprite, SPRITE_ID_PINKY_UP1);

    constexpr Vector2 INKY_STARTING_CELL = { 12.0f, 14.5f };
    transform.translate = cell_size * INKY_STARTING_CELL;
    sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_INKY_DOWN1];
    CreateGhost(&world, transform, sprite, SPRITE_ID_INKY_DOWN1);

    constexpr Vector2 CLYDE_STARTING_CELL = { 16.0f, 14.5f };
    transform.translate = cell_size * CLYDE_STARTING_CELL;
    sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_CLYDE_DOWN1];
    CreateGhost(&world, transform, sprite, SPRITE_ID_CLYDE_DOWN1);

    constexpr Vector2 PACMAN_STARTING_CELL = { 14.0f, 23.5f };
    Entity pacman = CreateEntity(&world);
    // Pacmans animation system is enabled when he moves
    world.entity_masks[pacman] = MASK_TRANSFORM | MASK_SPRITE;
    pacman_movement_system.pacman = pacman;

    transform.translate = cell_size * PACMAN_STARTING_CELL;
    world.transforms[pacman] = transform;
    sprite.vertex_array = animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT3];
    world.sprites[pacman] = sprite;

    Animation *pacman_animation = &world.animations[pacman];
    pacman_animation->base_sprite_id = SPRITE_ID_PACMAN_RIGHT1;
    pacman_animation->num_frames = 3;
    pacman_animation->seconds_between_frames = 0.05f;
}

void
GameUpdate(f32 delta_time, Input input) {
    world.delta_time = delta_time;
    pacman_movement_system.input = input;

    UpdatePacmanMovementSystem(&world, &pacman_movement_system);
    UpdateGhostMovementSystem(&world, &ghost_movement_system);
    UpdateAnimationSystem(&world, &animation_system);
    UpdateRenderSystem(&world);
}
