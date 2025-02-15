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

static constexpr uint8_t PLAYER_COUNT = 2;
enum Player {
    PLAYER_RED,
    PLAYER_BLUE,
    PLAYER_NONE
};

using ID = int;
using CardContainer = std::vector<ID>;

static constexpr int STRENGTH_MAX = 255;

static SDL_Window* WINDOW = nullptr;
static SDL_Renderer* RENDERER = nullptr;
static constexpr int WINDOW_WIDTH = 3440;
static constexpr int WINDOW_HEIGHT = 1440;

namespace Color {
    static constexpr SDL_Color WHITE = { 255, 255, 255, 255 };
    static constexpr SDL_Color LIGHT_GRAY = { 211, 211, 211, 255 };
    static constexpr SDL_Color DARK_GRAY = { 170, 170, 170, 255 };
    static constexpr SDL_Color BLUE = { 55, 55, 175, 255 };
    static constexpr SDL_Color BLACK = { 0, 0, 0, 255 };
    static constexpr SDL_Color RED = { 175, 55, 55, 255 };
    static constexpr SDL_Color GREEN = { 100, 255, 0, 255 };
}