#pragma once
#include "defs.hpp"
#include "ELO.hpp"
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <numeric>

class DeckStats {
public:
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
        Stats() : myDeckID(0), myWins(0), myDraws(0), myLosses(0) {
            ELO::initializeRatings(myDeckID);
        }

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

        const float winrate() const {
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

        void addMatchupResult(ID enemyDeck, Result result) {
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

    static void recordMatchResultAndUpdateELO(ID redDeck, ID blueDeck, Player winner) {
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
        ELO::updateElo(redDeck, blueDeck, winner);
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

    static ID id(const CardContainer deck) {
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

    static bool isLegal(const CardContainer deck) {
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

    static int randomID() {
        return std::rand() % stats.size();
    }

    static CardContainer createRandom() {
        CardContainer tmpDeck;
        do {
            tmpDeck.clear();
            for (int j = 0; j < DECK_SIZE; j++) {
                tmpDeck.push_back(CardCollection::randomID());
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
            stats.push_back(Stats());
            ELO::initializeRatings(decks.size() - 1);
        }
    }

    static void initRandoms(const int count) {
        while (stats.size() < count)
            addIfUnique(createRandom());
    }

    static CardContainer randomFromStarCriteria(const int askingCount[STARS_MAX]) {
        int found[STARS_MAX] = { 0, 0, 0, 0 ,0 };
        CardContainer cards;
        cards.reserve(DECK_SIZE);

        for (int i = 0; i < STARS_MAX; i++) {
            while (found[i] < askingCount[i]) {
                const ID& id = CardCollection::randomWithStarsID(i + 1);
                bool duplicateFound = false;
                for (int j = 0; j < cards.size(); j++)
                    if (cards[j] == id)
                        duplicateFound = true;
                if (!duplicateFound) {
                    found[i]++;
                    cards.push_back(id);
                }
            }
        }
        return cards;
    }

    static CardContainer createWithMaxStars() {
        // 0x 1-star, 0x 2-star, 3x 3-star, 1x 4-star, 1x 5-star
        const int maxStars[5] = { 0, 0, 3, 1, 1 };
        return randomFromStarCriteria(maxStars);
    }

    static void initWithMaxStars(const int count) {
        while (stats.size() < count)
            addIfUnique(createWithMaxStars());
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

    static bool containsCard(ID deckID, ID cardID) {
        for (int i = 0; i < DECK_SIZE; i++)
            if (deck(deckID).at(i) == cardID)
                return true;
        return false;
    }

    static double averageELOofCard(ID card) {
        double sumELO = 0;
        int matchHits = 0;
        for (int i = 0; i < decks.size(); i++) {
            if (containsCard(i, card)) {
                double deckElo = ELO::getElo(i);
                if (deckElo != -1.0) {  // Ensure Elo is valid
                    sumELO += deckElo;
                    matchHits++;
                }
            }
        }

        // Avoid division by zero if no valid decks contain the card
        return matchHits > 0 ? sumELO / matchHits : 0.0;
    }

    static std::vector<std::pair<ID, double>> topPerformingCards(int topX) {
        std::vector<std::pair<ID, double>> cardELOs;

        for (int i = 0; i < CardCollection::cardCount(); i++)
            cardELOs.push_back({ i, averageELOofCard(i)});

        // Sort the cards in descending order of Elo
        std::sort(cardELOs.begin(), cardELOs.end(),
            [](const std::pair<ID, double>& a, const std::pair<ID, double>& b) {
                return a.second > b.second;
            });

        // Extract the top X performing cards
        std::vector<std::pair<ID, double>> topCards;
        for (int i = 0; i < topX && i < cardELOs.size(); i++) {
            topCards.push_back(cardELOs[i]);
        }

        return topCards;  // Returns a vector of pairs {cardID, averageELO}
    }

    static std::vector<std::pair<ID, double>> lowestPerformingCards(int bottomX) {
        std::vector<std::pair<ID, double>> cardELOs;

        for (int i = 0; i < CardCollection::cardCount(); i++) {
            double cardElo = averageELOofCard(i);
            if (cardElo > 0) {  // Only include cards with Elo > 0
                cardELOs.push_back({ i, cardElo });
            }
        }

        // Sort the cards in ascending order of Elo to get the lowest-performing cards
        std::sort(cardELOs.begin(), cardELOs.end(),
            [](const std::pair<ID, double>& a, const std::pair<ID, double>& b) {
                return a.second < b.second;  // Ascending order for lowest ELO
            });

        // Extract the bottom X lowest-performing cards
        std::vector<std::pair<ID, double>> bottomCards;
        for (int i = 0; i < bottomX && i < cardELOs.size(); i++) {
            bottomCards.push_back(cardELOs[i]);
        }

        return bottomCards;  // Returns a vector of pairs {cardID, averageELO}
    }

    static void printBestCards(int numCards) {
        std::cout << std::endl;
        std::cout << "----------------------- Displaying Best Performing Cards ----------------------- " << std::endl;
        std::vector<std::pair<ID, double>> topCards = topPerformingCards(numCards);
        for (int i = 0; i < numCards; i++)
            printCardPerformance(topCards[i]);
    }

    static void printWorstCards(int numCards) {
        std::cout << std::endl;
        std::cout << "----------------------- Displaying Lowest Performing Cards ----------------------- " << std::endl;
        std::vector<std::pair<ID, double>> lowestCards = lowestPerformingCards(numCards);
        for (int i = 0; i < numCards; i++)
            printCardPerformance(lowestCards[i]);
    }
    
    static void printBestPerforming(int numDecks) {
        std::cout << std::endl;
        std::cout << "----------------------- Displaying Best Performing Decks -----------------------" << std::endl;
        std::vector<ID> topDecks = ELO::getTopDecks(numDecks);
        for (int i = 0; i < topDecks.size(); i++)
            DeckStats::print(topDecks[i]);
    }

    static void printCardPerformance(std::pair<ID, double> performance) {
        std::cout << std::endl;
        std::cout << "ELO: " << performance.second << std::endl;
        std::cout << "Card: " << CardCollection::name(performance.first) << std::endl;
    }

    static void print(ID id) {
        std::cout << std::endl;
        std::cout << "ELO: " << ELO::getElo(id) << std::endl;
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