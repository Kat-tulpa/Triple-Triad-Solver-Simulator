#pragma once
#include "GraphicsSDL.hpp"
#include "CardCollection.hpp"

namespace TextureCache {
    static constexpr const char* MISSING_TEXTURE_FILEPATH = "MISSING CARD.png";
    static constexpr float SCALE_FACTOR = 2.f; // Scale card images to 2x size
    static constexpr float CARD_WIDTH = 100.f * SCALE_FACTOR;
    static constexpr float CARD_HEIGHT = 126.f * SCALE_FACTOR;
    static constexpr float CARD_HALF_WIDTH = CARD_WIDTH / 2.f;
    static constexpr float CARD_HALF_HEIGHT = CARD_HEIGHT / 2.f;
    static constexpr float RARITY_WIDTH = 27.f * SCALE_FACTOR;
    static constexpr float RARITY_HEIGHT = 26.f * SCALE_FACTOR;

    inline static std::vector<SDL_Texture*> cardTextures;
    inline static std::vector<SDL_Texture*> rarityTextures;
    inline static std::vector<bool> textureFound;
    inline static ID COLORED_BACKING[PLAYER_COUNT + 1];

    static void textureAddRarity(const std::string& filePath) {
        SDL_Texture* newTexture = IMG_LoadTexture(RENDERER, filePath.c_str());
        if (!newTexture) {
            std::cout << "Error: Failed to load texture in TextureCache::textureAddRarity() " << SDL_GetError() << std::endl;
            newTexture = IMG_LoadTexture(RENDERER, MISSING_TEXTURE_FILEPATH);
        }

        rarityTextures.push_back(newTexture);
    }

    static ID textureAddCard(const std::string& filePath) {
        SDL_Texture* newTexture = IMG_LoadTexture(RENDERER, filePath.c_str());
        bool textureMissing = false;
        if (!newTexture) {
            std::cout << "Error: Failed to load texture in TextureCache::textureAddCard() " << SDL_GetError() << std::endl;
            newTexture = IMG_LoadTexture(RENDERER, MISSING_TEXTURE_FILEPATH);
            textureMissing = true;
            if (!newTexture)
                std::cout << "Error: Also failed to load missing texture in TextureCache::textureAddCard() " << SDL_GetError() << std::endl;
        }

        const ID index = cardTextures.size();
        if (!textureMissing)
            cardTextures.emplace_back(newTexture);
        else
            cardTextures.emplace_back(cardTextures[0]);
        return index;
    }

    static void textureClear(SDL_Texture* texture) {
        if (!texture)
            std::cout << "Error: tried to clear invalid texture in TextureCache::textureClear()" << std::endl;
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    static void clear() {
        for (auto& image : cardTextures)
            textureClear(image);
        cardTextures.clear();
    }

    // Draw an image from the pool using it's ID
    static void draw(ID imgID, float x, float y) {
        if (imgID < 0 || imgID >= cardTextures.size() || !cardTextures[imgID])
            std::cout << "Error: Image ID out of bounds or texture not loaded in TextureCache::draw()" << std::endl;

        SDL_Texture* texture = cardTextures[imgID];
        float imgWidth, imgHeight;
        SDL_GetTextureSize(texture, &imgWidth, &imgHeight);
        SDL_FRect dstRect = { x, y, imgWidth, imgHeight };
        GraphicsSDL::RenderTexture(texture, &dstRect);
    }

    static void init() {
        textureAddRarity("rarity1.png");
        textureAddRarity("rarity2.png");
        textureAddRarity("rarity3.png");
        textureAddRarity("rarity4.png");
        textureAddRarity("rarity5.png");

        for (int i = 0; i < CardCollection::cardCount(); i++) {
            const ID id = textureAddCard(CardCollection::name(i) + ".png");
            if (id != i)
                std::cout << "Error: TextureCache entry ID didn't match the Card ID. Name was: " << CardCollection::name(i) << std::endl;
        }
        COLORED_BACKING[PLAYER_RED] = textureAddCard("RED CARD.png");
        COLORED_BACKING[PLAYER_BLUE] = textureAddCard("BLUE CARD.png");
        COLORED_BACKING[PLAYER_NONE] = textureAddCard("EMPTY CARD.png");
    }
}