// unit test for xString_findAllCString function in xString module

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int expected_count;
};

int main(void) {
    struct test_case test_cases[] = {
        {"cgtgtcggacagcaacaaacgatatccacgccctacggtgactcacaatgggctacatgc", 60, "", 0},
        {"cgtatctatagtccgacccacccgcgaacacaaaaatctggctaattaaaaccaaatgtt", 60, "c", 17},
        {"ccgtccgatgtttccatcgtgtgatggttgatgaccaggtgctaattgtgagctgcaaaa", 60, "tg", 10},
        {"acgtcccagagctgggtctgattcatatgtgcggagtttttgcgacgatccttgtcttga", 60, "fa", 0},
        {"gataggaggtgtggctcgcgacccgcaggactcgaacggaagcatctatgggcgggcatc", 60, "agg", 3},
        {"tttgcgaatgcgaacgttacccgacggcttcgcgagggaaccaattagaggctattacag", 60, "cg", 7},
        {"cgctcttactcctgtctatccaatcgtccaatgtttgggaccagcgaaaccgaggttgtg", 60, "caat", 2},
        {"gtcgcacctactgctgaactgttaaattgcgtcccagacacgccaggcatgccaatcaga", 60, "ca", 7},
        {"gaaaggtaagggggccgtcgaggcacgcgactttattacgccacttgggtatgtcgttcg", 60, "ggg", 2},
        {"aagagtgaccgaacaagattcttaacgtagtgcagctctgcaatgacactcatacgacgg", 60, "aag", 2},
        {"atttaaccctgccagttcgcccatatccgagtaaccaatatcccttgggacggtacttac", 60, "taacc", 2},
        {"cacgagcgacgcaggtataggtcacaggaatatcatcggtgaacattgggcattgccgcg", 60, "gcact", 0},
        {"tgtgtccattatattggtcatgctacttcgctGgtagcttgcggacgttagcctcaattc", 60, "tg", 5},
        {"tatcccagttgttatatcagaattaatctggacccatccgccttgaccttcccttgcgct", 60, "t\0", 20},
        {"gccaagaccacttgagagatacggagatatcgagccatgggccctatccctgttctattt", 60, "gag", 3},
        {"gctatgtctctatgagaaattcgtctcagagtcattagacgaattggtcctcgaaccgtc", 60, "tcgtc", 1},
        {"abababababababababababababababababababababababababababababab", 60, "ababab", 10},
        {"abababababababababababababababababababababababababababababab", 60, "bababa", 9},
        {"abababababababababababababababababababababababababababababab", 60, "abc", 0},
        {"tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 60, "tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 1},
        {"banana", 6, "ana", 1},
        {"", 0, "", 0},
        {"", 0, "a", 0}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);
    int test_case_expected[][61] = {
        {-1},
        {0, 5, 12, 13, 16, 17, 18, 20, 21, 22, 24, 28, 30, 37, 41, 51, 52, -1},
        {8, 19, 21, 24, 28, 31, 39, 46, 48, 53, -1},
        {-1},
        {3, 6, 26, -1},
        {4, 10, 14, 21, 24, 30, 32, -1},
        {20, 28, -1},
        {4, 34, 38, 43, 47, 52, 56, -1},
        {9, 46, -1},
        {0, 14, -1},
        {3, 31, -1},
        {-1},
        {0, 2, 14, 20, 39, -1},
        {0, 2, 8, 9, 11, 12, 14, 16, 22, 23, 26, 28, 36, 42, 43, 48, 49, 53, 54, 59, -1},
        {13, 23, 31, -1},
        {20, -1},
        {0, 6, 12, 18, 24, 30, 36, 42, 48, 54, -1},
        {1, 7, 13, 19, 25, 31, 37, 43, 49, -1},
        {-1},
        {0, -1},
        {1, -1},
        {-1},
        {-1}
    };

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        int *actual = xString_findAllCString(str, test_cases[i].sub);
        int actual_count = 0;

        for(int j = 0; actual[j] != -1; j++) {
            actual_count++;
        }

        assert(actual_count == test_cases[i].expected_count && "xString_findAllCString() failed: count mismatch");
        for(int j = 0; j < test_cases[i].expected_count; j++) {
            assert(actual[j] == test_case_expected[i][j] && "xString_findAllCString() failed: content mismatch");
        }

        free(actual);
        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}