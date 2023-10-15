// unit test for xString_findAll_overlapping function in xString module

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int sub_len;
    int expected_count;
};

int main(void) {
    struct test_case test_cases[] = {
        {"cgtgtcggacagcaacaaacgatatccacgccctacggtgactcacaatgggctacatgc", 60, "", 0, 0},
        {"cgtatctatagtccgacccacccgcgaacacaaaaatctggctaattaaaaccaaatgtt", 60, "c", 1, 17},
        {"ccgtccgatgtttccatcgtgtgatggttgatgaccaggtgctaattgtgagctgcaaaa", 60, "tg", 2, 10},
        {"acgtcccagagctgggtctgattcatatgtgcggagtttttgcgacgatccttgtcttga", 60, "fa", 2, 0},
        {"gataggaggtgtggctcgcgacccgcaggactcgaacggaagcatctatgggcgggcatc", 60, "gg", 2, 9},
        {"tttgcgaatgcgaacgttacccgacggcttcgcgagggaaccaattagaggctattacag", 60, "tt", 2, 6},
        {"cgctcttactcctgtctatccaatcgtccaatgtttgggaccagcgaaaccgaggttgtg", 60, "caat", 4, 2},
        {"gtcgcacctactgctgaactgttaaattgcgtcccagacacgccaggcatgccaatcaga", 60, "ca", 2, 7},
        {"gaaaggtaagggggccgtcgaggcacgcgactttattacgccacttgggtatgtcgttcg", 60, "ggg", 3, 4},
        {"aagagtgaccgaacaagattcttaacgtagtgcagctctgcaatgacactcatacgacgg", 60, "aag", 3, 2},
        {"atttaaccctgccagttcgcccatatccgagtaaccaatatcccttgggacggtacttac", 60, "cc", 2, 9},
        {"cacgagcgacgcaggtataggtcacaggaatatcatcggtgaacattgggcattgccgcg", 60, "gg", 2, 6},
        {"tgtgtccattatattggtcatgctacttcgctGgtagcttgcggacgttagcctcaattc", 60, "tg", 2, 5},
        {"tatcccagttgttatatcagaattaatctggacccatccgccttgaccttcccttgcgct", 60, "t\0", 2, 0},
        {"gccaagaccacttgagagatacggagatatcgagccatgggccctatccctgttctattt", 60, "gag", 3, 4},
        {"gctatgtctctatgagaaattcgtctcagagtcattagacgaattggtcctcgaaccgtc", 60, "tct", 3, 3},
        {"abababababababababababababababababababababababababababababab", 60, "ababab", 6, 28},
        {"abababababababababababababababababababababababababababababab", 60, "bababa", 6, 27},
        {"abababababababababababababababababababababababababababababab", 60, "abc", 3, 0},
        {"tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 60, "tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 60, 1},
        {"banana", 6, "ana", 3, 2},
        {"", 0, "", 0, 0},
        {"", 0, "a", 1, 0}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);
    int test_case_expected[][61] = {
        {-1},
        {0, 5, 12, 13, 16, 17, 18, 20, 21, 22, 24, 28, 30, 37, 41, 51, 52, -1},
        {8, 19, 21, 24, 28, 31, 39, 46, 48, 53, -1},
        {-1},
        {4, 7, 12, 27, 37, 49, 50, 53, 54, -1},
        {0, 1, 16, 28, 44, 54, -1},
        {20, 28, -1},
        {4, 34, 38, 43, 47, 52, 56, -1},
        {9, 10, 11, 46, -1},
        {0, 14, -1},
        {6, 7, 11, 19, 20, 26, 34, 41, 42, -1},
        {13, 19, 26, 37, 47, 48, -1},
        {0, 2, 14, 20, 39, -1},
        {-1},
        {13, 15, 23, 31, -1},
        {6, 8, 23, -1},
        {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, -1},
        {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, -1},
        {-1},
        {0, -1},
        {1, 3, -1},
        {-1},
        {-1}
    };

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        int *actual = xString_findAll_overlapping(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        int actual_count = 0;

        for(int j = 0; actual[j] != -1; j++) {
            actual_count++;
        }

        assert(actual_count == test_cases[i].expected_count && "xString_findAll_overlapping() failed: count mismatch");
        for(int j = 0; j < test_cases[i].expected_count; j++) {
            assert(actual[j] == test_case_expected[i][j] && "xString_findAll_overlapping() failed: content mismatch");
        }

        free(actual);
        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}