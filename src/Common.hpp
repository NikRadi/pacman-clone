#ifndef PACMAN_COMMON_HPP
#define PACMAN_COMMON_HPP

#define PACMAN_DEBUG

#ifdef PACMAN_DEBUG
    #define ASSERT(expr) if (!(expr)) { *(int *) 0 = 0; }
#else
    #define ASSERT(expr)
#endif // PACMAN_DEBUG

#define NOT_IMPLEMENTED ASSERT(0)


typedef signed char        s8;
typedef short              s16;
typedef int                s32;
typedef long long          s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

#endif // PACMAN_COMMON_HPP
