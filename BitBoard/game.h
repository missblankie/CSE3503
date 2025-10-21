//
// Created by saraw on 2025-10-20.
//

#ifndef BITBOARD_GAME_H
#define BITBOARD_GAME_H

#include <stdint.h>
#include <stdbool.h>
//Header file for BitBoard Checkers game logic


// Represents entire game state of checkers using 64 bit bitboards
typedef struct{
    // bitboard for each kind of piece
    uint64_t white;
    uint64_t black;
    uint64_t black_kings;
    uint64_t white_kings;
    // True if it's white's turn
    bool white_turn;
} GameState;

//Initialize game
GameState init_game(void);

//switch turns
void switch_turns(GameState *g);

//apply a move if it is valid
bool apply_move(GameState *g, int from, int to);

//promote pieces to king
void promote_kings(GameState *g);

//checks if there is a winner or a draw
bool check_winner(GameState *g);

//checks if square is occupied
bool is_square_occupied(const GameState *g, int pos);

//checks if piece is white
bool is_white(const GameState *g,  int pos);

//checks if piece is black
bool is_black(const GameState *g, int pos);

#endif //BITBOARD_GAME_H