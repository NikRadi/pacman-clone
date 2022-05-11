#include "Game.hpp"
#include "Math.hpp"
#include "OpenGL.hpp"


struct Entity {
    u32 VAO;
    Vector2 scale;
    Vector2 translate;
    bool is_active = true;
};

struct Pacman : public Entity {
    s32 direction;
    s32 next_direction;
};

template <class NumClips>
struct Animation {
    RectangleInt texture_coords[NumClips];
    s32 current_clip = 0;
};

enum {
    DIRECTION_NONE,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_UP,
};

enum {
    d, // Small dot
    D, // Big dot
    E, // Empty
    W, // Wall
};


constexpr static s32 MAZE_WIDTH = 28;
constexpr static s32 MAZE_HEIGHT = 31;
constexpr static u8 MAZE[MAZE_HEIGHT][MAZE_WIDTH] = {
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,W,W,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,D,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,D,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W,
    W,d,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,d,W,
    W,d,d,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,d,d,W,
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,W,W,W,E,W,W,E,W,W,W,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    E,E,E,E,E,E,d,E,E,E,E,E,E,E,E,E,E,E,E,E,E,d,E,E,E,E,E,E,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,E,E,E,E,E,E,E,E,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,W,W,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,W,W,W,W,W,d,W,W,E,W,W,W,W,W,W,W,W,E,W,W,d,W,W,W,W,W,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,W,W,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,d,W,W,W,W,d,W,W,W,W,W,d,W,W,d,W,W,W,W,W,d,W,W,W,W,d,W,
    W,D,d,d,W,W,d,d,d,d,d,d,d,E,E,d,d,d,d,d,d,d,W,W,d,d,D,W,
    W,W,W,d,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,d,W,W,W,
    W,W,W,d,W,W,d,W,W,d,W,W,W,W,W,W,W,W,d,W,W,d,W,W,d,W,W,W,
    W,d,d,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,W,W,d,d,d,d,d,d,W,
    W,d,W,W,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,W,W,d,W,
    W,d,W,W,W,W,W,W,W,W,W,W,d,W,W,d,W,W,W,W,W,W,W,W,W,W,d,W,
    W,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,W,
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W
};

static constexpr RectangleInt MAZE_RECT = { 1, 0, 224, 248 };
static constexpr RectangleInt PACMAN_RECT = { 261, 0, 15, 15 };
static constexpr RectangleInt SMALL_DOT_RECT = { 227, 242, 4, 4 };
static constexpr RectangleInt BIG_DOT_RECT = { 233, 240, 8, 8 };

static constexpr f32 MOVEMENT_PRECISION = 2.0f;
static constexpr s32 PACMAN_SPEED = 150;
static constexpr s32 MAX_ENTITIES = 1048;

static s32 num_entities = 0;
static s32 num_dots = 0;
static u32 model_location;
static Vector2 cell_size;

static Pacman pacman;
static Entity maze;
static Entity dots[1048]; // Just picked some number
static Entity *entities[MAX_ENTITIES];


static void
AddEntity(Entity *entity) {
    ASSERT(num_entities < MAX_ENTITIES);
    entities[num_entities] = entity;
    num_entities += 1;
}

static Vector2Int
ToMazeCoordinates(Vector2 position) {
    Vector2Int maze_coordinates;
    maze_coordinates.x = static_cast<s32>(position.x / cell_size.x);
    maze_coordinates.y = static_cast<s32>(position.y / cell_size.y);
    return maze_coordinates;
}

void
GameInit(s32 window_width, s32 window_height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    u32 program_id = CreateOpenGLProgram();
    glUseProgram(program_id);
    model_location = glGetUniformLocation(program_id, "model");

    f32 w = static_cast<f32>(window_width);
    f32 h = static_cast<f32>(window_height);
    f32 half_w = w / 2.0f;
    f32 half_h = h / 2.0f;
    cell_size.x = w / MAZE_WIDTH;
    cell_size.y = h / MAZE_HEIGHT;

    Matrix4 projection = Orthographic(0.0f, w, 0.0f, h);
    SetMatrix4Uniform(program_id, "projection", projection);

    char *spritesheet_path = "C:\\Users\\nik\\Desktop\\pacman\\sprites\\spritesheet.bmp";
    Texture2D texture = LoadAndBindTexture(spritesheet_path);

    maze.VAO = InitVAO(texture, MAZE_RECT);
    maze.scale = { half_w, half_h };
    maze.translate = { half_w, half_h };
    AddEntity(&maze);

    for (s32 row = 0; row < MAZE_HEIGHT; ++row) {
        for (s32 col = 0; col < MAZE_WIDTH; ++col) {
            bool is_small_dot = MAZE[row][col] == d;
            bool is_big_dot = MAZE[row][col] == D;
            if (is_small_dot || is_big_dot) {
                Entity dot;
                dot.translate = { cell_size.x * col, cell_size.y * row };
                dot.translate += cell_size * 0.5f;
                if (is_small_dot) {
                    dot.VAO = InitVAO(texture, SMALL_DOT_RECT);
                    dot.scale = cell_size * 0.2f;
                }
                else {
                    dot.VAO = InitVAO(texture, BIG_DOT_RECT);
                    dot.scale = cell_size * 0.5f;
                }
                
                dots[num_dots] = dot;
                AddEntity(&dots[num_dots]);
                num_dots += 1;
            }
        }
    }

    pacman.VAO = InitVAO(texture, PACMAN_RECT);
    pacman.scale = cell_size;
    pacman.translate.x = cell_size.x * 14.0f;
    pacman.translate.y = cell_size.y * 23.5f;
    pacman.direction = DIRECTION_NONE;
    pacman.next_direction = DIRECTION_NONE;
    AddEntity(&pacman);
}

void
GameUpdate(f32 delta_time, Input input) {
    // Register which direction pacman should move next
    switch (input.last_pressed_key) {
        case KEY_A: { pacman.next_direction = DIRECTION_LEFT;  } break;
        case KEY_D: { pacman.next_direction = DIRECTION_RIGHT; } break;
        case KEY_S: { pacman.next_direction = DIRECTION_DOWN;  } break;
        case KEY_W: { pacman.next_direction = DIRECTION_UP;    } break;
    }

    // Check if we can move in pacmans next direction
    Vector2Int cell = ToMazeCoordinates(pacman.translate);
    Vector2 cell_center = { cell.x * cell_size.x, cell.y * cell_size.y };
    cell_center += cell_size * 0.5f;
    Vector2Int next_cell = cell;
    bool is_in_center_of_current_cell = false;
    switch (pacman.next_direction) {
        case DIRECTION_LEFT: {
            next_cell.x -= 1;
            is_in_center_of_current_cell = Abs(pacman.translate.y - cell_center.y) < MOVEMENT_PRECISION;
        } break;
        case DIRECTION_RIGHT: {
            next_cell.x += 1;
            is_in_center_of_current_cell = Abs(pacman.translate.y - cell_center.y) < MOVEMENT_PRECISION;
        } break;
        case DIRECTION_DOWN: {
            next_cell.y += 1;
            is_in_center_of_current_cell = Abs(pacman.translate.x - cell_center.x) < MOVEMENT_PRECISION;
        } break;
        case DIRECTION_UP: {
            next_cell.y -= 1;
            is_in_center_of_current_cell = Abs(pacman.translate.x - cell_center.x) < MOVEMENT_PRECISION;
        } break;
    }

    if (is_in_center_of_current_cell && MAZE[next_cell.y][next_cell.x] != W) {
        pacman.direction = pacman.next_direction;
    }

    // Move in pacmans current direction
    f32 speed = delta_time * PACMAN_SPEED;
    switch (pacman.direction) {
        case DIRECTION_LEFT: {
            if (MAZE[cell.y][cell.x - 1] != W || Abs(pacman.translate.x - cell_center.x) > MOVEMENT_PRECISION) {
                pacman.translate.x -= speed;
            }
        } break;
        case DIRECTION_RIGHT: {
            if (MAZE[cell.y][cell.x + 1] != W || Abs(pacman.translate.x - cell_center.x) > MOVEMENT_PRECISION) {
                pacman.translate.x += speed;
            }
        } break;
        case DIRECTION_DOWN: {
            if (MAZE[cell.y + 1][cell.x] != W || Abs(pacman.translate.y - cell_center.y) > MOVEMENT_PRECISION) {
                pacman.translate.y += speed;
            }
        } break;
        case DIRECTION_UP: {
            if (MAZE[cell.y - 1][cell.x] != W ||Abs(pacman.translate.y - cell_center.y) > MOVEMENT_PRECISION) {
                pacman.translate.y -= speed;
            }
        } break;
    }

    // Eat dots
    if (MAZE[cell.y][cell.x] == d || MAZE[cell.y][cell.x] == D) {
        for (int i = 0; i < num_dots; ++i) {
            Vector2Int dot_cell = ToMazeCoordinates(dots[i].translate);
            if (dot_cell == cell) {
                dots[i].is_active = false;
                break;
            }
        }
    }
}

void
GameRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (s32 i = 0; i < num_entities; ++i) {
        Entity *entity = entities[i];
        if (!entity->is_active) {
            continue;
        }

        glBindVertexArray(entity->VAO);
        Matrix4 model = IDENDITY_MATRIX4;
        model = Scale(model, entity->scale);

        Vector2 translate = entity->translate;
        translate.y = (MAZE_HEIGHT * cell_size.y) - translate.y;
        model = Translate(model, translate);
        SetMatrix4Uniform(model_location, model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
