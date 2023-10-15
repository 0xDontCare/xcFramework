// unit test for xString_countCString function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int expected;
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

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works correctly -> no assertions placed

        int actual = xString_countCString(str, test_cases[i].sub);
        assert(actual == test_cases[i].expected && "xString_countCString failed");

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}