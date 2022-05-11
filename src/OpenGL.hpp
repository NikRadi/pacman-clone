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
OpenGLCreateProgram();

Texture2D
OpenGLLoadAndBindTexture(char *file_name);

void
SetMatrix4Uniform(u32 location, Matrix4 m);

void
SetMatrix4Uniform(u32 program_id, char *name, Matrix4 m);

#endif // PACMAN_OPENGL_HPP
