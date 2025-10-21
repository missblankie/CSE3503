//
// Created by saraw on 2025-10-20.
//

#include <stdint.h>
#include <stdio.h>

// Sets the bit at the given position to 1
// Returns updated 64 bit value

uint64_t set_bit(uint64_t value, int position) {
    if (position < 0 || position >= 64) return value;
    return value | (1ULL << position);
}

// Clears the bit at given position (sets to 0)
// The updated 64 bit value with the bits cleared

uint64_t clear_bit(uint64_t value, int position) {
    if (position < 0 || position >= 64) return value;
    return value & ~(1ULL << position);
}

// Toggles the bit at given position (1->0 or 0->1)
// The updated 64 bit value with the bit flipped

uint64_t toggle_bit(uint64_t value, int position) {
    if (position < 0 || position >= 64) return value;
    return value ^ (1ULL << position);
}

// Returns the value of the bit at a given position
// Returns 1 if set otherwise 0

int get_bit(uint64_t value, int position) {
    if (position < 0 || position >= 64) return 0;
    return (value >> position);
}

// Shifts the value left by a given number of bits
// Returns 0 if it is greater or equal to 64
// Returns the shifted 64 bit value

uint64_t shift_left(uint64_t value, int position) {
    if (position <= 0) return value;
    if (position >= 64) return 0ULL;
    return value << position;
}

// Shifts the value right by a given number of bits
// Returns 0 if it is greater or equal to 64
// Returns the shifted 64 bit value

uint64_t shift_right(uint64_t value, int position) {
    if (position <= 0) return value;
    if (position >= 64) return 0ULL;
    return value >> position;
}

// Counts how many bits are set to 1 in 64 bit value
// Returns number of set bits

int count_bits(uint64_t value) {
    int count = 0;
    while (value) {
        count += value & 1ULL;
        value >>= 1;
    }
    return count;
}

// Prints the 64 bit value in binary, grouped by bytes
// 64 bit value to print

void print_binary(uint64_t value) {
    for (int i = 63; i >=0; i--) {
        printf("%c", (value &(1ULL << i)) ? '1' : '0');
        if ((i % 8) == 0 && i != 0) printf(" ");
    }
    printf("\n");
}