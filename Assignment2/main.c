#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void oct_to_bin(const char *oct, char *out);
void oct_to_hex(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void to_sign_magnitude(int32_t n, char *out);
void to_ones_complement(int32_t n, char *out);
void to_twos_complement(int32_t n, char *out);
void to_32bit_binary ( uint32_t value , char * out);

int main(void) {

    // Open the test file
    FILE *file = fopen("a2_test.txt", "r");
    // If file cannot be opened report and exit
    if (file == NULL) {
        printf("File not found\n");
        return 0;
    }
    char line[256]; // Holds one line
    char function[128]; // Function name
    int total = 0; // Total number of tests counted
    int pass = 0; // Number of tests passed
    char input[128];
    char expected[128]; // Expected string if 4 inputs
    char result[128]; // Buffer compared against expected

    // Read file line by line
    while (fgets(line, sizeof(line), file)) {
        // Creating pointer to parse current line
        char *p = line;

        // Skip blank or comment lines
        if (*p == '\0') continue;
        if (*p == '#') continue;

        // For three inputs formatting
        if (sscanf(p,"%127s %127s %127s", function, input, expected) ==3) {
            // Count test
            total++;
            // Check for what function needs to be run
            if (strcmp(function, "oct_to_bin") == 0) {
                oct_to_bin(input, result);
            }else if (strcmp(function, "oct_to_hex") == 0) {
                oct_to_hex(input, result);
            }else if (strcmp(function, "hex_to_bin") == 0) {
                hex_to_bin(input, result);
            }else if (strcmp(function, "to_sign_magnitude") == 0) {
                int32_t n = atoi(input);
                to_sign_magnitude(n, result);
            }else if (strcmp(function, "to_ones_complement") == 0) {
                int32_t n = atoi(input);
                to_ones_complement(n, result);
            }else if (strcmp(function, "to_twos_complement") == 0) {
                int32_t n = atoi(input);
                to_twos_complement(n, result);
            } else {
                printf("Unrecognized function: %s\n", function);
                continue;

            }

            // Comparing results to expected and printing pass/fail
            int ok = (strcmp(result, expected) == 0);
            printf("Test %d: %s(%s) -> Expected: \"%s\", Got: \"%s\" [%s]\n",
                   total, function, input, expected, result, ok ? "PASS" : "FAIL");
            if (ok) pass++;

        }


    }
    // Close file
    fclose(file);

    // Summary of results
    printf("\nSummary: %d/%d tests passed\n", pass, total);


    return 0;
}