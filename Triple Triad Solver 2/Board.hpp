#pragma once
#include "defs.hpp"
#include "helpers.hpp"
#include "Card.hpp"
#include "DeckStats.hpp"
#include "PossibleMove.hpp"
#include "MoveHistory.hpp"
#include "CardGrid.hpp"

namespace Board {
    enum Border { TOP_BORDER = 0, RIGHT_BORDER = 2, BOTTOM_BORDER = 2, LEFT_BORDER = 0 };
    static constexpr int WIDTH = 3;
    static constexpr int HEIGHT = 3;
    static CardContainer hand[PLAYER_COUNT];
    static ID deck[PLAYER_COUNT];
    static CardGrid cards;

    static Player currentPlayer;

    static void init(ID redDeck, ID blueDeck) {
        deck[PLAYER_RED] = redDeck; 
        deck[PLAYER_BLUE] = blueDeck;
        hand[PLAYER_RED] = DeckStats::deck(redDeck);
        hand[PLAYER_BLUE] = DeckStats::deck(blueDeck);

        initCardGrid(cards, WIDTH, HEIGHT);
        for (int row = 0; row < HEIGHT; row++)
            for (int col = 0; col < WIDTH; col++)
                cards[col][row] = CardCollection::getEmpty();

        currentPlayer = PLAYER_RED;
    }

    static int hash() {
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

    static bool isEmpty(int col, int row) {
        return cards[col][row].controllingPlayer() == PLAYER_NONE;
    }

    static std::vector<PossibleMove> getAllPossibleMoves()  {
        const int handSize = hand[currentPlayer].size();
        std::vector<PossibleMove> possibleMoves;
        for (int col = 0; col < WIDTH; col++)
            for (int row = 0; row < HEIGHT; row++)
                if (isEmpty(col, row))
                    for (int i = 0; i < handSize; i++)
                        possibleMoves.emplace_back(col, row, hand[currentPlayer][i]);

        return possibleMoves;
    }

    static bool adjacentPosOOB(int col, int row) {
        return col < 0 || col >= WIDTH || row < 0 || row >= HEIGHT;
    }

    static void sortHand(const Player player) {
        std::sort(hand[player].begin(), hand[player].end(), std::greater<ID>());
    }

    static void removeCardFromHand(const Player player, const ID id) {
        hand[player].erase(
            std::remove(hand[player].begin(), hand[player].end(), id), hand[player].end()
        );
    }

    static void swapTurn() {
        currentPlayer = otherPlayer(currentPlayer);
    }

    static void flip(Card& card) {
        card.setControllingPlayer(otherPlayer(card.controllingPlayer()));
    }

    static void resolveFlipsAndRecordThem(PossibleMove move) {
        const int col = move.col; const int row = move.row;
        const Card& placedCard = cards[col][row];

        // Right Flips
        if (!adjacentPosOOB(col + 1, row)) {
            Card& adjacentCard = cards[col + 1][row];
            if (placedCard.isEnemy(adjacentCard))
                if (placedCard.attribute(RIGHT) > adjacentCard.attribute(LEFT)) {
                    MoveHistory::addFlip(move, RIGHT);
                    flip(adjacentCard);
                }
        }

        // Left Flips
        if (!adjacentPosOOB(col - 1, row)) {
            Card& adjacentCard = cards[col - 1][row];
            if (placedCard.isEnemy(adjacentCard))
                if (placedCard.attribute(LEFT) > adjacentCard.attribute(RIGHT)) {
                    MoveHistory::addFlip(move, LEFT);
                    adjacentCard.setControllingPlayer(placedCard.controllingPlayer());
                }
        }

        // Top Flips
        if (!adjacentPosOOB(col, row - 1)) {
            Card& adjacentCard = cards[col][row - 1];
            if (placedCard.isEnemy(adjacentCard))
                if (placedCard.attribute(TOP) > adjacentCard.attribute(BOTTOM)) {
                    MoveHistory::addFlip(move, TOP);
                    adjacentCard.setControllingPlayer(placedCard.controllingPlayer());
                }
        }

        // Bottom Flips
        if (!adjacentPosOOB(col, row + 1)) {
            Card& adjacentCard = cards[col][row + 1];
            if (placedCard.isEnemy(adjacentCard))
                if (placedCard.attribute(BOTTOM) > adjacentCard.attribute(TOP)) {
                    MoveHistory::addFlip(move, BOTTOM);
                    adjacentCard.setControllingPlayer(placedCard.controllingPlayer());
                }
        }
    }

    static void placeCard(PossibleMove move) {
        cards[move.col][move.row] = CardCollection::card(move.card);
        cards[move.col][move.row].setControllingPlayer(currentPlayer);
    }

    static void makeMove(PossibleMove move) {
        placeCard(move);
        removeCardFromHand(currentPlayer, move.card);
        sortHand(currentPlayer);
        swapTurn();
        MoveHistory::currentMove.move.card = move.card;
        MoveHistory::currentMove.move.col = move.col;
        MoveHistory::currentMove.move.row = move.row;
        MoveHistory::finishMove();
    }

    static void undoMove() {
        //std::cout << "Move history size: " << MoveHistory::moveHistory.size() << std::endl;
        const PossibleMove lastMove = MoveHistory::getLast().move;
        // Remove card from the board
        //std::cout << "Replacing card: " << CardCollection::name(cards[lastMove.col][lastMove.row].id()) << " with empty." << std::endl;
        cards[lastMove.col][lastMove.row] = CardCollection::getEmpty();

        // Restore flipped cards
        if (MoveHistory::getLast().flipped[TOP]) {
            flip(cards[lastMove.col][lastMove.row - 1]);
            //std::cout << "Undoing top flip" << std::endl;
        }
        if (MoveHistory::getLast().flipped[RIGHT]) {
            flip(cards[lastMove.col + 1][lastMove.row]);
            //std::cout << "Undoing right flip" << std::endl;
        }
        if (MoveHistory::getLast().flipped[BOTTOM]) {
            flip(cards[lastMove.col][lastMove.row + 1]);
            //std::cout << "Undoing bottom flip" << std::endl;
        }
        if (MoveHistory::getLast().flipped[LEFT]) {
            flip(cards[lastMove.col - 1][lastMove.row]);
            //std::cout << "Undoing left flip" << std::endl;
        }


        const Player previousPlayer = otherPlayer(currentPlayer);

        // Replace the card back into player's hand
        //std::cout << "Adding card back to player's hand: " << CardCollection::name(lastMove.card) << std::endl;
        hand[previousPlayer].push_back(lastMove.card);
        sortHand(previousPlayer);

        // Restore the previous player as the new current one
        swapTurn();
        //std::cout << "Reversing player" << std::endl;

        // Delete the turn from history
        MoveHistory::removeLast();
        //std::cout << "Deleting last move history" << std::endl;
    }

    static bool matchEnded() {
        for (int col = 0; col < WIDTH; col++)
            for (int row = 0; row < HEIGHT; row++)
                if (cards[col][row].controllingPlayer() == PLAYER_NONE)
                    return false;
        return true; // The game is over when all spaces have been filled
    }

    static void printColors() {
        for (int row = 0; row < HEIGHT; row++)
            for (int col = 0; col < WIDTH; col++)
                std::cout << colorToChar(cards[col][row].controllingPlayer()) << " ";
            std::cout << std::endl;
        std::cout << std::endl;
    }

    static Player winningPlayer() {
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