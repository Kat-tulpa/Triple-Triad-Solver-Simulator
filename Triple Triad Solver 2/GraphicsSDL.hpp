#pragma once
#include "defs.hpp"

namespace GraphicsSDL {
    // Text Display Settings
    static constexpr const char* FONT_PATH = "Transformers Movie.ttf";
    static constexpr SDL_Color TEXT_COLOR = Color::BLACK;
    static constexpr float FONT_SIZE = 48; // 12pt
    static TTF_Font* FONT;

    // ----------------------------------------------------------------------
    // Custom overloaded wrapper functions for readability
    // These assume things like the renderer from global scope and minimize argument variables
    static void SetRenderDrawColor(const SDL_Color color) {
        SDL_SetRenderDrawColor(RENDERER, color.r, color.g, color.b, color.a);
    }

    static void RenderClear() {
        SDL_RenderClear(RENDERER);
    }

    static void RenderTexture(SDL_Texture* texture, SDL_FRect* dstRect) {
        SDL_RenderTexture(RENDERER, texture, nullptr, dstRect);
    }

    static void RenderPresent() {
        SDL_RenderPresent(RENDERER);
    }

    static SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface) {
        return SDL_CreateTextureFromSurface(RENDERER, surface);
    }

    // End of wrapper functions ---------------------------------------------

    static void text(float x, float y, float pt, bool centered, SDL_Color color, const char* text) {
        if (strlen(text) == 0)
            std::cout << "GrpaphicsSDL::text() Tried using text with 0 length" << std::endl;

        SDL_Surface* textSurface = TTF_RenderText_Solid(FONT, text, strlen(text), color);
        if (!textSurface)
            std::cout << "GraphicsSDL::text() Failure to create surface from font. " << SDL_GetError() << std::endl;

        SDL_Texture* textTexture = CreateTextureFromSurface(textSurface);
        if (!textTexture)
            std::cout << "GraphicsSDL::text() Failure to create texture from surface. " << SDL_GetError() << std::endl;

        const int textWidth = textSurface->w;
        const int textHeight = textSurface->h;
        SDL_DestroySurface(textSurface);

        if (centered) { // Center the text
            x -= textWidth / 2.f;
            y -= textHeight / 2.f;
        }
        SDL_FRect destRect = { x, y, textWidth, textHeight };
        RenderTexture(textTexture, &destRect);
        SDL_DestroyTexture(textTexture);
    }

    static void init() {
        if (!SDL_Init(SDL_INIT_VIDEO))
            std::cout << "GraphicsSDL::init() SDL_Init failed: " << SDL_GetError() << std::endl;

        WINDOW = SDL_CreateWindow("Triple Triad Solver", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
        if (!WINDOW)
            std::cout << "GraphicsSDL::init() SDL_CreateWindow failed: " << SDL_GetError() << std::endl;

        RENDERER = SDL_CreateRenderer(WINDOW, NULL);
        if (!RENDERER)
            std::cout << "GraphicsSDL::init() SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;

        if (!TTF_Init())
            std::cout << "GraphicsSDL::init() TTF_Init failed: " << SDL_GetError() << std::endl;

        FONT = TTF_OpenFont(FONT_PATH, FONT_SIZE);
        if (!FONT)
            std::cout << "GraphicsSDL::init() TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    }

    static void eventLoop() {
        bool running = true;
        SDL_Event event;
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
            }
        }
    }

    static void cleanup() {
        TTF_CloseFont(FONT);
        SDL_DestroyRenderer(RENDERER);
        SDL_DestroyWindow(WINDOW);
        SDL_Quit();
    }
}