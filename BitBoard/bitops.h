//
// Created by saraw on 2025-10-20.
//

#ifndef BITBOARD_BITOPS_H
#define BITBOARD_BITOPS_H
#include <stdint.h>
// Header file for low-level bit manipulation operations



//Sets bit at a given position
uint64_t set_bit(uint64_t value, int position);

// Clears bit at given position
uint64_t clear_bit(uint64_t value, int position);

// retrieves the value of bit
int get_bit(uint64_t value, int position);

// Flips bit at given position
uint64_t toggle_bit(uint64_t value, int position);

// Shift bits right by given num
uint64_t shift_right(uint64_t value, int position);

// Shift bits left by given num
uint64_t shift_left(uint64_t value, int position);

// Counts num of bits set to 1
int count_bits(uint64_t value);

// Prints binary representation of 64 bit bitboard to stdout
void print_binary(uint64_t value);


#endif //BITBOARD_BITOPS_H