#ifndef PACMAN_OPENGL_HPP
#define PACMAN_OPENGL_HPP
#include "Common.hpp"
#include "glad\glad.h"
#include "Math.hpp"


struct Texture2D {
    u32 handle;
    s32 width;
    s32 height;
};

struct VertexArray {
    u32 id;
    u32 vertex_buffer_id; // For vertices
};

void
OpenGLInit();

Texture2D
LoadAndBindTexture(char *file_name);

void
SetMatrix4Uniform(char *name, Matrix4 m);

VertexArray
MakeVertexArray(Texture2D texture, RectangleInt rect);

void
UpdateVertexBuffer(u32 vertex_buffer_id, Texture2D texture, RectangleInt rect);

#endif // PACMAN_OPENGL_HPP
