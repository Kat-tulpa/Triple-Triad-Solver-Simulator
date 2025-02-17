#include <cstdlib>  // For rand()
#include <ctime>    // For time()
#include "Graphics.hpp"
#include "Matchplay.hpp"

// Main game loop
int main() {
    std::srand(static_cast<int>(std::time(0)));  // Seed RNG
    CardCollection::init();
    DeckStats::initWithMaxStars(750);

    Graphics::init();
   
    Matchplay::graphicallyResimulateMatchManual(DeckStats::randomID(), DeckStats::randomID());
    
    Graphics::cleanup();
    return 0;
}
