#pragma once
#include "helpers.hpp"
#include "GraphicsSDL.hpp"
#include "CardCollection.hpp"
#include "Board.hpp"
#include "TextureCache.hpp"

/* For a simple graphics abstraction layer for SDL, providing basic
drawing utilities to separate rendering logic from game logic */
namespace Graphics {
    static float LINE_THICKNESS, MARGIN;

    static void background() {
        GraphicsSDL::SetRenderDrawColor(Color::BLACK);
    }

    static void line(float x1, float y1, float x2, float y2) {
        GraphicsSDL::SetRenderDrawColor(Color::WHITE);
        SDL_FRect rect = { x1, y1, x2 - x1, y2 - y1 };
        SDL_RenderFillRect(RENDERER, &rect);
    }

    static void image(const ID imageID, const float x, const float y) {
        if (!TextureCache::cardTextures[imageID])
            std::cout << "Error: Attempted to render an image with an invalid texture in Graphics::image()" << std::endl;
        SDL_FRect destRect = { x, y, TextureCache::CARD_WIDTH, TextureCache::CARD_HEIGHT };
        SDL_RenderTexture(RENDERER, TextureCache::cardTextures[imageID], nullptr, &destRect);
    }

    static void init() {
        GraphicsSDL::init();
        TextureCache::init();
    }

    static void cleanup() {
        TextureCache::clear();
        GraphicsSDL::cleanup();
    }
}