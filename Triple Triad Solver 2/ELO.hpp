#pragma once
#include "defs.hpp"
#include <unordered_map>
#include <algorithm>
#include <cmath>

namespace ELO {
    static std::unordered_map<ID, double> ratings;

    static const int K = 32; // Adjusting factor

    static void initializeRatings(ID deck) {
        if (ratings.find(deck) == ratings.end())
            ratings[deck] = 1200.0; // Default starting rating
    }

    static double expectedScore(double ratingA, double ratingB) {
        return 1.0 / (1.0 + pow(10.0, (ratingB - ratingA) / 400.0));
    }

    static void updateElo(ID redDeck, ID blueDeck, Player winner) {
        double redRating = ratings[redDeck];
        double blueRating = ratings[blueDeck];

        double expectedRed = expectedScore(redRating, blueRating);
        double expectedBlue = expectedScore(blueRating, redRating);

        double scoreRed, scoreBlue;
        switch (winner) {
        case PLAYER_RED:
            scoreRed = 1.0;
            scoreBlue = 0.0;
            break;
        case PLAYER_BLUE:
            scoreRed = 0.0;
            scoreBlue = 1.0;
            break;
        default:
            scoreRed = 0.5;
            scoreBlue = 0.5;
        }

        ratings[redDeck] += K * (scoreRed - expectedRed);
        ratings[blueDeck] += K * (scoreBlue - expectedBlue);
    }

    static std::vector<ID> getTopDecks(int numTopDecks) {
        std::vector<std::pair<ID, double>> sortedDecks(ratings.begin(), ratings.end());

        // Sort in descending order of Elo
        std::sort(sortedDecks.begin(), sortedDecks.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        // Extract the top X deck IDs
        std::vector<ID> topDecks;
        for (int i = 0; i < numTopDecks && i < sortedDecks.size(); i++)
            topDecks.push_back(sortedDecks[i].first);

        return topDecks;
    }

    static double getElo(ID deck) {
        if (ratings.find(deck) != ratings.end()) {
            return ratings[deck]; // Return the Elo rating for the given deck
        }
        else {
            return -1.0; // Return a value indicating the deck ID doesn't exist
        }
    }
}