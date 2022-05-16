#include "OpenGL.hpp"
#include "Platform.hpp"


#pragma pack(push, 1)
struct BitmapFile {
    u16 signature;
    u32 file_size;
    u16 reserved1;
    u16 reserved2;
    u32 pixels_offset;
    u32 size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 *pixels;
};
#pragma pack(pop)


static char *vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec2 a_position;\n"
    "layout (location = 1) in vec2 a_texcoord;\n"
    ""
    "out vec2 v_texcoord;\n"
    ""
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    ""
    "void main() {\n"
    "    gl_Position = projection * model * vec4(a_position, 0.0, 1.0);\n"
    "    v_texcoord = a_texcoord;\n"
    "}\n"
    "";

static char *fragment_shader =
    "#version 330 core\n"
    "in vec2 v_texcoord;\n"
    ""
    "uniform sampler2D texture1;\n"
    ""
    "void main() {\n"
    "    gl_FragColor = texture(texture1, v_texcoord);\n"
    "}\n"
    "";


static u32 program_id;


static u32
CreateAndCompileShader(char *shader_code, GLenum shader_type) {
    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, 0);
    glCompileShader(shader_id);

    GLint is_compiled = false;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
    if (!is_compiled) {
        PlatformShowErrorAndExit("Could not compile shaders");
    }

    return shader_id;
}

static void
CreateOpenGLProgram() {
    GLuint vertex_shader_id = CreateAndCompileShader(vertex_shader, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = CreateAndCompileShader(fragment_shader, GL_FRAGMENT_SHADER);
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    GLint is_linked = false;
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);
    if (!is_linked) {
        PlatformShowErrorAndExit("Could not link shader program");
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

void
OpenGLInit() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CreateOpenGLProgram();
    glUseProgram(program_id);
}

Texture2D
LoadAndBindTexture(char *file_name) {
    File file = PlatformReadFile(file_name);
    void *buffer = file.buffer;
    BitmapFile *bmp = static_cast<BitmapFile *>(buffer);
    u8 *pixels = static_cast<u8 *>(buffer) + bmp->pixels_offset;
    bmp->pixels = reinterpret_cast<u32 *>(pixels);

    u32 texture_handle;
    glGenTextures(1, &texture_handle);
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, bmp->width, bmp->height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, bmp->pixels
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    Texture2D texture;
    texture.handle = texture_handle;
    texture.width = bmp->width;
    texture.height = bmp->height;
    PlatformFreeFile(file);
    return texture;
}

void
SetMatrix4Uniform(char *name, Matrix4 m) {
    u32 location = glGetUniformLocation(program_id, name);
    glUniformMatrix4fv(location, 1, GL_TRUE, m.data[0]);
}

VertexArray
MakeVertexArray(Texture2D texture, RectangleInt rect) {
    VertexArray vertex_array;

    glGenVertexArrays(1, &vertex_array.id);
    glBindVertexArray(vertex_array.id);

    glGenBuffers(1, &vertex_array.vertex_buffer_id);
    UpdateVertexBuffer(vertex_array.vertex_buffer_id, texture, rect);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void *) (2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    u32 indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    u32 element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDeleteBuffers(1, &element_buffer);
    return vertex_array;
}

void
UpdateVertexBuffer(u32 vertex_buffer_id, Texture2D texture, RectangleInt rect) {
    f32 x1 = static_cast<f32>(rect.left) / texture.width;
    f32 y1 = 1.0f - (static_cast<f32>(rect.top) / texture.height);
    f32 x2 = x1 + (static_cast<f32>(rect.width) / texture.width);
    f32 y2 = y1 - (static_cast<f32>(rect.height) / texture.height);

    f32 vertices[] = {
        -1.0f,  1.0f, x1, y1,
         1.0f,  1.0f, x2, y1,
        -1.0f, -1.0f, x1, y2,
         1.0f, -1.0f, x2, y2
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
