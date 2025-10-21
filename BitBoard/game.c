//
// Created by saraw on 2025-10-20.
//

#include "game.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Top row, white promoting to king
#define white_promotion_row 0xFF00000000000000ULL
// Bottom row, black promoting to king
#define black_promotion_row 0x00000000000000FFULL

// Initializing Game
// White starts at bottom, black at top

GameState init_game(void) {
    GameState g = {0};
    // Bottom 3 rows
    g.white = 0x0000000000AA55AAULL;
    // Top 3 rows
    g.black = 0x55AA550000000000ULL;
    g.black_kings = 0;
    g.white_kings = 0;
    //White moves first
    g.white_turn = true;
    return g;
}

// To manage turns, switches active player's turns
void switch_turns(GameState *g) {
    g->white_turn = !g->white_turn;
}

// checks if a piece can continue capturing after a jump
bool can_capture_again(GameState * game_state, int to, bool is_white_piece);

// applies a move from one square to another
// Handles: Single move, captures, Multi-jumps, and promotions
bool apply_move(GameState *g, int from, int to) {
    if (from < 0 || from > 63 || to < 0 || to > 63) return false;

    // Enforces turns
    if ((g->white_turn && !is_white(g, from)) ||
        (!g->white_turn && !is_black(g, from))) {
        return false;
    }

    int fromRow = from / 8;
    int fromCol = from % 8;
    int toRow = to / 8;
    int toCol = to % 8;
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    // Only move on dark square
    if (((toRow + toCol) % 2) == 0) return false;

    // Square moved to must be empty
    if (is_square_occupied(g, to)) return false;

    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask = 1ULL << to;

    bool isKing = (g->white_kings & from_mask) || (g->black_kings & from_mask);

    // Regular pieces can only move forward
    if (!isKing) {
        if (g->white_turn && rowDiff <= 0) return false; // white moves up visually
        if (!g->white_turn && rowDiff >= 0) return false; // black moves down visually
    }

    // Normal single move
    if (abs(rowDiff) == 1 && abs(colDiff) == 1) {
    }

    // Capture move
    else if (abs(rowDiff) == 2 && abs(colDiff) == 2) {
        if (!isKing) {
        if (g->white_turn && rowDiff <= 0) return false;
        if (!g->white_turn && rowDiff >= 0) return false;
    }
        // Find the jumped square
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        int midIndex = midRow * 8 + midCol;
        uint64_t midMask = 1ULL << midIndex;

        // Remove enemy piece
        if (g->white_turn && is_black(g, midIndex)) {
            g->black &= ~midMask;
            g->black_kings &= ~midMask;
        } else if (!g->white_turn && is_white(g, midIndex)) {
            g->white &= ~midMask;
            g->white_kings &= ~midMask;
        } else {
            // Invalid capture target
            return false;
        }
        // Invalid move
    } else return false;

    // Moving the piece
    if (g->white_turn) {
        if (g->white & from_mask) {
            g->white &= ~from_mask;
            g->white |= to_mask;
        }
        if (g->white_kings & from_mask) {
            g->white_kings &= ~from_mask;
            g->white_kings |= to_mask;
        }
    } else {
        if (g->black & from_mask) {
            g->black &= ~from_mask;
            g->black |= to_mask;
        }
        if (g->black_kings & from_mask) {
            g->black_kings &= ~from_mask;
            g->black_kings |= to_mask;
        }
    }

    // Promoting kings if they reach opposite side
    promote_kings(g);

    // Multi-jump logic
    bool wasCapture = (abs(to / 8 - from / 8) == 2);
    bool canContinue = false;

    if (wasCapture) {
        bool isWhitePiece = g->white_turn; // still same player
        canContinue = can_capture_again(g, to, isWhitePiece);
    }
    // Switching turns
    if (!canContinue) {
        switch_turns(g);
    }
    return true;
}

// Determining if a piece can make another capture after a cpature
bool can_capture_again(GameState *g, int pos, bool isWhite) {
    int row = pos / 8;
    int col = pos % 8;

    // Checking if this piece is a king
    uint64_t mask = 1ULL << pos;
    bool isKing = (g->white_kings & mask) || (g->black_kings & mask);

    // Directions to check
    int dirs[4][2];
    int dirCount = 0;

    if (isKing) {
        // Kings, all four diagonals
        dirs[0][0] =  1; dirs[0][1] =  1;
        dirs[1][0] =  1; dirs[1][1] = -1;
        dirs[2][0] = -1; dirs[2][1] =  1;
        dirs[3][0] = -1; dirs[3][1] = -1;
        dirCount = 4;
    } else {
        // Regular pieces moves forward only
        if (isWhite) {
            dirs[0][0] =  1; dirs[0][1] =  1;
            dirs[1][0] =  1; dirs[1][1] = -1;
        } else {
            dirs[0][0] = -1; dirs[0][1] =  1;
            dirs[1][0] = -1; dirs[1][1] = -1;
        }
        dirCount = 2;
    }

    // Check each allowed direction
    for (int i = 0; i < dirCount; i++) {
        int dr = dirs[i][0];
        int dc = dirs[i][1];
        int midRow = row + dr;
        int midCol = col + dc;
        int endRow = row + dr * 2;
        int endCol = col + dc * 2;

        // Stay on board
        if (midRow < 0 || midRow >= 8 || midCol < 0 || midCol >= 8) continue;
        if (endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8) continue;

        int midIndex = midRow * 8 + midCol;
        int endIndex = endRow * 8 + endCol;

        // Skip if destination is occupied
        if (is_square_occupied(g, endIndex)) continue;

        // Must capture enemy piece
        if (isWhite) {
            if (is_black(g, midIndex)) return true;
        } else {
            if (is_white(g, midIndex)) return true;
        }
    }

    return false;
}

// Move Validation
// Checks if a player has any legal moves left
bool has_any_valid_move(GameState *g, bool isWhite) {
    uint64_t pieces = isWhite ? (g->white | g->white_kings)
                              : (g->black | g->black_kings);

    for (int pos = 0; pos < 64; pos++) {
        uint64_t mask = 1ULL << pos;
        if (!(pieces & mask)) continue;

        int row = pos / 8;
        int col = pos % 8;
        bool isKing = (g->white_kings & mask) || (g->black_kings & mask);

        // Possible directions
        int dirs[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (int i = 0; i < 4; i++) {
            int dr = dirs[i][0];
            int dc = dirs[i][1];

            // Restrict direction for normal pieces
            if (!isKing) {
                if (isWhite && dr <= 0) continue;  // white moves up visually
                if (!isWhite && dr >= 0) continue; // black moves down visually
            }

            // Check simple move
            int r1 = row + dr;
            int c1 = col + dc;
            if (r1 >= 0 && r1 < 8 && c1 >= 0 && c1 < 8) {
                int to = r1 * 8 + c1;
                if (!is_square_occupied(g, to)) return true;
            }

            // Check capture move
            int r2 = row + 2 * dr;
            int c2 = col + 2 * dc;
            if (r2 >= 0 && r2 < 8 && c2 >= 0 && c2 < 8) {
                int mid = (row + r2) / 2 * 8 + (col + c2) / 2;
                int to = r2 * 8 + c2;
                if (is_square_occupied(g, to)) continue;
                if (isWhite && is_black(g, mid)) return true;
                if (!isWhite && is_white(g, mid)) return true;
            }
        }
    }
    return false;
}




// Promotes pieces that reach to the opposite side to kings
void promote_kings(GameState *g) {
    g->white_kings |= g->white & white_promotion_row;
    g->black_kings |= g->black & black_promotion_row;

}

// Checks if it's a winner or draw
bool check_winner(GameState *g) {
    if (g->white == 0 && g->white_kings == 0) {
        printf("Black wins!\n");
        return true;

    }
    if (g->black == 0 && g->black_kings == 0) {
        printf("White wins!\n");
        return true;
    }
    if (!has_any_valid_move(g, true)) {
        printf("Draw!\n");
        return true;
    }
    if (!has_any_valid_move(g, false)) {
        printf("Draw!\n");
        return true;
    }

    return false;
}


// Checks if square is occupied
bool is_square_occupied(const GameState *g, int pos) {
    uint64_t mask = 1ULL << pos;
    return (g->white | g->white_kings | g->black | g->black_kings) & mask;
}

// Checks if the given square contains a white piece (normal or king)
bool is_white(const GameState *g, int pos) {
    uint64_t mask = 1ULL << pos;
    return (g->white | g->white_kings ) & mask;
}

// Checks if the given square contains a black piece (normal or king)
bool is_black(const GameState *g, int pos) {
    uint64_t mask = 1ULL << pos;
    return (g->black | g->black_kings ) & mask;
}
