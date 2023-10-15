// unit test for xString_findAllChar function in xString module

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    unsigned char sub;
    int expected_count;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, 'a', 0},
        {"cgtatctatagtccgacccacccgcgaacacaaaaatctggctaattaaaaccaaatgtt", 60, 'c', 17},
        {"ccgtccgatgtttccatcgtgtgatggttgatgaccaggtgctaattgtgagctgcaaaa", 60, 'a', 13},
        {"acgtcccagagctgggtctgattcatatgtgcggagtttttgcgacgatccttgtcttga", 60, 't', 20},
        {"gataggaggtgtggctcgcgacccgcaggactcgaacggaagcatctatgggcgggcatc", 60, 'g', 23},
        {"tttgcgaatgcgaacgttacccgacggcttcgcgagggaaccaattagaggctattacag", 60, '\0', 0},
        {"cgctcttactcctgtctatccaatcgtccaatgtttgggaccagcgaaaccgaggttgtg", 60, 99, 17},
        {"gtcgcacctactgctgaactgttaaattgcgtcccagacacgccaggcatgccaatcaga", 60, 0x61, 16},
        {"gaaaggtaagggggccgtcgaggcacgcgactttattacgccacttgggtatgtcgttcg", 60, 'F', 0},
        {"aagagtgaccgaacaagattcttaacgtagtgcagctctgcaatgacactcatacgacgg", 60, 'c', 14}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    int test_case_expected[][61] = {
        {-1},
        {0, 5, 12, 13, 16, 17, 18, 20, 21, 22, 24, 28, 30, 37, 41, 51, 52, -1},
        {7, 15, 23, 30, 33, 36, 43, 44, 50, 56, 57, 58, 59, -1},
        {3, 12, 16, 18, 21, 22, 25, 27, 29, 36, 37, 38, 39, 40, 48, 51, 52, 54, 56, 57, -1},
        {0, 4, 5, 7, 8, 10, 12, 13, 17, 19, 24, 27, 28, 33, 37, 38, 41, 49, 50, 51, 53, 54, 55, -1},
        {-1},
        {0, 2, 4, 8, 10, 11, 15, 19, 20, 24, 27, 28, 40, 41, 44, 49, 50, -1},
        {5, 9, 16, 17, 23, 24, 25, 35, 37, 39, 44, 48, 53, 54, 57, 59, -1},
        {-1},
        {8, 9, 13, 20, 25, 32, 35, 37, 40, 46, 48, 50, 54, 57, -1}
    };

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++){
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        int *actual = xString_findAllChar(str, test_cases[i].sub);
        int actual_count = 0;

        for(int j = 0; actual[j] != -1; j++){
            actual_count++;
        }

        assert(actual_count == test_cases[i].expected_count && "xString_findAllChar() failed: count mismatch");
        for(int j = 0; j < test_cases[i].expected_count; j++) {
            assert(actual[j] == test_case_expected[i][j] && "xString_findAllChar() failed: content mismatch");
        }

        free(actual);
        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}