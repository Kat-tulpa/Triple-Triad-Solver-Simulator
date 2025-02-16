#pragma once
#include "defs.hpp"

struct PossibleMove {
    int col, row;
    ID card;
    bool flipped[4] = { false, false, false, false };

    PossibleMove() = default;

    PossibleMove(int _col, int _row, ID _card)
        : col(_col), row(_row), card(_card) {
        for (int i = 0; i < 4; i++)
            flipped[i] = false;
    }
};