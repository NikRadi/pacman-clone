#include "Game.hpp"
#include "Math.hpp"
#include "OpenGL.hpp"


enum {
    DIRECTION_NONE,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_UP,
};

struct Entity {
    u32 VAO;
    Vector2 scale;
    Vector2 translate;
};

struct Pacman : public Entity {
    s32 direction;
    s32 next_direction;
};


enum {
    D, // Dot
    E, // Empty
    W, // Wall
};


constexpr static s32 MAZE_WIDTH = 28;
constexpr static s32 MAZE_HEIGHT = 31;
constexpr static u8 MAZE[MAZE_HEIGHT][MAZE_WIDTH] = {
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,
    W,D,D,D,D,D,D,D,D,D,D,D,D,W,W,D,D,D,D,D,D,D,D,D,D,D,D,W,
    W,D,W,W,W,W,D,W,W,W,W,W,D,W,W,D,W,W,W,W,W,D,W,W,W,W,D,W,
    W,D,W,W,W,W,D,W,W,W,W,W,D,W,W,D,W,W,W,W,W,D,W,W,W,W,D,W,
    W,D,W,W,W,W,D,W,W,W,W,W,D,W,W,D,W,W,W,W,W,D,W,W,W,W,D,W,
    W,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,W,
    W,D,W,W,W,W,D,W,W,D,W,W,W,W,W,W,W,W,D,W,W,D,W,W,W,W,D,W,
    W,D,W,W,W,W,D,W,W,D,W,W,W,W,W,W,W,W,D,W,W,D,W,W,W,W,D,W,
    W,D,D,D,D,D,D,W,W,D,D,D,D,W,W,D,D,D,D,W,W,D,D,D,D,D,D,W,
    W,W,W,W,W,W,D,W,W,W,W,W,E,W,W,E,W,W,W,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,W,W,W,E,W,W,E,W,W,W,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    E,E,E,E,E,E,D,E,E,E,E,E,E,E,E,E,E,E,E,E,E,D,E,E,E,E,E,E,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,E,E,E,E,E,E,E,E,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,W,W,W,W,W,W,W,W,E,W,W,D,W,W,W,W,W,W,
    W,W,W,W,W,W,D,W,W,E,W,W,W,W,W,W,W,W,E,W,W,D,W,W,W,W,W,W,
    W,D,D,D,D,D,D,D,D,D,D,D,D,W,W,D,D,D,D,D,D,D,D,D,D,D,D,W,
    W,D,W,W,W,W,D,W,W,W,W,W,D,W,W,D,W,W,W,W,W,D,W,W,W,W,D,W,
    W,D,W,W,W,W,D,W,W,W,W,W,D,W,W,D,W,W,W,W,W,D,W,W,W,W,D,W,
    W,D,D,D,W,W,D,D,D,D,D,D,D,E,E,D,D,D,D,D,D,D,W,W,D,D,D,W,
    W,W,W,D,W,W,D,W,W,D,W,W,W,W,W,W,W,W,D,W,W,D,W,W,D,W,W,W,
    W,W,W,D,W,W,D,W,W,D,W,W,W,W,W,W,W,W,D,W,W,D,W,W,D,W,W,W,
    W,D,D,D,D,D,D,W,W,D,D,D,D,W,W,D,D,D,D,W,W,D,D,D,D,D,D,W,
    W,D,W,W,W,W,W,W,W,W,W,W,D,W,W,D,W,W,W,W,W,W,W,W,W,W,D,W,
    W,D,W,W,W,W,W,W,W,W,W,W,D,W,W,D,W,W,W,W,W,W,W,W,W,W,D,W,
    W,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,W,
    W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W
};

static constexpr s32 PACMAN_SPEED = 150;
static constexpr s32 MAX_ENTITIES = 1048;
static constexpr s32 MAX_DOTS = 1048;
static s32 num_entities = 0;
static s32 num_dots = 0;
static u32 model_location;
static Vector2 cell_size;
static Entity maze;
static Pacman pacman;
static Entity dots[MAX_DOTS];
static Entity *entities[MAX_ENTITIES];


static u32
InitVAO(Texture2D texture, RectangleInt rect) {
    u32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    f32 x1 = static_cast<f32>(rect.left) / texture.width;
    f32 y1 = 1.0f - static_cast<f32>(rect.top) / texture.height;
    f32 x2 = x1 + (static_cast<f32>(rect.width) / texture.width);
    f32 y2 = y1 - (static_cast<f32>(rect.height) / texture.height);

    f32 vertices[] = {
        -1.0f,  1.0f, x1, y1,
         1.0f,  1.0f, x2, y1,
        -1.0f, -1.0f, x1, y2,
         1.0f, -1.0f, x2, y2
    };

    u32 VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void *) (2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    u32 indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    u32 EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDeleteBuffers(1, &EBO);

    return VAO;
}

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

    u32 program_id = OpenGLCreateProgram();
    glUseProgram(program_id);
    model_location = glGetUniformLocation(program_id, "model");

    f32 w = static_cast<f32>(window_width);
    f32 h = static_cast<f32>(window_height);
    f32 half_w = w / 2.0f;
    f32 half_h = h / 2.0f;
    Matrix4 projection = Orthographic(0.0f, w, 0.0f, h);
    SetMatrix4Uniform(program_id, "projection", projection);

    char *spritesheet_path = "C:\\Users\\nik\\Desktop\\pacman\\sprites\\spritesheet.bmp";
    Texture2D texture = OpenGLLoadAndBindTexture(spritesheet_path);
    glBindTexture(GL_TEXTURE_2D, texture.handle);

    cell_size.x = w / MAZE_WIDTH;
    cell_size.y = h / MAZE_HEIGHT;
    RectangleInt rect;

    rect = { 1, 0, 224, 248 };
    maze.VAO = InitVAO(texture, rect);
    maze.scale = { half_w, half_h };
    maze.translate = { half_w, half_h };
    AddEntity(&maze);

    rect = { 227, 242, 4, 4 };
    for (s32 row = 0; row < MAZE_HEIGHT; ++row) {
        for (s32 col = 0; col < MAZE_WIDTH; ++col) {
            Entity dot;
            dot.scale = cell_size * 0.8f;
            dot.translate.x = cell_size.x * col;
            dot.translate.y = cell_size.y * row;
            dot.translate += cell_size * 0.5f;
            if (MAZE[row][col] == W) {
                dot.VAO = InitVAO(texture, rect);
                dots[num_dots] = dot;
                AddEntity(&dots[num_dots]);
                num_dots += 1;
            }
        }
    }

    rect = { 261, 0, 15, 15 };
    pacman.VAO = InitVAO(texture, rect);
    pacman.scale = cell_size;
    pacman.translate.x = half_w;
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
    Vector2Int next_cell = cell;
    switch (pacman.next_direction) {
        case DIRECTION_LEFT:  { next_cell.x -= 1; } break;
        case DIRECTION_RIGHT: { next_cell.x += 1; } break;
        case DIRECTION_DOWN:  { next_cell.y += 1; } break;
        case DIRECTION_UP:    { next_cell.y -= 1; } break;
    }

    if (MAZE[next_cell.y][next_cell.x] != W) {
        pacman.direction = pacman.next_direction;
    }

    // Move in pacmans current direction
    f32 speed = delta_time * PACMAN_SPEED;
    switch (pacman.direction) {
        case DIRECTION_LEFT: {
            if (MAZE[cell.y][cell.x - 1] != W) {
                pacman.translate.x -= speed;
            }
        } break;
        case DIRECTION_RIGHT: {
            if (MAZE[cell.y][cell.x + 1] != W) {
                pacman.translate.x += speed;
            }
        } break;
        case DIRECTION_DOWN: {
            if (MAZE[cell.y + 1][cell.x] != W) {
                pacman.translate.y += speed;
            }
        } break;
        case DIRECTION_UP: {
            if (MAZE[cell.y - 1][cell.x] != W) {
                pacman.translate.y -= speed;
            }
        } break;
    }
}

void
GameRender() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (s32 i = 0; i < num_entities; ++i) {
        Entity *entity = entities[i];

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

