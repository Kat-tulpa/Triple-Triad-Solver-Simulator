#include <cstdlib>  // For rand()
#include <ctime>    // For time()
#include "Graphics.hpp"
#include "Matchplay.hpp"

// Main game loop
int main() {
    std::srand(static_cast<int>(std::time(0)));  // Seed RNG
    CardCollection::init();
    DeckStats::initRandoms(750);
    //playRandomMatchups();

    Graphics::init();

    // Watch a random match
    const ID redDeck = rand() % DeckStats::deckCount();
    const ID blueDeck = rand() % DeckStats::deckCount();
    //graphicallyResimulateMatchManual(redDeck, DeckStats::bestPerformingID());
    Matchplay::graphicallyResimulateMatchManual(redDeck, blueDeck);

    GraphicsSDL::eventLoop();
    Graphics::cleanup();

    return 0;
}
