#pragma once
#include "defs.hpp"
#include "TranspositionTable.hpp"
#include "Board.hpp"

namespace Search {
    static Player alphaBeta(Board& thisBoard, Player maximizingPlayer, int depth = 1) {
        int boardHash = thisBoard.hash();

        auto it = transpositionTable.find(boardHash);
        if (it != transpositionTable.end() && it->second.depth >= depth) {
            return it->second.result;  // Use cached result if available at sufficient depth
        }

        if (thisBoard.matchEnded()) {
            return thisBoard.winningPlayer();  // Returns PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE
        }

        Player bestOutcome = (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Worst case scenario

        const auto possibleMoves = thisBoard.getAllPossibleMoves();
        for (const auto& move : possibleMoves) {
            thisBoard.makeMove(move);  // Apply move

            Player eval = alphaBeta(thisBoard, (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED, depth + 1);

            thisBoard.undoLastMove();  // Undo move after recursion

            // Maximizing Player (RED)
            if (maximizingPlayer == PLAYER_RED) {
                if (eval == PLAYER_RED) {
                    transpositionTable[boardHash] = { PLAYER_RED, depth };
                    return PLAYER_RED;  // Immediate win, prune further
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;
            }
            // Minimizing Player (BLUE)
            else {
                if (eval == PLAYER_BLUE) {
                    transpositionTable[boardHash] = { PLAYER_BLUE, depth };
                    return PLAYER_BLUE;  // Immediate win for opponent, prune
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;
            }
        }

        transpositionTable[boardHash] = { bestOutcome, depth };
        return bestOutcome;
    }

    static Board findBestMove(Board& board) {
        Board bestMove = board;  // Default to current board
        Player currentPlayer = board.currentPlayer;
        Player bestOutcome = (currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Assume worst case

        const auto possibleMoves = board.getAllPossibleMoves();
        for (const auto& move : possibleMoves) {
            board.makeMove(move);  // Apply move

            Player eval = alphaBeta(board, (currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED);

            if (eval == currentPlayer) {  // Winning move found
                bestMove = board;
                board.undoLastMove();  // Undo before returning
                return bestMove;
            }
            if (eval == PLAYER_NONE && bestOutcome != currentPlayer) {  // Prefer a draw over a loss
                bestOutcome = PLAYER_NONE;
                bestMove = board;
            }
            else if (bestOutcome != currentPlayer && eval != currentPlayer) {
                // If still in worst case, update to the first available move
                bestMove = board;
            }

            board.undoLastMove();  // Undo move after evaluation
        }

        return bestMove;
    }
}