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


u32
CreateOpenGLProgram();

Texture2D
LoadAndBindTexture(char *file_name);

void
SetMatrix4Uniform(u32 location, Matrix4 m);

void
SetMatrix4Uniform(u32 program_id, char *name, Matrix4 m);

u32
InitVAO(Texture2D texture, RectangleInt rect);

#endif // PACMAN_OPENGL_HPP
