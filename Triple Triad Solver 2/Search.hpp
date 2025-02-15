#pragma once
#include "defs.hpp"
#include <unordered_map>

namespace Search {
    struct TranspositionEntry {
        Player result;  // Stores the determined game outcome (PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE)
        int depth;      // Depth at which this position was analyzed
    };

    static std::unordered_map<int, TranspositionEntry> transpositionTable;

    static Player alphaBeta(Board& thisBoard, Player maximizingPlayer, int depth = 1) {
        /* int boardHash = thisBoard.hash();

        Check transposition table
        auto it = transpositionTable.find(boardHash);
        if (it != transpositionTable.end() && it->second.depth >= depth) {
            return it->second.result;  // Use cached result if available at sufficient depth
        }
        */

        if (thisBoard.matchEnded()) {
            return thisBoard.winningPlayer();  // Returns PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE
        }

        Player bestOutcome = (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Worst case scenario

        const auto& possibleMoves = thisBoard.getAllPossibleMoves();
        for (const auto& move : possibleMoves) {
            Board childBoard = thisBoard;
            childBoard.playCard(move);

            Player eval = alphaBeta(childBoard, (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED, depth + 1);

            // Maximizing Player (RED)
            if (maximizingPlayer == PLAYER_RED) {
                if (eval == PLAYER_RED) {
                    transpositionTable[childBoard.hash()] = { PLAYER_RED, depth };  // Store correct hash
                    return PLAYER_RED;  // Immediate win, prune further
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;  // Prefer a draw over losing
            }
            // Minimizing Player (BLUE)
            else {
                if (eval == PLAYER_BLUE) {
                    transpositionTable[childBoard.hash()] = { PLAYER_BLUE, depth };
                    return PLAYER_BLUE;  // Immediate win for opponent, prune
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;  // Prefer a draw over losing
            }
        }

        // Store result in transposition table using `childBoard.hash()`
        //transpositionTable[boardHash] = { bestOutcome, depth };
        return bestOutcome;
    }


    static Board findBestMove(Board& board) {
        Board bestMove = board;  // Default to current board
        Player currentPlayer = board.currentPlayer;
        Player bestOutcome = PLAYER_BLUE;  // Assume worst case for PLAYER_RED (losing)

        if (currentPlayer == PLAYER_BLUE) {
            bestOutcome = PLAYER_RED;  // Assume worst case for PLAYER_BLUE (losing)
        }

        const auto& possibleMoves = board.getAllPossibleMoves();
        for (const auto& move : possibleMoves) {
            Board newBoard = board;
            newBoard.playCard(move);

            Player eval = alphaBeta(newBoard, (currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED);

            if (eval == currentPlayer) {  // Winning move found
                return newBoard;
            }
            if (eval == PLAYER_NONE && bestOutcome != currentPlayer) {  // Prefer a draw over a loss
                bestOutcome = PLAYER_NONE;
                bestMove = newBoard;
            }
            else if (bestOutcome != currentPlayer && eval != currentPlayer) {
                // If still in worst case, update to the first available move
                bestMove = newBoard;
            }
        }

        return bestMove;
    }
}