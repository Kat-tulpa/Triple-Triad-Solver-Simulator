#pragma once
#include "TextureCache.hpp"
#include "Graphics.hpp"
#include "Attributes.hpp"

struct RenderableCardContainer {
    enum Location {
        TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, CENTER
    };

    CardGrid cards;
    float xStart, yStart, drawWidth, drawHeight;
    int width, height;
    bool drawEmpty;

    // From CardContainer
    RenderableCardContainer(int width, int height, Location _location, Player _player, const CardContainer _cards)
        : width(width), height(height), xStart(0), yStart(0) {

        cards = cardsToGrid(width, height, filledWithEmpty(idsToCards(_cards), HAND_SIZE));
        setControllingPlayerOn(cards, _player);
        init(_location);
    }

    // From std::vector<Card>
    RenderableCardContainer(int _width, int _height, Location _location, const std::vector<Card> _cards)
        : width(_width), height(_height), xStart(0), yStart(0) {

        // In the case of hands less than full, fill them
        const std::vector<Card> filledCards; // = filledWithEmpty(_cards, HAND_SIZE);
        cards = cardsToGrid(width, height, filledCards);
        init(_location);
    }

    // From CardGrid
    RenderableCardContainer(int width, int height, Location _location, const CardGrid _cards)
        : width(width), height(height), cards(_cards), xStart(0), yStart(0) {

        init(_location);
    }

    inline static std::vector<RenderableCardContainer> renderables;

    void init(Location location) {
        drawWidth = TextureCache::CARD_WIDTH * width;
        drawHeight = TextureCache::CARD_HEIGHT * height;
        drawEmpty = false;

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
            drawEmpty = true;
            break;
        }
    }

    // For Manipulating Object Data -----------------------------------------------------

    static void setControllingPlayerOn(CardGrid& cards, Player player) {
        for (int i = 0; i < cards.size(); i++) // Iterate over rows
            for (int j = 0; j < cards[i].size(); j++) // Iterate over columns in the row
                cards[i][j].setControllingPlayer(player);
    }

    static std::vector<Card> filledWithEmpty(std::vector<Card> cards, int fillTo) {
        while (cards.size() < fillTo)
            cards.push_back(CardCollection::getEmpty());
        return cards;
    }

    static CardGrid cardsToGrid(int width, int height, const std::vector<Card> cards) {
        CardGrid grid;
        int cardIndex = 0;
        for (int row = 0; row < width; row++) {  // Width is rows
            std::vector<Card> gridLine;
            for (int col = 0; col < height; col++) {  // Height is columns
                if (cardIndex < cards.size()) {
                    gridLine.push_back(cards[cardIndex]);
                    cardIndex++;
                }
            }
            grid.push_back(gridLine);
        }

        return grid;
    }

    static std::vector<Card> idsToCards(const CardContainer ids) {
        std::vector<Card> cards;
        cards.reserve(ids.size());
        for (int i = 0; i < ids.size(); i++)
            cards.push_back(CardCollection::card(ids[i]));

        return cards;
    }

    // For Drawing ---------------------------------------------------------------------

    static float cardXCenter(float x) {
        return x + TextureCache::CARD_HALF_WIDTH;
    }

    static float cardYCenter(float y) {
        return y + TextureCache::CARD_HALF_HEIGHT;
    }

    // Draws the background based on the controlling player
    static void drawCardBackground(float x, float y, const Player player) {
        Graphics::image(x, y, TextureCache::COLORED_BACKING[player]);
    }

    static void drawName(float x, float y, std::string name) {
        return; // TODO: Implement
    }

    static void drawCardStars(float x, float y, int stars) {
        // Give a little bit of air padding
        x += (TextureCache::CARD_WIDTH * 0.085f);
        y += (TextureCache::CARD_HEIGHT * 0.07f);
        if (!TextureCache::rarityTextures[stars - 1])
            std::cout << "Error: Attempted to render an image with an invalid texture in Graphics::image() " << std::endl;
        SDL_FRect destRect = { x, y, TextureCache::RARITY_WIDTH, TextureCache::RARITY_HEIGHT };
        SDL_RenderTexture(RENDERER, TextureCache::rarityTextures[stars - 1], nullptr, &destRect);
    }

    static void drawCardAttributes(float x, float y, Card card) {
        Attributes::draw(cardXCenter(x), cardYCenter(y), card.attributes());
    }

    static void drawCardImage(float x, float y, ID id) {
        Graphics::image(x, y, id);
    }

    void draw() const {
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                float x = xStart + (TextureCache::CARD_WIDTH * col);
                float y = yStart + (TextureCache::CARD_HEIGHT * row);

                drawCardBackground(x, y, cards[col][row].controllingPlayer());
                if (cards[col][row].id() == EMPTY_CARD_ID)
                    continue;

                drawCardImage(x, y, cards[col][row].id());
                drawCardAttributes(x, y, cards[col][row]);
                drawCardStars(x, y, cards[col][row].stars());
            }
        }
    }

    static void addAllBoardElementsToRenderingList() {
        // The board
        auto board = RenderableCardContainer(Board::WIDTH, Board::HEIGHT, CENTER, Board::cards);
        renderables.push_back(board);

        // Both Player's Decks and Hands
        for (int player = 0; player < PLAYER_COUNT; player++) {
            Location deckLocation, handLocation;

            switch (player) {
            case PLAYER_RED:
                deckLocation = TOP_LEFT;
                handLocation = BOTTOM_LEFT;
                break;
            case PLAYER_BLUE:
                deckLocation = TOP_RIGHT;
                handLocation = BOTTOM_RIGHT;
                break;
            }

            // Get the player's hand and ensure it has the correct number of cards
            auto deck = RenderableCardContainer(DECK_SIZE, 1, deckLocation, Player(player), DeckStats::deck(Board::deck[player]));
            auto hand = RenderableCardContainer(HAND_SIZE, 1, handLocation, Player(player), Board::hand[player]);
            renderables.push_back(deck);
            renderables.push_back(hand);
        }
    }

    static void drawGame() {
        GraphicsSDL::RenderClear();
        Graphics::background();

        renderables.clear();
        addAllBoardElementsToRenderingList();

        // Draw elements
        for (auto& renderable : renderables)
            renderable.draw();
    }
};