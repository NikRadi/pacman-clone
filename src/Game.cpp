#include "Game.hpp"
#include "OpenGL.hpp"
#include "Systems.hpp"
#include "World.hpp"


static World world;
static AnimationSystem animation_system;
static RenderSystem render_system;
static PacmanSystem pacman_system;


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

    char *spritesheet_path = "C:\\Users\\nik\\Desktop\\pacman\\sprites\\spritesheet.bmp";
    Texture2D texture = LoadAndBindTexture(spritesheet_path);

    world = {};
    render_system.window_height = window_height;
    pacman_system.cell_size = cell_size;
    pacman_system.half_cell_size = half_cell_size;

    // These constexpr variables are defined here because they are used later also
    constexpr RectangleInt BIG_DOT_RECT = { 233, 240, 8, 8 };
    constexpr RectangleInt PACMAN_RECT = { 261, 0, 15, 15 };
    animation_system.vertex_arrays[SPRITE_ID_BIG_DOT_1] = MakeVertexArray(texture, BIG_DOT_RECT);
    animation_system.vertex_arrays[SPRITE_ID_BIG_DOT_2] = MakeVertexArray(texture, { 242, 240, 8, 8 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT_1] = MakeVertexArray(texture, { 229, 0, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT_2] = MakeVertexArray(texture, { 245, 0, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_RIGHT_3] = MakeVertexArray(texture, PACMAN_RECT);
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT_1] = MakeVertexArray(texture, { 229, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT_2] = MakeVertexArray(texture, { 245, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_LEFT_3] = MakeVertexArray(texture, { 261, 16, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP_1] = MakeVertexArray(texture, { 229, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP_2] = MakeVertexArray(texture, { 245, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_UP_3] = MakeVertexArray(texture, { 261, 32, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN_1] = MakeVertexArray(texture, { 229, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN_2] = MakeVertexArray(texture, { 245, 48, 15, 15 });
    animation_system.vertex_arrays[SPRITE_ID_PACMAN_DOWN_3] = MakeVertexArray(texture, { 261, 48, 15, 15 });

    Entity maze = CreateEntity(&world);
    world.entity_masks[maze.id] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *maze_transform = &world.transforms[maze.id];
    maze_transform->scale = { half_w, half_h };
    maze_transform->translate = { half_w, half_h };

    Sprite *maze_sprite = &world.sprites[maze.id];
    maze_sprite->texture = texture;
    maze_sprite->vertex_array = MakeVertexArray(texture, { 1, 0, 224, 248 });

    for (u32 row = 0; row < MAZE_HEIGHT; ++row) {
        for (u32 col = 0; col < MAZE_WIDTH; ++col) {
            if (MAZE[row][col] == d) {
                Entity small_dot = CreateEntity(&world);
                world.entity_masks[small_dot.id] = MASK_TRANSFORM | MASK_SPRITE;

                Transform *small_dot_transform = &world.transforms[small_dot.id];
                Vector2 small_dot_cell = { static_cast<f32>(col), static_cast<f32>(row) };
                small_dot_transform->translate = cell_size * small_dot_cell + half_cell_size;
                small_dot_transform->scale = cell_size * 0.3f;

                Sprite *small_dot_sprite = &world.sprites[small_dot.id];
                small_dot_sprite->texture = texture;
                small_dot_sprite->vertex_array = MakeVertexArray(texture, { 227, 242, 4, 4 });
            }
            else if (MAZE[row][col] == D) {
                Entity big_dot = CreateEntity(&world);
                world.entity_masks[big_dot.id] = MASK_TRANSFORM | MASK_SPRITE | MASK_ANIMATION;

                Transform *big_dot_transform = &world.transforms[big_dot.id];
                Vector2 big_dot_cell = { static_cast<f32>(col), static_cast<f32>(row) };
                big_dot_transform->translate = cell_size * big_dot_cell + half_cell_size;
                big_dot_transform->scale = half_cell_size;

                Sprite *big_dot_sprite = &world.sprites[big_dot.id];
                big_dot_sprite->texture = texture;
                big_dot_sprite->vertex_array = MakeVertexArray(texture, BIG_DOT_RECT);

                Animation *big_dot_animation = &world.animations[big_dot.id];
                big_dot_animation->base_sprite_id = SPRITE_ID_BIG_DOT_1;
                big_dot_animation->num_frames = 2;
                big_dot_animation->seconds_between_frames = 0.2f;
            }
        }
    }

    constexpr Vector2 BLINKY_STARTING_CELL = { 14.0f, 11.5f };
    Entity blinky = CreateEntity(&world);
    world.entity_masks[blinky.id] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *blinky_transform = &world.transforms[blinky.id];
    blinky_transform->scale = cell_size;
    blinky_transform->translate = cell_size * BLINKY_STARTING_CELL;

    Sprite *blinky_sprite = &world.sprites[blinky.id];
    blinky_sprite->texture = texture;
    blinky_sprite->vertex_array = MakeVertexArray(texture, { 229, 64, 15, 15 });


    constexpr Vector2 PINKY_STARTING_CELL = { 14.0f, 14.5f };
    Entity pinky = CreateEntity(&world);
    world.entity_masks[pinky.id] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *pinky_transform = &world.transforms[pinky.id];
    pinky_transform->scale = cell_size;
    pinky_transform->translate = cell_size * PINKY_STARTING_CELL;

    Sprite *pinky_sprite = &world.sprites[pinky.id];
    pinky_sprite->texture = texture;
    pinky_sprite->vertex_array = MakeVertexArray(texture, { 229, 80, 15, 15 });


    constexpr Vector2 INKY_STARTING_CELL = { 12.0f, 14.5f };
    Entity inky = CreateEntity(&world);
    world.entity_masks[inky.id] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *inky_transform = &world.transforms[inky.id];
    inky_transform->scale = cell_size;
    inky_transform->translate = cell_size * INKY_STARTING_CELL;

    Sprite *inky_sprite = &world.sprites[inky.id];
    inky_sprite->texture = texture;
    inky_sprite->vertex_array = MakeVertexArray(texture, { 229, 96, 15, 15 });


    constexpr Vector2 CLYDE_STARTING_CELL = { 16.0f, 14.5f };
    Entity clyde = CreateEntity(&world);
    world.entity_masks[clyde.id] = MASK_TRANSFORM | MASK_SPRITE;

    Transform *clyde_transform = &world.transforms[clyde.id];
    clyde_transform->scale = cell_size;
    clyde_transform->translate = cell_size * CLYDE_STARTING_CELL;

    Sprite *clyde_sprite = &world.sprites[clyde.id];
    clyde_sprite->texture = texture;
    clyde_sprite->vertex_array = MakeVertexArray(texture, { 229, 112, 15, 15 });


    constexpr Vector2 PACMAN_STARTING_CELL = { 14.0f, 23.5f };
    Entity pacman = CreateEntity(&world);
    world.entity_masks[pacman.id] = MASK_TRANSFORM | MASK_SPRITE | MASK_ANIMATION;
    pacman_system.pacman = pacman;

    Transform *pacman_transform = &world.transforms[pacman.id];
    pacman_transform->scale = cell_size;
    pacman_transform->translate = cell_size * PACMAN_STARTING_CELL;

    Sprite *pacman_sprite = &world.sprites[pacman.id];
    pacman_sprite->texture = texture;
    pacman_sprite->vertex_array = MakeVertexArray(texture, PACMAN_RECT);

    Animation *pacman_animation = &world.animations[pacman.id];
    pacman_animation->base_sprite_id = SPRITE_ID_PACMAN_RIGHT_1;
    pacman_animation->num_frames = 3;
    pacman_animation->seconds_between_frames = 0.05f;
}

void
GameUpdate(f32 delta_time, Input input) {
    pacman_system.delta_time = delta_time;
    pacman_system.input = input;
    animation_system.delta_time = delta_time;

    UpdatePacmanSystem(&world, &pacman_system);
    UpdateAnimationSystem(&world, &animation_system);
    UpdateRenderSystem(&world, &render_system);

    // Update player input
}
