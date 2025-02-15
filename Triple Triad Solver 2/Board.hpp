#pragma once
#include "defs.hpp"
#include "helpers.hpp"
#include "Card.hpp"
#include "DeckStats.hpp"

struct Board {
    enum Border { TOP_BORDER = 0, RIGHT_BORDER = 2, BOTTOM_BORDER = 2, LEFT_BORDER = 0 };
    static constexpr uint8_t WIDTH = 3;
    static constexpr uint8_t HEIGHT = 3;

    std::vector<CardContainer> hand = std::vector<CardContainer>(PLAYER_COUNT);
    std::vector<ID> deckIDs = std::vector<ID>(PLAYER_COUNT);

    Player currentPlayer;
    Card cards[WIDTH][HEIGHT];

    struct PossibleMove {
        uint8_t col, row;
        ID card;

        PossibleMove(uint8_t _col, uint8_t _row, ID _card)
            : col(_col), row(_row), card(_card) {
        }
    };

    Board(ID redDeck, ID blueDeck) {
        currentPlayer = PLAYER_RED;

        // Initialize the board with empty cards
        for (int row = 0; row < HEIGHT; row++)
            for (int col = 0; col < WIDTH; col++)
                cards[col][row] = Card();
        deckIDs[PLAYER_RED] = redDeck;
        deckIDs[PLAYER_BLUE] = blueDeck;
        hand[PLAYER_RED] = DeckStats::deck(redDeck);
        hand[PLAYER_BLUE] = DeckStats::deck(blueDeck);
    }

    int hash() const {
        int h = 0;
        std::hash<int> hasher;

        // Hash board state (card IDs + controlling colors)
        for (int row = 0; row < HEIGHT; ++row)
            for (int col = 0; col < WIDTH; ++col)
                h ^= hasher(cards[row][col].id()) ^ (hasher(cards[row][col].controllingPlayer()) << 1);

        // Hash player hands (requires hands to be sorted)
        for (int player = 0; player < PLAYER_COUNT; ++player)
            for (int card = 0; card < hand[player].size(); ++card)
                h ^= hasher(hand[player][card]) << 3;

        // Hash the player to move (assuming Player is an enum)
        h ^= hasher(currentPlayer) << 5;

        return h;
    }


    void initPlayerByPickingRandomDeck(Player player) {
        const int deckID = DeckStats::getRandomID();
        deckIDs[player] = deckID;
        hand[player] = DeckStats::deck(deckID);
    }

    std::vector<PossibleMove> getAllPossibleMoves() const {
        const uint8_t handSize = hand[currentPlayer].size();
        std::vector<PossibleMove> possibleMoves;
        for (uint8_t col = 0; col < WIDTH; col++)
            for (uint8_t row = 0; row < HEIGHT; row++)
                if (isEmpty(col, row))
                    for (int i = 0; i < handSize; i++)
                        possibleMoves.emplace_back(col, row, hand[currentPlayer][i]);

        return possibleMoves;
    }

    void flip(Card& card) {
        card.setControllingPlayer(otherPlayer(card.controllingPlayer()));
    }

    void removeCardFromHand(ID cardID) {
        hand[currentPlayer].erase(
            std::remove(hand[currentPlayer].begin(), hand[currentPlayer].end(), cardID), hand[currentPlayer].end());
    }

    void sortHand() {
        std::sort(hand[currentPlayer].begin(), hand[currentPlayer].end(), std::greater<ID>());
    }

    void playCard(PossibleMove move) {
        placeCard(move.card, move.col, move.row);  // Place the card on the board
        removeCardFromHand(move.card);  // Remove the card from the hand
        sortHand(); // This should make transposition-hashing faster
        currentPlayer = otherPlayer(currentPlayer);
    }

    void placeCard(ID cardID, int col, int row) {
        Card placedCard = CardCollection::card(cardID);
        placedCard.setControllingPlayer(currentPlayer);
        cards[col][row] = placedCard;

        // Check Top For Flips
        if (row > TOP_BORDER) {
            Card& adjacentCard = cards[col][row - 1];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute(BOTTOM) < placedCard.attribute(TOP))
                flip(adjacentCard);
        }

        // Check Right For Flips
        if (col < RIGHT_BORDER) {
            Card& adjacentCard = cards[col + 1][row];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute(LEFT) < placedCard.attribute(RIGHT))
                flip(adjacentCard);
        }

        // Check Bottom For Flips
        if (row < BOTTOM_BORDER) {
            Card& adjacentCard = cards[col][row + 1];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute(TOP) < placedCard.attribute(BOTTOM))
                flip(adjacentCard);
        }

        // Check Left For Flips
        if (col > LEFT_BORDER) {
            Card& adjacentCard = cards[col - 1][row];
            if (adjacentCard.isEnemy(placedCard)
                && adjacentCard.attribute(RIGHT) < placedCard.attribute(LEFT))
                flip(adjacentCard);
        }
    }

    bool matchEnded() const {
        for (int col = 0; col < WIDTH; col++)
            for (int row = 0; row < HEIGHT; row++)
                if (cards[col][row].controllingPlayer() == PLAYER_NONE)
                    return false;
        return true; // The game is over when all spaces have been filled
    }

    bool isEmpty(int col, int row) const {
        return cards[col][row].controllingPlayer() == PLAYER_NONE;
    }

    void printColors() const {
        for (uint8_t row = 0; row < HEIGHT; row++) {
            for (uint8_t col = 0; col < WIDTH; col++) {
                std::cout << colorToChar(cards[col][row].controllingPlayer()) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    Player winningPlayer() const {
        if (!matchEnded())
            std::cout << "Error: Tried to call winningPlayer() on an unfinished game" << std::endl;

        int cardsControlled[PLAYER_COUNT] = { 0 };
        for (int col = 0; col < WIDTH; col++)
            for (int row = 0; row < HEIGHT; row++) {
                Player controllingPlayer = cards[col][row].controllingPlayer();
                if (controllingPlayer != PLAYER_NONE)
                    cardsControlled[controllingPlayer]++;
            }

        cardsControlled[PLAYER_BLUE]++; // Blue gets a handicap of 1 card for going second
        if (cardsControlled[PLAYER_RED] > cardsControlled[PLAYER_BLUE])
            return PLAYER_RED;
        if (cardsControlled[PLAYER_BLUE] > cardsControlled[PLAYER_RED])
            return PLAYER_BLUE;
        return PLAYER_NONE; // Tie
    }
};