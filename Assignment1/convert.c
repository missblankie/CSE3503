//
// Created by Sara on 9/9/2025.
//
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void div_convert(uint32_t n, int base, char *out){
    char temp[65];
    int pos = 0;

    // Handle zero case
    if ( n == 0) {
        strcpy ( out , "0") ;
        return ;
    }


    //Extract digits from right to left
    while (n > 0) {
        int remainder = n % base;
        n = n / base;

        //Convert digit to character
        if (remainder < 10)
            temp[pos++] = '0' + remainder;
        else
            temp[pos++] = 'A' + (remainder - 10);

    }
    //Reverse the result
    for (int i = pos - 1, j=0; i >= 0; i--,j++) {
        out[j] = temp[i];
    }
    out[pos] = '\0';
}
void sub_convert(uint32_t n, int base, char *out) {
    int pos = 0;
    uint32_t power = 1;

    // Handle zero case
    if ( n == 0) {
        strcpy ( out , "0") ;
        return ;
    }

    while (power <= n / base) {
        power *= base;
    }

    int start = 0;

    while (power > 0) {
        uint32_t num = n / power;
        if (start || num > 0)
            if (num < 10)
                out[pos++] = '0' + num;
            else
                out[pos++] = 'A' + (num - 10);
        start = 1;

    n = n - num * power;
    power = power/base;
    }
    out[pos] = 0;

}

void print_tables(uint32_t n) {
    char bin[33], oct[12], hex[9];
    // Original number
    div_convert(n, 2, bin);
    div_convert(n, 8, oct);
    div_convert(n, 16, hex);
    printf("Original: Binary=%s Octal=%s Decimal=%u Hex=%s\n", bin, oct, n, hex);

    // Left shift by 3
    uint32_t shifted = n << 3;
    div_convert ( shifted , 2 , bin ) ;
    div_convert ( shifted , 8 , oct ) ;
    div_convert ( shifted , 16 , hex ) ;
    printf ("Left Shift by 3: Binary =%s Octal =%s Decimal =%u Hex =%s\n", bin , oct , shifted , hex ) ;

    // AND with 0 xFF
    uint32_t masked = n & 0xFF;
    div_convert ( masked , 2 , bin ) ;
    div_convert ( masked , 8 , oct ) ;
    div_convert ( masked , 16 , hex ) ;
    printf ("And with 0xFF: Binary =%s Octal =%s Decimal =%u Hex =%s\n", bin , oct , masked , hex ) ;


}