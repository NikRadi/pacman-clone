#ifndef PACMAN_GAME_HPP
#define PACMAN_GAME_HPP
#include "Common.hpp"
#include "Platform.hpp"


void
GameInit(s32 window_width, s32 window_height);

void
GameUpdate(f32 delta_time, Input input);

#endif // PACMAN_GAME_HPP
