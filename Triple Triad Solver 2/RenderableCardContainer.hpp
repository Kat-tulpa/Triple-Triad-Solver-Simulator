#pragma once
#include "TextureCache.hpp"
#include "Graphics.hpp"
#include "Attributes.hpp"

class RenderableCardContainer {
private:
    typedef std::vector<std::vector<Card>> CardGrid;

    enum Location {
        TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER
    };

    inline static std::vector<RenderableCardContainer> renderables;
    Location location;
    CardGrid cards;
    float xStart, yStart, drawWidth, drawHeight;
    int colCount, rowCount;

public:
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
            xStart = Graphics::MARGIN;
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

    void draw() {
        for (int col = 0; col < colCount; ++col) {
            for (int row = 0; row < rowCount; ++row) {
                float cardX = xStart + (row * TextureCache::CARD_WIDTH);
                float cardY = yStart + (col * TextureCache::CARD_HEIGHT);
                const Card& card = cards[col][row];
                bool coloredOwner = true;
                // Draw Backing
                ID colorID;
                if (card.controllingPlayer() == PLAYER_RED)
                    colorID = CardCollection::getIDfromName("RED CARD");
                else if (card.controllingPlayer() == PLAYER_BLUE)
                    colorID = CardCollection::getIDfromName("BLUE CARD");
                else {
                    colorID = CardCollection::getIDfromName("EMPTY CARD");
                    coloredOwner = false;
                }

                Graphics::image(colorID, cardX, cardY);
                // Draw Card
                if (TextureCache::cardTextures[card.id()] != TextureCache::cardTextures[0])
                    Graphics::image(card.id(), cardX, cardY);

                // Calculate center position for attribute drawing
                const float xCenter = cardX + TextureCache::CARD_HALF_WIDTH;
                const float yCenter = cardY + TextureCache::CARD_HALF_HEIGHT;

                if (coloredOwner) {
                    Attributes::draw(card.attributes(), xCenter, yCenter);
                    GraphicsSDL::drawTextWithBackground(xCenter, yCenter - (TextureCache::CARD_HALF_HEIGHT * 0.8f), 36, CardCollection::name(card.id()), TextureCache::CARD_WIDTH);
                }
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
        CardGrid cards(1, std::vector<Card>(HAND_SIZE));
        while (hand.size() < HAND_SIZE)
            hand.push_back(0);

        for (int i = 0; i < HAND_SIZE; i++) {
            Card handCard = CardCollection::card(hand[i]);
            handCard.setControllingPlayer(player);
            cards[0][i] = handCard;
        }

        Location location;
        if (player == PLAYER_RED)
            location = Location::BOTTOM_LEFT;
        else
            location = Location::BOTTOM_RIGHT;
        renderables.emplace_back(location, cards);
    }

    static void addDecksAsRenderable(const ID redDeckID, const ID blueDeckID) {
        CardGrid redCards(1, std::vector<Card>(DECK_SIZE));
        CardGrid blueCards(1, std::vector<Card>(DECK_SIZE));
        CardContainer redDeck = DeckStats::deck(redDeckID);
        CardContainer blueDeck = DeckStats::deck(blueDeckID);
        for (int i = 0; i < DECK_SIZE; i++) {
            Card redCard = CardCollection::card(redDeck[i]);
            redCard.setControllingPlayer(PLAYER_RED);
            Card blueCard = CardCollection::card(blueDeck[i]);
            blueCard.setControllingPlayer(PLAYER_BLUE);
            redCards[0][i] = redCard;
            blueCards[0][i] = blueCard;
        }
        renderables.emplace_back(TOP_LEFT, redCards);
        renderables.emplace_back(TOP_RIGHT, blueCards);
    }

    static void drawGame(const Board board) {
        GraphicsSDL::RenderClear();
        Graphics::background();

        renderables.clear();
        addBoardCardsAsRenderable(board);
        addHandAsRenderable(board.hand[PLAYER_RED], PLAYER_RED);
        addHandAsRenderable(board.hand[PLAYER_BLUE], PLAYER_BLUE);
        addDecksAsRenderable(board.deckIDs[PLAYER_RED], board.deckIDs[PLAYER_BLUE]);

        // Draw elements
        for (auto& renderable : renderables)
            renderable.draw();
    }
};