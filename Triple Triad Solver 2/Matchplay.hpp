#pragma once
#include "DeckStats.hpp"
#include "Board.hpp"
#include "Search.hpp"
#include "Clock.hpp"
#include "CardCollection.hpp"
#include "Graphics.hpp"
#include "RenderableCardContainer.hpp"

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
    static void prepareBoard() {
        Board::init(DeckStats::randomID(), DeckStats::randomID());
    }

    static void simulateMatch() {
        // Assume the board's state is now managed by the Board namespace.
        while (!Board::matchEnded()) {
            // Find the best move for the current player
            PossibleMove bestMove = Search::findBestMove();

            // Make the move using the Board namespace
            Board::makeMove(bestMove);

            // Simulate the match by recording the result after the move
            // (Optional: Record intermediate results if needed)

            // Check if the match has ended after the move
            if (Board::matchEnded()) {
                break; // Exit if match is over
            }

            // Unmake the move to revert the board to its previous state
            Board::undoMove();
        }

        // After simulating, record the match result
        DeckStats::recordMatchResult(Board::deck[PLAYER_RED], Board::deck[PLAYER_BLUE], Board::winningPlayer());
    }

    static void playRandomMatchups() {
        if (!hasSufficientDecks()) return;

        int matchesPlayed = 0;
        while (matchesPlayed < DeckStats::MATCHES_TO_PLAY) {
            // Prepare the board using a static method from the Board namespace
            prepareBoard();  // Assuming this method sets up the board

            // Check if the decks have already played against each other
            if (!DeckStats::hasPlayedAgainst(Board::deck[PLAYER_RED], Board::deck[PLAYER_BLUE])) {
                // Clear transposition table before starting a new match
                transpositionTable.clear();

                // Simulate the match
                simulateMatch();  // Assuming simulateMatch now uses Board directly

                matchesPlayed++;
                Clock::printProgressEveryXseconds(matchesPlayed, DeckStats::MATCHES_TO_PLAY, 3);
            }
        }

        std::cout << "Games simulated: " << matchesPlayed << std::endl;
        DeckStats::printBest();
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

    static void findBestCounter(ID targetDeck) {
        std::vector<ID> decksWhichWonAgainst;

        // --- Play Matches As PLAYER_RED into targetDeck ---
        for (int i = 0; i < DeckStats::deckCount(); i++) {
            Board::init(i, targetDeck);  // Static method to prepare the board state
            transpositionTable.clear();

            // Simulate match until it ends
            while (!Board::matchEnded()) {
                Board::makeMove(Search::findBestMove());  // Apply best move to the board
            }

            if (Board::winningPlayer() == PLAYER_RED) {
                decksWhichWonAgainst.push_back(Board::deck[PLAYER_RED]);
            }
        }

        // --- Play Matches As PLAYER_BLUE into targetDeck ---
        std::vector<ID> finalistDecks;
        for (int i = 0; i < decksWhichWonAgainst.size(); i++) {
            Board::init(targetDeck, i);  // Static method to prepare the board state
            transpositionTable.clear();

            // Simulate match until it ends
            while (!Board::matchEnded()) {
                Board::makeMove(Search::findBestMove());  // Apply best move to the board
            }

            // Filter Out Losses From Finalist Decks
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
        std::cout << (float(finalistDeckCount * 100) / float(DeckStats::deckCount())) << "% Of Decks Were Able To Win-Win or Win-Draw Against It" << std::endl;
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
            Board::makeMove(Search::findBestMove());  // Apply the best move found
            SDL_Delay(16 * 360);  // Cap to ~0.18 fps (60 FPS equivalent delay)
            RenderableCardContainer::drawGame();  // Re-render after the move
        }

        RenderableCardContainer::drawGame();  // Final render after the match ends
    }

    static void graphicallyResimulateMatchManual(ID redDeck, ID blueDeck) {
        while (true) {  // Restart loop
            Graphics::background();  // Set up the background
            transpositionTable.clear();  // Clear the transposition table
            Board::init(redDeck, blueDeck);  // Initialize the board with the given decks
            RenderableCardContainer::drawGame();  // Initial render
            GraphicsSDL::RenderPresent();  // Ensure first frame is visible

            while (!Board::matchEnded()) {  // Loop until the match ends
                if (Board::currentPlayer == PLAYER_RED) {  // Player RED's turn
                    bool validMove = false;
                    int cardIndex, row, col;
                    while (!validMove) {
                        std::cout << "Player RED, it's your turn!" << std::endl;
                        std::cout << "Select a card to play (1-" << Board::hand[PLAYER_RED].size()
                            << ") followed by col(0-" << Board::WIDTH - 1 << ") and row(0-"
                            << Board::HEIGHT - 1 << "). Example: 412" << std::endl;

                        int input;
                        std::cin >> input;

                        // Extract values
                        cardIndex = input / 100 - 1;
                        row = (input / 10) % 10;
                        col = input % 10;

                        std::cout << "You selected card " << (cardIndex + 1)
                            << " to be placed at (" << row << ", " << col << ")" << std::endl;

                        if (cardIndex >= Board::hand[PLAYER_RED].size() || cardIndex < 0)
                            std::cout << "Invalid card selection. Try again." << std::endl;
                        else if (row < 0 || row >= Board::WIDTH || col < 0 || col >= Board::HEIGHT)
                            std::cout << "Out-of-bounds placement. Try again." << std::endl;
                        else if (!Board::isEmpty(col, row))
                            std::cout << "That spot is already occupied. Try again." << std::endl;
                        else
                            validMove = true;
                    }

                    // Apply the player's move
                    auto move = PossibleMove(col, row, Board::hand[PLAYER_RED][cardIndex]);
                    Board::makeMove(move);  // Static makeMove

                    //std::cout << CardCollection::name(Board::cards[0][0].id()) << CardCollection::name(Board::cards[1][0].id()) << CardCollection::name(Board::cards[2][0].id()) << std::endl;
                    //std::cout << CardCollection::name(Board::cards[0][1].id()) << CardCollection::name(Board::cards[1][1].id()) << CardCollection::name(Board::cards[2][1].id()) << std::endl;
                    //std::cout << CardCollection::name(Board::cards[0][2].id()) << CardCollection::name(Board::cards[1][2].id()) << CardCollection::name(Board::cards[2][2].id()) << std::endl;

                    // Ensure player move is displayed
                    RenderableCardContainer::drawGame();
                    GraphicsSDL::RenderPresent();
                }
                else {  // AI's turn
                    // Force render update before AI move
                    RenderableCardContainer::drawGame();
                    GraphicsSDL::RenderPresent();
                    SDL_Delay(100);  // Brief pause to ensure rendering

                    // AI makes a move
                    PossibleMove bestMove = Search::findBestMove();  // Get the best move
                    Board::makeMove(bestMove);  // Apply AI's move

                    // Ensure AI move is displayed
                    RenderableCardContainer::drawGame();
                    GraphicsSDL::RenderPresent();
                }
            }

            // Final render
            RenderableCardContainer::drawGame();
            GraphicsSDL::RenderPresent();

            // Wait for user input before restarting
            std::cout << "Match over! Press any key to restart..." << std::endl;
            std::cin.ignore();  // Clear newline if any left in buffer
            std::cin.get();  // Wait for user input
        }
    }
}