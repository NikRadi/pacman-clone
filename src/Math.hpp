#ifndef PACMAN_MATH_HPP
#define PACMAN_MATH_HPP
#include "Common.hpp"


struct Vector2 {
    f32 x;
    f32 y;
};

struct Vector2Int {
    s32 x;
    s32 y;
};

struct Matrix4 {
    f32 data[4][4]; // Row-major: matrix[row][col]
};

struct RectangleInt {
    s32 left;
    s32 top;
    s32 width;
    s32 height;
};


constexpr Matrix4 IDENDITY_MATRIX4 = { 1.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 1.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 1.0f };


Matrix4
Orthographic(f32 left, f32 right, f32 bottom, f32 top);

Matrix4
Scale(Matrix4 matrix, Vector2 scale);

Matrix4
Translate(Matrix4 matrix, Vector2 translate);

f32
Abs(f32 a);

Vector2
operator+(Vector2 a, Vector2 b);

Vector2
operator-(Vector2 a, Vector2 b);

Vector2
operator*(Vector2 a, Vector2 b);

bool
operator==(Vector2 a, Vector2 b);

Vector2
operator*(Vector2Int a, Vector2 b);

Vector2
operator*(Vector2 a, f32 b);

Vector2
operator*(f32 a, Vector2 b);

Vector2Int
operator-(Vector2Int a, Vector2Int b);

bool
operator==(Vector2Int a, Vector2Int b);

bool
operator!=(Vector2Int a, Vector2Int b);

#endif // PACMAN_MATH_HPP
