#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

static constexpr int HAND_SIZE = 5;
static constexpr int DECK_SIZE = 5;

enum Edge {
    TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3
};

static constexpr int PLAYER_COUNT = 2;
enum Player {
    PLAYER_RED,
    PLAYER_BLUE,
    PLAYER_NONE
};

using ID = int;
using CardContainer = std::vector<ID>;

static constexpr int STRENGTH_MAX = 255;
static constexpr int STARS_MAX = 5;