#pragma once
#include "Card.hpp"

using CardGrid = std::vector<std::vector<Card>>;

static void initCardGrid(CardGrid& cards, int numColumns, int numRows) {
    // Resize the outer vector (rows)
    cards.resize(numRows);

    // Resize each inner vector (columns)
    for (int row = 0; row < numRows; ++row) {
        cards[row].resize(numColumns);  // Ensure that each row has 'numColumns' cards
    }
}

static int cardGridRowCount(const CardGrid& cards) {
    return cards.size();  // Number of rows
}

static int cardGridColCount(const CardGrid& cards) {
    if (cards.empty() || cards[0].empty()) {
        return 0;  // Return 0 if there are no rows or no columns
    }
    return cards[0].size();  // Number of columns
}