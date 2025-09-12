#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void div_convert(uint32_t n, int base, char *out);
void sub_convert(uint32_t n, int base, char *out);
void print_tables(uint32_t n);

int main(void) {
    // open the test file
    FILE *file = fopen("a1_test_file.txt", "r");
    // if file cannot be opened report and exit
    if (file == NULL) {
        printf("File not found\n");
        return 0;
    }
    char line[256]; //holds one line
    char function[128]; // function name (div_convert/sub_convert/print_tables)
    int total = 0; // total number of tests counted
    int pass = 0; // number of tests passed
    uint32_t num; // num value from each line
    int base; // base value from each line
    char expected[128]; // expected string if 4 inputs
    char result[128]; // buffer compared against expected

    //read file line by line
    while (fgets(line, sizeof(line), file)) {
        // creating pointer to parse current line
        char *p = line;

        // skip blank or comment lines
        if (*p == '\0') continue;
        if (*p == '#') continue;

        // if there is 4 inputs formatting
        if (sscanf(p,"%127s %u %d %127s", function, &num, &base, expected) ==4) {
            //count test
            total++;

            if (strcmp(function, "div_convert") == 0) {
                div_convert(num, base, result);
            }else if (strcmp(function, "sub_convert") == 0) {
                sub_convert(num, base, result);

                // if there is an unknown function name in file
            }else {
                printf("Unrecognized function: %s\n", function);
                continue;
            }

            // comparing results to expected and printing pass/fail
            int ok = (strcmp(result, expected) == 0);
            printf("Test %d: %s(%u,%d) => [%s]  expect=[%s]  %s\n",
                   total, function, num, base, result, expected, ok ? "[PASS]" : "[FAIL]");
            if (ok) pass++;

        }
        // if 2 inputs formatting
        if (sscanf(p, "print_tables %u", &num) == 1) {
            total++;
            printf("Test %d: print_tables(%u)=> \n", total, num);
            print_tables(num);
            printf("[PASS]\n");
            pass++;
            continue;
        }

    }
    //close file
    fclose(file);

    //summary of results
    printf("\nSummary: %d/%d tests passed\n", pass, total);
    return 0;
}




