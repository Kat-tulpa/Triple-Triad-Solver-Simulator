#pragma once
#include "defs.hpp"
#include <unordered_map>

struct TranspositionEntry {
    Player result;  // Stores the determined game outcome (PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE)
    int depth;      // Depth at which this position was analyzed
};

inline static std::unordered_map<int, TranspositionEntry> transpositionTable;