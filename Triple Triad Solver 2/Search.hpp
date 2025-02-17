#pragma once
#include "defs.hpp"
#include "TranspositionTable.hpp"
#include "Board.hpp"

namespace Search {
    static Player alphaBeta(Player maximizingPlayer, int depth = 1) {
        int boardHash = Board::hash();  // Compute hash before move
        auto it = transpositionTable.find(boardHash);
        if (it != transpositionTable.end() && it->second.depth >= depth)
            return it->second.result;  // Use cached result if available at sufficient depth
        if (Board::matchEnded())
            return Board::winningPlayer();  // Returns PLAYER_RED, PLAYER_BLUE, or PLAYER_NONE
        Player bestOutcome = (maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Worst case scenario
        const auto possibleMoves = Board::getAllPossibleMoves();
        for (const auto& move : possibleMoves) {
            Board::makeMove(move);  // Apply move
            Player eval = alphaBeta((maximizingPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED, depth + 1);
            Board::undoMove();  // Undo move after recursion
            // Recompute hash in case flipping affected state
            boardHash = Board::hash();

            // Maximizing Player (RED)
            if (maximizingPlayer == PLAYER_RED)
                if (eval == PLAYER_RED) {
                    transpositionTable[boardHash] = { PLAYER_RED, depth };
                    return PLAYER_RED;  // Immediate win, prune further
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;
            // Minimizing Player (BLUE)
            else
                if (eval == PLAYER_BLUE) {
                    transpositionTable[boardHash] = { PLAYER_BLUE, depth };
                    return PLAYER_BLUE;  // Immediate win for opponent, prune
                }
                if (eval == PLAYER_NONE) bestOutcome = PLAYER_NONE;
        }
        transpositionTable[boardHash] = { bestOutcome, depth };
        return bestOutcome;
    }

    static PossibleMove findBestMove() {
        Player currentPlayer = Board::currentPlayer;
        Player bestOutcome = (currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED;  // Assume worst case
        PossibleMove bestMove;  // Variable to store the best move found
        const auto possibleMoves = Board::getAllPossibleMoves();  // Get all possible moves for the current player

        for (const auto& move : possibleMoves) {
            // Apply the move to the board
            Board::makeMove(move);

            // Evaluate the position using the alpha-beta search for the opponent's turn
            Player eval = alphaBeta((currentPlayer == PLAYER_RED) ? PLAYER_BLUE : PLAYER_RED);

            // If the move results in a win for the current player, return the best move directly
            if (eval == currentPlayer) {
                bestMove = move;
                Board::undoMove();  // Revert the move after evaluation
                return bestMove;  // Return the winning move
            }
            // If a draw is found and it’s better than a loss, prefer a draw over a loss
            if (eval == PLAYER_NONE && bestOutcome != currentPlayer) {
                bestOutcome = PLAYER_NONE;
                bestMove = move;
            }
            // If we haven’t found a winning or drawing move, update the worst-case scenario
            else if (bestOutcome != currentPlayer && eval != currentPlayer) {
                bestMove = move;
            }

            // Undo the move after the evaluation is done
            Board::undoMove();
        }

        // Return the best move found after evaluating all possibilities
        return bestMove;
    }
}