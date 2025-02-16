#pragma once
#include "defs.hpp"
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <numeric>

class DeckStats {
public:
    static constexpr int MATCHES_TO_PLAY = 420;
    static constexpr int MATCHES_THRESHOLD = 100;

    enum Result {
        WIN, DRAW, LOSS, NONE
    };

    struct Stats {
    private:
        ID myDeckID;
        int myWins, myDraws, myLosses;
        std::unordered_map<ID, Result> playedAgainst;

    public:
        Stats() = default;

        // Getters
        const int& wins() const {
            return myWins;
        }

        const int& draws() const {
            return myDraws;
        }

        const int& losses() const {
            return myLosses;
        }

        const bool& hasPlayedAgainst(const ID& targetDeck) const {
            return playedAgainst.find(targetDeck) != playedAgainst.end();
        }

        const Result& resultAgainst(const ID& targetDeck) const {
            auto it = playedAgainst.find(targetDeck);
            return (it != playedAgainst.end()) ? it->second : Result::NONE;
        }

        const int& matchesPlayed() const {
            return myWins + myLosses + myDraws;
        }

        const float& winrate() const {
            if (!(matchesPlayed() < MATCHES_THRESHOLD)) // Avoids division by zero
                return (float(myWins) / float(matchesPlayed()));
            return -1.f;
        }

        // Setters
        void addWin() {
            myWins++;
        }

        void addLoss() {
            myLosses++;
        }

        void addDraw() {
            myDraws++;
        }

        void addMatchupResult(const ID& enemyDeck, const Result& result) {
            playedAgainst[enemyDeck] = result;
        }
    };

private:
    inline static std::vector<CardContainer> decks;
    inline static std::vector<Stats> stats;

public:
    static bool hasPlayedAgainst(const ID& deckFirst, const ID& deckSecond) {
        return stats[deckFirst].hasPlayedAgainst(deckSecond);
    }

    static void recordMatchResult(ID redDeck, ID blueDeck, Player winner) {
        Result redResult, blueResult;

        switch (winner) {
        case PLAYER_RED:
            redResult = Result::WIN;
            blueResult = Result::LOSS;
            stats[redDeck].addWin();
            stats[blueDeck].addLoss();
            break;
        case PLAYER_BLUE:
            redResult = Result::LOSS;
            blueResult = Result::WIN;
            stats[redDeck].addLoss();
            stats[blueDeck].addWin();
            break;
        case PLAYER_NONE:
            redResult = Result::DRAW;
            blueResult = Result::DRAW;
            stats[redDeck].addDraw();
            stats[blueDeck].addDraw();
            break;
        }

        stats[redDeck].addMatchupResult(blueDeck, redResult);
        stats[blueDeck].addMatchupResult(redDeck, blueResult);
    }

    static int deckCount() {
        return decks.size();
    }

    static ID bestPerformingID() {
        int bestID = 0;
        float bestWinrate = -1.f;
        for (int i = 0; i < stats.size(); i++)
            if (stats[i].winrate() > bestWinrate) {
                bestWinrate = stats[i].winrate();
                bestID = i;
            }
        return bestID;
    }

    static const CardContainer& deck(const ID id) {
        return decks[id];
    }

    static ID getID(const CardContainer& deck) {
        for (int i = 0; i < stats.size(); i++)
            if (decks[i] == deck)
                return i;
        std::cout << "DeckStats::getID() failed to find any deck which matched" << std::endl;
    }

    static void sort(CardContainer& deck) {
        std::sort(deck.begin(), deck.end());
    }

    static bool hasDuplicates(const CardContainer& deck) {
        for (int i = 0; i < DECK_SIZE; i++)
            for (int j = i + 1; j < DECK_SIZE; j++)
                if (deck[i] == deck[j])
                    return true;
        return false;
    }

    static bool isLegal(const CardContainer& deck) {
        // Deck rules: no more than 1 card of 5-star rarity per deck, 
        // and no more than 2 cards of 4-star rarity or higher per deck
        // Also no duplicates

        if (hasDuplicates(deck))
            return false;

        int fourStarCount = 0, fiveStarCount = 0;
        for (int i = 0; i < DECK_SIZE; i++)
            if (CardCollection::card(deck[i]).stars() == 4)
                fourStarCount++;
            else if (CardCollection::card(deck[i]).stars() == 5)
                fiveStarCount++;

        return (fourStarCount + fiveStarCount) <= 2
            && fiveStarCount < 2 && deck.size() == DECK_SIZE;
    }

    static int getRandomID() {
        return std::rand() % stats.size();
    }

    static CardContainer createRandom() {
        CardContainer tmpDeck;
        do {
            tmpDeck.clear();
            for (int j = 0; j < DECK_SIZE; j++) {
                tmpDeck.push_back(CardCollection::getRandomID());
            }
        } while (!isLegal(tmpDeck));
        std::sort(tmpDeck.begin(), tmpDeck.end());
        return tmpDeck;
    }

    static bool matchFound(const CardContainer targetDeck) {
        // Assumes targetDeck is sorted
        for (int i = 0; i < stats.size(); i++)
            if (targetDeck == decks[i])
                return true;
        return false;
    }

    static void addIfUnique(CardContainer deck) {
        sort(deck);
        if (!matchFound(deck)) {
            decks.push_back(deck);
            stats.emplace_back();
        }
    }

    static void initRandoms(const int count) {
        while (stats.size() < count)
            addIfUnique(createRandom());
    }

    static void initAllPossible() {
        CardContainer deck(DECK_SIZE);
        std::vector<uint16_t> cards;
        std::iota(cards.begin(), cards.end(), 1);  // Fill with {1, 2, 3, ..., TRIPLE_TRIAD_CARD_COUNT}

        // Generate all unique 5-card combinations
        std::vector<bool> selector(cards.size(), false);
        std::fill(selector.begin(), selector.begin() + DECK_SIZE, true);  // First DECK_SIZE elements = true

        do {
            int index = 0;
            for (int i = 0; i < cards.size(); i++)
                if (selector[i]) deck[index++] = cards[i];

            addIfUnique(deck);
        } while (std::prev_permutation(selector.begin(), selector.end()));
    }

    static void print(const ID& id) {
        std::cout << std::endl;
        std::cout << "Winrate: " << stats[id].winrate() * 100 << "%" << std::endl;
        std::cout << "Best Win-Loss-Draw: " << std::endl;
        std::cout << "W: " << stats[id].wins() << " | L: " << stats[id].losses() << " | D: " << stats[id].draws() << std::endl;
        std::cout << "Deck List: " << std::endl;

        for (int i = 0; i < decks[id].size(); i++)
            std::cout << CardCollection::name(decks[id][i]) << " (" << int(CardCollection::card(decks[id][i]).stars()) << "-Star), ";
        std::cout << std::endl;
    }

    static void printBest() {
        print(bestPerformingID());
    }
};