#ifndef PACMAN_COMMON_HPP
#define PACMAN_COMMON_HPP
#include <stdint.h>


#define PACMAN_DEBUG

#ifdef PACMAN_DEBUG
    #define ASSERT(expression)  \
        if(!(expression)) {     \
            *(int *) 0 = 0;     \
        }

#else
    #define ASSERT(expression)
#endif // PACMAN_DEBUG

#define INVALID_CODE_PATH ASSERT(!"InvalidCodePath")


typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#endif // PACMAN_COMMON_HPP