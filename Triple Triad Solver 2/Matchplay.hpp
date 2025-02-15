#pragma once
#include "DeckStats.hpp"
#include "Board.hpp"
#include "Search.hpp"
#include "Clock.hpp"
#include "CardCollection.hpp"
#include "Graphics.hpp"

namespace Matchplay {
    // Check if there are enough decks for the simulation
    static bool hasSufficientDecks() {
        const int sufficientDecks = std::sqrt(DeckStats::MATCHES_TO_PLAY) * 2;
        if (DeckStats::deckCount() < sufficientDecks) {
            std::cout << "Error: Not enough decks to satisfy MATCHES_TO_PLAY... OR, will be EXTREMELY slow" << std::endl;
            return false;
        }
        return true;
    }

    // Select a random matchup and set up the board
    static Board preparedBoard() {
        return Board(DeckStats::getRandomID(), DeckStats::getRandomID());
    }

    // Simulate a match and return the result
    static void simulateMatch(Board& board) {
        int movesPlayed = 0;
        while (!board.matchEnded()) {
            Board bestMove = Search::findBestMove(board);
            board = bestMove;
            movesPlayed++;
        }
        DeckStats::recordMatchResult(board.deckIDs[PLAYER_RED],
            board.deckIDs[PLAYER_BLUE], board.winningPlayer());
    }

    // Print progress of the simulation
    static void printSimulationProgress(int matchesPlayed) {
        Clock::printProgressEveryXseconds(matchesPlayed, DeckStats::MATCHES_TO_PLAY, 3);
    }

    // Main simulation loop
    static void playRandomMatchups() {
        if (!hasSufficientDecks()) return;

        int matchesPlayed = 0;
        while (matchesPlayed < DeckStats::MATCHES_TO_PLAY) {
            Board board = preparedBoard();

            if (!DeckStats::hasPlayedAgainst(board.deckIDs[PLAYER_RED], board.deckIDs[PLAYER_BLUE])) {
                Search::transpositionTable.clear();
                simulateMatch(board);
                matchesPlayed++;
                printSimulationProgress(matchesPlayed);
            }
        }

        std::cout << "Games simulated: " << matchesPlayed << std::endl;
        DeckStats::printBest();
    }

    // 1. Create a predefined target deck
    static CardContainer createTargetDeck() {
        return {
            CardCollection::getIDfromName("Shantotto"),
            CardCollection::getIDfromName("Vaan"),
            CardCollection::getIDfromName("Lightning"),
            CardCollection::getIDfromName("Noctis Lucis Caelum"),
            CardCollection::getIDfromName("Clive Rosfield")
        };
    }

    static void findBestCounter(ID targetDeck) {
        std::vector<ID> decksWhichWonAgainst;

        // --- Play Matches As PLAYER_RED into targetDeck ---
        for (int i = 0; i < DeckStats::deckCount(); i++) {
            Board board = Board(i, targetDeck);
            Search::transpositionTable.clear();

            while (!board.matchEnded()) {
                Board bestMove = Search::findBestMove(board);
                board = bestMove;
            }

            if (board.winningPlayer() == PLAYER_RED)
                decksWhichWonAgainst.push_back(board.deckIDs[PLAYER_RED]);
        }

        // --- Play Matches As PLAYER_BLUE into targetDeck ---
        std::vector<ID> finalistDecks;
        for (int i = 0; i < decksWhichWonAgainst.size(); i++) {
            Board board = Board(targetDeck, i);
            Search::transpositionTable.clear();

            while (!board.matchEnded()) {
                Board bestMove = Search::findBestMove(board);
                board = bestMove;
            }

            // Filter Out Losses From Finalist Decks
            if (board.winningPlayer() == PLAYER_BLUE || board.winningPlayer() == PLAYER_NONE)
                finalistDecks.push_back(board.deckIDs[PLAYER_BLUE]);
        }

        std::cout << "Finalist Count: " << finalistDecks.size() << std::endl << std::endl;

        // Print The Results
        const int finalistDeckCount = finalistDecks.size();
        for (int i = 0; i < finalistDeckCount; i++)
            DeckStats::print(finalistDecks[i]);

        std::cout << std::endl;
        std::cout << (float(finalistDeckCount * 100) / float(DeckStats::deckCount())) << "% Of Decks Were Able To Win-Win or Win-Draw Against It" << std::endl;
    }

    // 3. Find the best counter to the target deck
    static void findCounterForDeck(const CardContainer& targetDeck) {
        findBestCounter(DeckStats::getID(targetDeck));
    }

    // Main function using the refactored helpers
    static void playMatchupsAgainstPreloadedDeck() {
        CardContainer targetDeck = createTargetDeck();
        DeckStats::addIfUnique(targetDeck);
        findCounterForDeck(targetDeck);
    }

    static void graphicallyResimulateMatch(ID redDeck, ID blueDeck) {
        Graphics::background();
        Search::transpositionTable.clear();
        Board board = Board(redDeck, blueDeck);
        Graphics::RenderableCardContainer::drawGame(board);

        while (!board.matchEnded()) {
            Board bestMove = Search::findBestMove(board);
            board = bestMove;
            SDL_Delay(16 * 360);  // Cap to 0.18ish fps
            Graphics::RenderableCardContainer::drawGame(board);
        }

        Graphics::RenderableCardContainer::drawGame(board);
    }

    static void graphicallyResimulateMatchManual(ID redDeck, ID blueDeck) {
        while (true) {  // Restart loop
            Graphics::background();
            Search::transpositionTable.clear();
            Board board = Board(redDeck, blueDeck);
            Graphics::RenderableCardContainer::drawGame(board);
            GraphicsSDL::RenderPresent();  // Ensure first frame is visible

            while (!board.matchEnded()) {
                if (board.currentPlayer == PLAYER_RED) {
                    bool validMove = false;
                    int cardIndex, row, col;
                    while (!validMove) {
                        std::cout << "Player RED, it's your turn!" << std::endl;
                        std::cout << "Select a card to play (1-" << board.hand[PLAYER_RED].size()
                            << ") followed by row(0-" << Board::WIDTH - 1 << ") and col(0-"
                            << Board::HEIGHT - 1 << "). Example: 412" << std::endl;

                        int input;
                        std::cin >> input;

                        // Extract values
                        cardIndex = input / 100 - 1;
                        row = (input / 10) % 10;
                        col = input % 10;

                        std::cout << "You selected card " << (cardIndex + 1)
                            << " to be placed at (" << row << ", " << col << ")" << std::endl;

                        if (cardIndex >= board.hand[PLAYER_RED].size() || cardIndex < 0)
                            std::cout << "Invalid card selection. Try again." << std::endl;
                        else if (row < 0 || row >= Board::WIDTH || col < 0 || col >= Board::HEIGHT)
                            std::cout << "Out-of-bounds placement. Try again." << std::endl;
                        else if (!board.isEmpty(col, row))
                            std::cout << "That spot is already occupied. Try again." << std::endl;
                        else
                            validMove = true;
                    }

                    auto move = Board::PossibleMove(col, row, board.hand[PLAYER_RED][cardIndex]);
                    board.playCard(move);

                    // Ensure player move is displayed
                    Graphics::RenderableCardContainer::drawGame(board);
                    GraphicsSDL::RenderPresent();
                }
                else {
                    // Force render update before AI move
                    Graphics::RenderableCardContainer::drawGame(board);
                    GraphicsSDL::RenderPresent();
                    SDL_Delay(100);  // Brief pause to ensure rendering

                    // AI move
                    Board bestMove = Search::findBestMove(board);
                    board = bestMove;

                    // Ensure AI move is displayed
                    Graphics::RenderableCardContainer::drawGame(board);
                    GraphicsSDL::RenderPresent();
                }
            }

            // Final render
            Graphics::RenderableCardContainer::drawGame(board);
            GraphicsSDL::RenderPresent();

            // Wait for user input before restarting
            std::cout << "Match over! Press any key to restart..." << std::endl;
            std::cin.ignore();  // Clear newline if any left in buffer
            std::cin.get();  // Wait for user input
        }
    }
}