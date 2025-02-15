#pragma once
#include "GraphicsSDL.hpp"
#include "CardCollection.hpp"
#include "Board.hpp"
#include "helpers.hpp"

/* For a simple graphics abstraction layer for SDL, providing basic
drawing utilities to separate rendering logic from game logic */
namespace Graphics {
    static float LINE_THICKNESS, MARGIN;

    // A global allocation buffer which holds and manages all image textures
    namespace TextureCache {
        static constexpr const char* MISSING_TEXTURE_FILEPATH = "MISSING CARD.png";
        static constexpr float SCALE_FACTOR = 2.f; // Scale card images to 2x size
        static constexpr float CARD_WIDTH = 100.f * SCALE_FACTOR;
        static constexpr float CARD_HEIGHT = 126.f * SCALE_FACTOR;
        static constexpr float CARD_HALF_WIDTH = CARD_WIDTH / 2.f;
        static constexpr float CARD_HALF_HEIGHT = CARD_HEIGHT / 2.f;

        static std::vector<SDL_Texture*> cardTextures;
        static std::vector<bool> textureFound;
        static ID COLORED_BACKING[PLAYER_COUNT + 1];

        static void textureAddCard(const std::string& filePath) {
            SDL_Texture* newTexture = IMG_LoadTexture(RENDERER, filePath.c_str());
            bool textureMissing = false;
            if (!newTexture) {
                std::cout << "Error: Failed to load texture in TextureCache::textureAddCard()" << SDL_GetError() << std::endl;
                newTexture = IMG_LoadTexture(RENDERER, MISSING_TEXTURE_FILEPATH);
                textureMissing = true;
                if (!newTexture)
                    std::cout << "Error: Also failed to load missing texture in TextureCache::textureAddCard()" << SDL_GetError() << std::endl;
            }

            textureFound.push_back(!textureMissing);
            cardTextures.emplace_back(newTexture);
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
            for (int i = 0; i < CardCollection::cardCount(); i++)
                textureAddCard(CardCollection::name(i) + ".png");
            COLORED_BACKING[PLAYER_RED] = cardTextures.size();
            textureAddCard("RED CARD");
            COLORED_BACKING[PLAYER_BLUE] = cardTextures.size();
            textureAddCard("BLUE CARD");
            COLORED_BACKING[PLAYER_NONE] = 0;
        }
    }

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

    // String Version
    static void textCentered(float x, float y, float pt, SDL_Color color, std::string& str) {
        GraphicsSDL::text(x, y, pt * TextureCache::SCALE_FACTOR, true, color, str.c_str());
    }

    // Single Char Version
    static void textCentered(const float x, const float y, const float pt, SDL_Color color, char ch) {
        GraphicsSDL::text(x, y, pt * TextureCache::SCALE_FACTOR, true, color, &ch);
    }

    namespace Attributes {
        static constexpr float OFFSET_MULTIPLIER = 0.6f; // 60% of the way to each edge
        static constexpr float VERTICAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_HEIGHT;
        static constexpr float HORIZONTAL_OFFSET = OFFSET_MULTIPLIER * TextureCache::CARD_HALF_WIDTH;

        static void draw(const Card& card, float x, float y) {
            if (card.controllingPlayer() != PLAYER_NONE)
                for (int edge = 0; edge < 4; edge++) {
                    float xAfterOffset = x;
                    float yAfterOffset = y;

                    // Apply vertical position modifications to vertical edges
                    if (edge == Edge::TOP)
                        yAfterOffset -= VERTICAL_OFFSET;
                    else if (edge == Edge::BOTTOM)
                        yAfterOffset += VERTICAL_OFFSET;
                    else if (edge == Edge::LEFT)
                        xAfterOffset -= HORIZONTAL_OFFSET;
                    else if (edge == Edge::RIGHT)
                        xAfterOffset += HORIZONTAL_OFFSET;

                    char c = attributeToChar(card.attribute(edge));
                    textCentered(xAfterOffset, yAfterOffset, GraphicsSDL::FONT_SIZE, Color::WHITE, c);
                }
        }
    };

    struct RenderableCardContainer {
        typedef std::vector<std::vector<Card>> CardGrid;

        enum Location {
            TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER
        };

        Location location;
        CardGrid cards;
        float xStart, yStart, drawWidth, drawHeight;
        int colCount, rowCount;

        RenderableCardContainer(Location _location, CardGrid _cards)
            : location(_location), cards(_cards) {

            rowCount = cards.at(0).size();
            colCount = cards.size();

            drawWidth = TextureCache::CARD_WIDTH * rowCount;
            drawHeight = TextureCache::CARD_HEIGHT * colCount;

            xStart = 0; yStart = 0; // TOP_LEFT case
            switch (location) {
            case TOP_RIGHT:
                xStart = WINDOW_WIDTH - drawWidth;
                yStart = 0;
                break;
            case BOTTOM_LEFT:
                xStart = MARGIN;
                yStart = WINDOW_HEIGHT - drawHeight;
                break;
            case BOTTOM_RIGHT:
                xStart = WINDOW_WIDTH - drawWidth;
                yStart = WINDOW_HEIGHT - drawHeight;
                break;
            case CENTER:
                xStart = (WINDOW_WIDTH / 2.f) - (drawWidth / 2.f);
                yStart = (WINDOW_HEIGHT / 2.f) - (drawHeight / 2.f);
                break;
            }
        }

        static std::vector<RenderableCardContainer> renderables;

        void draw() {
            for (int col = 0; col < colCount; ++col) {
                for (int row = 0; row < rowCount; ++row) {
                    float cardX = xStart + (row * TextureCache::CARD_WIDTH);
                    float cardY = yStart + (col * TextureCache::CARD_HEIGHT);

                    // Draw Backing
                    image(TextureCache::COLORED_BACKING[cards[col][row].controllingPlayer()], cardX, cardY);
                    // Draw Card
                    image(cards[col][row].id(), cardX, cardY);

                    // Calculate center position for attribute drawing
                    const float xCenter = cardX + TextureCache::CARD_HALF_WIDTH;
                    const float yCenter = cardY + TextureCache::CARD_HALF_HEIGHT;

                    // Draw the card image and its attributes
                    image(cards[col][row].id(), cardX, cardY);
                    Attributes::draw(cards[col][row], xCenter, yCenter);
                }
            }
        }

        static void addBoardCardsAsRenderable(const Board board) {
            CardGrid cards(Board::WIDTH, std::vector<Card>(Board::HEIGHT));
            for (int row = 0; row < Board::WIDTH; row++)
                for (int col = 0; col < Board::HEIGHT; col++)
                    cards[col][row] = board.cards[col][row];
            renderables.push_back(RenderableCardContainer(CENTER, cards));
        }

        static void addHandAsRenderable(CardContainer hand, Player player) {
            CardGrid cards(HAND_SIZE, std::vector<Card>(1));
            while (hand.size() < HAND_SIZE)
                hand.push_back(0);

            for (int i = 0; i < HAND_SIZE; i++) {
                Card card = CardCollection::card(i);
                card.setControllingPlayer(player);
                cards[i][0] = card;
            }

            Location location;
            if (player == PLAYER_RED)
                location = Location::BOTTOM_LEFT;
            else
                location = Location::BOTTOM_RIGHT;
            renderables.emplace_back(location, cards);
        }

        static void addDeckAsRenderable(const ID& redDeckID, const ID& blueDeckID) {
            CardGrid redCards(DECK_SIZE, std::vector<Card>(1));
            CardGrid blueCards(DECK_SIZE, std::vector<Card>(1));
            for (int i = 0; i < DECK_SIZE; i++) {
                redCards[i][0] = CardCollection::cardFromID(DeckStats::deck(redDeckID)[i]);
                blueCards[i][0] = CardCollection::cardFromID(DeckStats::deck(blueDeckID)[i]);
            }
            renderables.push_back(RenderableCardContainer(TOP_LEFT, redCards));
            renderables.push_back(RenderableCardContainer(TOP_RIGHT, blueCards));
        }

        static void drawGame(const Board& board) {
            GraphicsSDL::RenderClear();
            Graphics::background();

            renderables.clear();
            addBoardCardsAsRenderable(board);
            addHandAsRenderable(board.hand[PLAYER_RED], PLAYER_RED);
            addHandAsRenderable(board.hand[PLAYER_BLUE], PLAYER_RED);
            addDeckAsRenderable(board.deckIDs[PLAYER_RED], board.deckIDs[PLAYER_BLUE]);

            // Draw elements
            for (auto& renderable : renderables)
                renderable.draw();
        }
    };

    static void init() {
        GraphicsSDL::init();
        TextureCache::init();
    }

    static void cleanup() {
        TextureCache::clear();
        GraphicsSDL::cleanup();
    }
}