#pragma once
#include "defs.hpp"

struct PossibleMove {
    int col, row;
    ID card;

    PossibleMove() = default;

    PossibleMove(int _col, int _row, ID _card)
        : col(_col), row(_row), card(_card) {}
};