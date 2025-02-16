#pragma once
#include "defs.hpp"
#include <iostream>

namespace Color {
    static constexpr SDL_Color WHITE = { 255, 255, 255, 255 };
    static constexpr SDL_Color LIGHT_GRAY = { 211, 211, 211, 255 };
    static constexpr SDL_Color DARK_GRAY = { 170, 170, 170, 255 };
    static constexpr SDL_Color BLUE = { 55, 55, 175, 255 };
    static constexpr SDL_Color BLACK = { 0, 0, 0, 255 };
    static constexpr SDL_Color RED = { 175, 55, 55, 255 };
    static constexpr SDL_Color GREEN = { 100, 255, 0, 255 };
}

static SDL_Window* WINDOW = nullptr;
static SDL_Renderer* RENDERER = nullptr;
static constexpr int WINDOW_WIDTH = 3440;
static constexpr int WINDOW_HEIGHT = 1440;

namespace GraphicsSDL {
    // Text Display Settings
    static constexpr const char* FONT_PATH = "Transformers Movie.ttf";
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
    char* stringToCharArray(const std::string& str) {
        size_t length = str.length() + 1; // +1 for null terminator
        char* charArray = new char[length]; // Allocate memory

        if (strcpy_s(charArray, length, str.c_str()) != 0) { // Safe copy
            delete[] charArray; // Clean up if copy fails
            return nullptr;
        }

        return charArray;
    }

    static SDL_Texture* createTextTexture(std::string textString, SDL_Color color, TTF_Font* font, int& textWidth, int& textHeight) {
        if (textString.empty()) {
            std::cout << "GraphicsSDL::text() Tried using text with 0 length" << std::endl;
            return nullptr;
        }

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, stringToCharArray(textString), textString.size(), color);
        if (!textSurface) {
            std::cout << "GraphicsSDL::text() Failure to create surface from font. " << SDL_GetError() << std::endl;
            return nullptr;
        }

        textWidth = textSurface->w;
        textHeight = textSurface->h;

        SDL_Texture* textTexture = CreateTextureFromSurface(textSurface);
        SDL_DestroySurface(textSurface);

        if (!textTexture)
            std::cout << "GraphicsSDL::text() Failure to create texture from surface. " << SDL_GetError() << std::endl;

        return textTexture;
    }

    static void renderText(float x, float y, bool centered, SDL_Texture* textTexture, int textWidth, int textHeight) {
        if (!textTexture) return;

        if (centered) {
            x -= textWidth / 2.f;
            y -= textHeight / 2.f;
        }

        SDL_FRect destRect = { x, y, (float)textWidth, (float)textHeight };
        RenderTexture(textTexture, &destRect);
    }

    static void text(float x, float y, float pt, bool centered, SDL_Color color, std::string str, TTF_Font* font) {
        int textWidth, textHeight;
        SDL_Texture* textTexture = createTextTexture(str, color, font, textWidth, textHeight);
        renderText(x, y, centered, textTexture, textWidth, textHeight);
        SDL_DestroyTexture(textTexture); // Destroy only after both render calls are done
    }

    std::vector<std::string> splitIntoWords(const std::string& text) {
        std::vector<std::string> words;
        std::string currentWord;

        for (char c : text) {
            if (c == ' ') {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
            }
            else {
                currentWord += c;
            }
        }

        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }

        return words;
    }

    static std::vector<std::string> wrapText(TTF_Font* font, float pt, const std::string& text, int maxWidth) {
        const std::vector<std::string> tokenWords = splitIntoWords(text);

        std::vector<std::string> wrappedLines;
        std::string line;
        for (int i = 0; i < tokenWords.size(); i++) {
            if (((line.size() + tokenWords[i].size()) * pt) <= maxWidth) {
                line += tokenWords[i];
            }
            else {
                wrappedLines.push_back(line);
                line.clear();
                line = tokenWords[i];
            }
        }

        return wrappedLines;
    }

    static void drawTextWithBackground(float x, float y, float pt, std::string textString, int maxWidth) {
        SDL_Color bgColor = { 0, 0, 0, 255 };  // Black background
        SDL_Color fgColor = { 255, 255, 255, 255 };  // White foreground

        TTF_Font* bigFont = TTF_OpenFont(FONT_PATH, pt * 1.2f);  // Background font
        TTF_Font* smallFont = TTF_OpenFont(FONT_PATH, pt * 1.f);  // Foreground font

        if (!bigFont || !smallFont) {
            std::cout << "Failed to load fonts!" << std::endl;
            return;
        }

        std::vector<std::string> lines = wrapText(smallFont, pt, textString, maxWidth);

        int lineHeight = pt;
        float yOffset = 0;

        for (const std::string& line : lines) {
            text(x, y + yOffset, pt * 1.2f, true, bgColor, line, bigFont);  // Background text
            text(x, y + yOffset, pt * 1.f, true, fgColor, line, smallFont);  // Foreground text
            yOffset += lineHeight;  // Move to the next line
        }

        TTF_CloseFont(bigFont);
        TTF_CloseFont(smallFont);
    }

    static void drawTextWithBackground(float x, float y, float pt, std::string textString) {
        SDL_Color bgColor = { 0, 0, 0, 255 };  // Black background
        SDL_Color fgColor = { 255, 255, 255, 255 };  // White foreground

        TTF_Font* bigFont = TTF_OpenFont(FONT_PATH, pt * 1.2f);  // Background font
        TTF_Font* smallFont = TTF_OpenFont(FONT_PATH, pt * 1.f);  // Foreground font

        if (!bigFont || !smallFont) {
            std::cout << "Failed to load fonts!" << std::endl;
            return;
        }

        text(x, y, pt * 1.2f, true, bgColor, textString, bigFont);  // Background text
        text(x, y, pt * 1.f, true, fgColor, textString, smallFont);  // Foreground text

        TTF_CloseFont(bigFont);
        TTF_CloseFont(smallFont);
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