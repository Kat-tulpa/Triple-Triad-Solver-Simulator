#include <cstdlib>  // For rand()
#include <ctime>    // For time()
#include "Graphics.hpp"
#include "Matchplay.hpp"

// Main game loop
int main() {
    std::srand(static_cast<int>(std::time(0)));  // Seed RNG
    CardCollection::init();
    DeckStats::initRandoms(750);
    //std::cout << "Starting matchups" << std::endl;
    //Matchplay::playRandomMatchups();

    Graphics::init();
    
    // Watch a random match
    ID redDeck = rand() % DeckStats::deckCount();
    ID blueDeck = rand() % DeckStats::deckCount();

    CardContainer redDeckCards = DeckStats::deck(redDeck);
    CardContainer blueDeckCards = DeckStats::deck(blueDeck);

    int redStarCount[5] = { 0, 0, 0, 0, 0 };
    int blueStarCount[5] = { 0, 0, 0, 0, 0 };

    for (int i = 0; i < DECK_SIZE; i++) {
        redStarCount[CardCollection::card(redDeckCards[i]).stars() - 1]++;
        blueStarCount[CardCollection::card(blueDeckCards[i]).stars() - 1]++;
    }

    bool redDeckGood = redStarCount[5] && redStarCount[3] && (redStarCount[2] == 3);
    bool blueDeckGood = blueStarCount[5] && blueStarCount[3] && (blueStarCount[2] == 3);

    while (!redDeckGood || !blueDeckGood) {
        redDeck = rand() % DeckStats::deckCount();
        blueDeck = rand() % DeckStats::deckCount();
        redDeckCards = DeckStats::deck(redDeck);
        blueDeckCards = DeckStats::deck(blueDeck);

        for (int j = 0; j < 5; j++) {  // Only iterate over 5, not DECK_SIZE
            redStarCount[j] = 0;
            blueStarCount[j] = 0;
        }

        for (int i = 0; i < DECK_SIZE; i++) {
            redStarCount[CardCollection::card(redDeckCards[i]).stars() - 1]++;
            blueStarCount[CardCollection::card(blueDeckCards[i]).stars() - 1]++;
        }

        // Update the original variables, not create new ones
        redDeckGood = redStarCount[4] && redStarCount[3] && (redStarCount[2] == 3);
        blueDeckGood = blueStarCount[4] && blueStarCount[3] && (blueStarCount[2] == 3);
    }

    //graphicallyResimulateMatchManual(redDeck, DeckStats::bestPerformingID());
    Matchplay::graphicallyResimulateMatchManual(redDeck, blueDeck);
    

    //GraphicsSDL::eventLoop();
    Graphics::cleanup();

    return 0;
}
