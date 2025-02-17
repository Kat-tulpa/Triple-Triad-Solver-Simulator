#pragma once
#include "PossibleMove.hpp"

struct MoveHistory {
    static std::vector<MoveHistory> moveHistory;
    static MoveHistory currentMove;  // Static member declaration

    bool flipped[4] = { false, false, false, false };
    PossibleMove move;

    static void addFlip(PossibleMove move, Edge edge) {
        currentMove.flipped[edge] = true;
    }

    static void finishMove() {
        moveHistory.push_back(currentMove);
        currentMove = MoveHistory(); // Reset currentMove
    }

    static void removeLast() {
        if (!moveHistory.empty()) {
            moveHistory.pop_back();
        }
        else
            std::cout << "MoveHistory::removeLast() was called despite being empty!" << std::endl;
    }

    static MoveHistory getLast() {
        if (!moveHistory.empty()) {
            return moveHistory[moveHistory.size() - 1];  // Access the last element correctly
        }
        return MoveHistory();  // Return an empty MoveHistory if the history is empty
    }
};

// Definition of the static member outside the struct in the header file itself
std::vector<MoveHistory> MoveHistory::moveHistory;
MoveHistory MoveHistory::currentMove;