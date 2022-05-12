#include "Math.hpp"


Matrix4
Orthographic(f32 left, f32 right, f32 bottom, f32 top) {
    Matrix4 result = IDENDITY_MATRIX4;
    f32 right_left_diff = right - left;
    f32 top_bottom_diff = top - bottom;

    result.data[0][0] = 2 / right_left_diff;
    result.data[1][1] = 2 / top_bottom_diff;

    result.data[0][3] = -(right + left) / right_left_diff;
    result.data[1][3] = -(top + bottom) / top_bottom_diff;

    return result;
}

Matrix4
Scale(Matrix4 matrix, Vector2 scale) {
    matrix.data[0][0] *= scale.x;
    matrix.data[1][1] *= scale.y;
    return matrix;
}

Matrix4
Translate(Matrix4 matrix, Vector2 translate) {
    matrix.data[0][3] += translate.x;
    matrix.data[1][3] += translate.y;
    return matrix;
}

f32
Abs(f32 a) {
    return (a > 0) ? a : -a;
}

Vector2
operator+(Vector2 a, Vector2 b) {
    return { a.x + b.x, a.y + b.y };
}

Vector2
operator*(Vector2 a, Vector2 b) {
    return { a.x * b.x, a.y * b.y };
}

Vector2
operator*(Vector2Int a, Vector2 b) {
    return { a.x * b.x, a.y * b.y };
}

Vector2
operator*(Vector2 a, f32 b) {
    return { a.x * b, a.y * b };
}

Vector2
operator*(f32 a, Vector2 b) {
    return b * a;
}

bool
operator==(Vector2Int a, Vector2Int b) {
    return a.x == b.x && a.y == b.y;
}
