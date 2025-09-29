//
// Created by Sara on 9/28/2025.
//

#include <stdint.h>
#include <string.h>


// Converting an unsigned 32bit int into a binary string of length 32
void to_32bit_binary(uint32_t value , char * out) {
    for (int i = 31; i >= 0; i --) {
        // Shift value right by i and mask lower bit '1' or '0'
        out [31 - i] = (( value >> i) & 1) ? '1' : '0';
    }
    out [32] = '\0';
}


// Converting oct string into binary
void oct_to_bin(const char *oct, char *out){
    int pos = 0;
    for(int i = 0; oct[i] != '\0'; i++){
        // Convert single octal character into it's int value
        int oct_digit = oct[i] - '0';
        //each octal digit corresponds to 3 binary bits
        out[pos++] = (oct_digit >> 2)& 1 ? '1': '0';
        out[pos++] = (oct_digit >> 1)& 1 ? '1': '0';
        out[pos++] = (oct_digit >> 0)& 1 ? '1': '0';
    }
    out[pos] = '\0';
}


// Converting oct input into binary string
void oct_to_hex(const char *oct, char *out){
    char temp[100]; // store temporary binary string
    char padding_bin[100]; //binary with padding
    int i, pos = 0;
    const char hex_map[] = "0123456789ABCDEF"; //table lookup for hex digit

    // Converting oct input to binary
    oct_to_bin(oct, temp);
    int length = strlen(temp);

    // Pad the binary string to ensure its in multiple of 4 bits (Example: 1000000 (7 bits) -> 01000000 (8 bits))
    int padding = (4 - (length % 4)) % 4;
    for(i =0; i < padding; i++){
        padding_bin[i] = '0';
    }
    strcpy(padding_bin + padding, temp);
    int padding_len = length + padding;

    // Processing 4 bits at a time and mapping to hex char
    for(i = 0; i < padding_len; i+= 4) {
        int value = 0;
        for(int j = 0; j < 4; j++){
            value = value * 2 + (padding_bin[i+j] == '1' ? 1 : 0);
        }
        out[pos++] = hex_map[value];
    }
    out[pos] = '\0';

    // Trimming unnecessary leading zeros
    int start = 0;
    while (out[start] == '0' && start < pos-1) {
        start++;
    }
    if (start > 0) {
        memmove(out, out+start, pos-start+1);
    }
}


// Converting a hex string to binary
void hex_to_bin(const char *hex, char *out){
    int pos = 0;

    for(int i = 0; hex[i] != '\0'; i++){
        int value;

        // Converting character to numerical value
        if(hex[i] >= '0' && hex[i] <= '9'){
            value = hex[i] - '0';
        }else if(hex[i] >= 'A' && hex[i] <= 'F'){
            value = hex[i] - 'A' + 10;
        }else if(hex[i] >= 'a' && hex[i] <= 'f'){
            value = hex[i] - 'a' + 10;
        }else
            continue; // skipping invalid chars

        // Each hex digit is 4 binary
        out[pos++] = (value >> 3)& 1 ? '1': '0';
        out[pos++] = (value >> 2)& 1 ? '1': '0';
        out[pos++] = (value >> 1)& 1 ? '1': '0';
        out[pos++] = (value >> 0)& 1 ? '1': '0';
    }
    out[pos] = '\0';
}


// Represent integer n using sign-magnitude form
void to_sign_magnitude(int32_t n, char *out){
    if (n>=0){
        // Positive numbers same as binary
        to_32bit_binary(n, out);
    }else{
        // Negative: MSB = 1, rest = magnitude
        uint32_t mag =(uint32_t)(-n);
        to_32bit_binary(mag, out);
        out[0] = '1';
    }
}

// Representing integer using one's complement
void to_ones_complement(int32_t n, char *out){
    if (n>=0){
        to_32bit_binary((uint32_t)n, out);
    }else{
        uint32_t mag =(uint32_t)(-n);
        to_32bit_binary(mag, out);
        // Flipping all bits
        for(int i = 0; i < 32; i++){
            out[i] = (out[i] == '0') ? '1':'0';
        }
    }
}

// Representing integer using two's complement
void to_twos_complement(int32_t n, char *out) {
    if (n>=0){
        to_32bit_binary((uint32_t)n, out);
    }else{
        uint32_t mag =(uint32_t)(-n);
        to_32bit_binary(mag, out);
        // Flipping all bits
        for (int i = 0; i < 32; i++) {
            out[i] = (out[i] == '0') ? '1' : '0';
        }

        // Add 1 (carry right to left)
        for (int i = 31; i >= 0; i--) {
            if (out[i] == '0') {
                out[i] = '1';
                break; // Carry resolved
            } else {
                out[i] = '0'; // Continue carry left
            }
        }
    }
}

