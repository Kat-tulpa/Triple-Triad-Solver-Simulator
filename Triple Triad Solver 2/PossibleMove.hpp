#pragma once
#include "defs.hpp"

struct PossibleMove {
    int col, row;
    ID card;

    PossibleMove() = default;

    PossibleMove(int _col, int _row, ID _card)
        : col(_col), row(_row), card(_card) {}

    bool isEmpty() const {
        return row == 0 && col == 0 && card == 0;
    }

    std::string toString() const {
        return "Col:" + std::to_string(col) + "Row: " + std::to_string(row) + " " + "Card ID: " + std::to_string(card);
    }
};