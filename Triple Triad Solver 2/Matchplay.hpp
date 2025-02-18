#pragma once
#include "DeckStats.hpp"
#include "Board.hpp"
#include "Search.hpp"
#include "Clock.hpp"
#include "CardCollection.hpp"
#include "Graphics.hpp"
#include "RenderableCardContainer.hpp"

namespace Matchplay {
    static constexpr int MATCHES_TO_PLAY = 84000 * 32;
    // Check if there are enough decks for the simulation
    static bool hasSufficientDecks() {
        const int sufficientDecks = std::sqrt(MATCHES_TO_PLAY) * 2;
        if (DeckStats::deckCount() < sufficientDecks) {
            std::cout << "Error: Not enough decks to satisfy MATCHES_TO_PLAY... OR, will be EXTREMELY slow" << std::endl;
            return false;
        }
        return true;
    }

    // Select a random matchup and set up the board
    static void prepareBoard() {
        Board::init(DeckStats::randomID(), DeckStats::randomID());
    }

    static void simulateMatch() {
        auto [result, bestMove] = Search::minimax(0, true);
        DeckStats::recordMatchResultAndUpdateELO(
            Board::deck[PLAYER_RED], Board::deck[PLAYER_BLUE], Player(result));
    }

    static void playAllMatchupsOnce() {
        const int matchesQueued = DeckStats::deckCount() * (DeckStats::deckCount() - 1);
        int matchesPlayed = 0;
        // Loop through every pair of decks once
        for (int i = 0; i < DeckStats::deckCount(); ++i) {
            for (int j = i + 1; j < DeckStats::deckCount(); ++j) {
                // First match: redDeck vs blueDeck
                Board::init(i, j); // Set the decks as red and blue
                simulateMatch(); // Simulate the match

                // Second match: blueDeck vs redDeck (swap sides)
                Board::init(j, i); // Swap the decks for this match
                simulateMatch(); // Simulate the match

                matchesPlayed += 2;  // Each pair of decks results in 2 matches
                std::cout << "Match Finished!" << std::endl;
                Clock::printProgressEveryXseconds(Search::nodes / 100, 1, 3); // Progress tracker
            }
        }

        std::cout << "Games simulated: " << matchesPlayed << std::endl;
    }

    static void playMatchupsRandomly() {
        if (!hasSufficientDecks()) return;

        int matchesPlayed = 0;
        while (matchesPlayed < MATCHES_TO_PLAY) {
            // Prepare the board using a static method from the Board namespace
            prepareBoard();  // Assuming this method sets up the board

            // Check if the decks have already played against each other
            if (!DeckStats::hasPlayedAgainst(Board::deck[PLAYER_RED], Board::deck[PLAYER_BLUE])) {
                // Clear transposition table before starting a new match
                transpositionTable.clear();

                // Simulate the match
                simulateMatch();  // Assuming simulateMatch now uses Board directly

                matchesPlayed++;
                Clock::printProgressEveryXseconds(matchesPlayed, MATCHES_TO_PLAY, 3);
            }
        }

        std::cout << "Games simulated: " << matchesPlayed << std::endl;
    }

    // 1. Create a predefined target deck
    static CardContainer createTargetDeck() {
        return {
            CardCollection::idFromName("Shantotto"),
            CardCollection::idFromName("Vaan"),
            CardCollection::idFromName("Lightning"),
            CardCollection::idFromName("Noctis Lucis Caelum"),
            CardCollection::idFromName("Clive Rosfield")
        };
    }

    /*static void findBestCounter(ID targetDeck) {
        std::vector<ID> decksWhichWonAgainst;

        // --- Play Matches As PLAYER_RED into targetDeck ---
        for (int i = 0; i < DeckStats::deckCount(); i++) {
            Board::init(i, targetDeck);
            transpositionTable.clear();

            while (!Board::matchEnded()) {
                PossibleMove bestMove = Search::findBestMove();
                if (bestMove.isEmpty())
                    std::cout << "Matchplay::findBestCounter() had no valid moves." << std::endl;

                Board::makeMove(bestMove);  // Apply the move if it's valid
            }

            if (Board::winningPlayer() == PLAYER_RED) {
                decksWhichWonAgainst.push_back(Board::deck[PLAYER_RED]);
            }
        }

        // --- Play Matches As PLAYER_BLUE into targetDeck ---
        std::vector<ID> finalistDecks;
        for (int i = 0; i < decksWhichWonAgainst.size(); i++) {
            Board::init(targetDeck, i);
            transpositionTable.clear();

            while (!Board::matchEnded()) {
                auto bestMove = Search::findBestMove();
                if (bestMove.isEmpty())
                    std::cout << "Matchplay::findBestCounter() had no valid moves." << std::endl;
                Board::makeMove(bestMove);
            }

            if (Board::winningPlayer() == PLAYER_BLUE || Board::winningPlayer() == PLAYER_NONE) {
                finalistDecks.push_back(Board::deck[PLAYER_BLUE]);
            }
        }

        std::cout << "Finalist Count: " << finalistDecks.size() << std::endl << std::endl;

        // Print The Results
        const int finalistDeckCount = finalistDecks.size();
        for (int i = 0; i < finalistDeckCount; i++) {
            DeckStats::print(finalistDecks[i]);
        }

        std::cout << std::endl;
        std::cout << (float(finalistDeckCount * 100) / float(DeckStats::deckCount()))
            << "% Of Decks Were Able To Win-Win or Win-Draw Against It"
            << std::endl;
    }

    // 3. Find the best counter to the target deck
    static void findCounterForDeck(const CardContainer& targetDeck) {
        findBestCounter(DeckStats::id(targetDeck));
    }

    // Main function using the refactored helpers
    static void playMatchupsAgainstPreloadedDeck() {
        CardContainer targetDeck = createTargetDeck();
        DeckStats::addIfUnique(targetDeck);
        findCounterForDeck(targetDeck);
    }

    static void graphicallyResimulateMatch(ID redDeck, ID blueDeck) {
        Graphics::background();  // Set up the background
        transpositionTable.clear();  // Clear the transposition table
        Board::init(redDeck, blueDeck);  // Initialize the board with the given decks
        RenderableCardContainer::drawGame();  // Initial render

        while (!Board::matchEnded()) {
            auto bestMove = Search::findBestMove();
            if (bestMove.isEmpty())
                std::cout << "Matchplay::graphicallyResimulateMatch() had no valid moves." << std::endl;
            Board::makeMove(bestMove);
            SDL_Delay(16 * 360);  // Cap to ~0.18 fps (60 FPS equivalent delay)
            RenderableCardContainer::drawGame();  // Re-render after the move
        }

        RenderableCardContainer::drawGame();  // Final render after the match ends
    } */

    static void initializeMatch(ID redDeck, ID blueDeck) {
        Graphics::background();
        transpositionTable.clear();
        Board::init(redDeck, blueDeck);
        RenderableCardContainer::drawGame();
        GraphicsSDL::RenderPresent();
    }

    static void displayMoveHelper() {
        std::cout << "Player RED, it's your turn!" << std::endl;
        std::cout << "Select a card to play (1-" << Board::hand[PLAYER_RED].size()
            << ") followed by x(0-" << Board::WIDTH - 1 << ") and y(0-"
            << Board::HEIGHT - 1 << "). Example: 412" << std::endl;

    }

    static bool validateMove(int index, int x, int y) {
        if (index >= Board::hand[PLAYER_RED].size() || index < 0) {
            return false;
            std::cout << "Invalid card selection. Try again." << std::endl;
        }

        else if (x < 0 || x >= Board::WIDTH || y < 0 || y >= Board::HEIGHT) {
            std::cout << "Out-of-bounds placement. Try again." << std::endl;
            return false;
        }

        if (!Board::isEmpty(x, y)) {
            std::cout << "That spot is already occupied. Try again." << std::endl;
            return false;
        }
        
        return true;
    }

    static PossibleMove findLastRemainingMove() {
        for (int col = 0; col < Board::WIDTH; col++)
            for (int row = 0; row < Board::HEIGHT; row++)
                if (Board::isEmpty(col, row))
                    return PossibleMove{ col, row, Board::hand[PLAYER_RED][0] };
    }

    static PossibleMove getPlayerMove() {
        // Auto-make the last remaining move
        if (Board::getAllPossibleMoves().size() == 1)
            return findLastRemainingMove();

        bool validMove = false;
        int cardIndex, x, y;
        while (!validMove) {
            displayMoveHelper();
            int input;
            std::cin >> input;

            cardIndex = input / 100 - 1;
            x = (input / 10) % 10;
            y = input % 10;
            y = (Board::HEIGHT - 1) - y;  // Flip y-axis

            std::cout << "You selected card " << (cardIndex + 1)
                << " to be placed at (" << x << ", " << (Board::HEIGHT - 1) - y << ")" << std::endl;

            validMove = validateMove(cardIndex, x, y);
        }
        return PossibleMove(x, y, Board::hand[PLAYER_RED][cardIndex]);
    }

    static void handlePlayerTurn() {
        PossibleMove move = getPlayerMove();
        Board::makeMove(move);
        RenderableCardContainer::drawGame();
        GraphicsSDL::RenderPresent();
    }

    /*static void handleAITurn() {
        RenderableCardContainer::drawGame();
        GraphicsSDL::RenderPresent();
        SDL_Delay(100);
        auto bestMove = Search::findBestMove();
        if (bestMove.isEmpty())
            std::cout << "Matchplay::handleAITurn() had no valid moves." << std::endl;
        Board::makeMove(bestMove);
        RenderableCardContainer::drawGame();
        GraphicsSDL::RenderPresent();
    }*/

    static void displayMatchResult() {
        if (Board::winningPlayer() == PLAYER_RED)
            std::cout << "Game over, RED won!" << std::endl;
        else if (Board::winningPlayer() == PLAYER_BLUE)
            std::cout << "Game over, BLUE won!" << std::endl;
        else
            std::cout << "Game was drawn!" << std::endl;

        std::cout << "Press any key to restart..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    }

    /*static void graphicallyResimulateMatchManualRandomDecks() {
        while (true) {
            transpositionTable.clear();
            initializeMatch(DeckStats::randomID(), DeckStats::randomID());

            while (!Board::matchEnded()) {
                if (Board::currentPlayer == PLAYER_RED)
                    handlePlayerTurn();
                else
                    handleAITurn();
            }

            RenderableCardContainer::drawGame();
            GraphicsSDL::RenderPresent();
            displayMatchResult();
        }
    }*/
}