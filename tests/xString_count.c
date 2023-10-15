// unit test for xString_count function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int sub_len;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"cgtgtcggacagcaacaaacgatatccacgccctacggtgactcacaatgggctacatgc", 60, "", 0, 0},
        {"cgtatctatagtccgacccacccgcgaacacaaaaatctggctaattaaaaccaaatgtt", 60, "c", 1, 17},
        {"ccgtccgatgtttccatcgtgtgatggttgatgaccaggtgctaattgtgagctgcaaaa", 60, "tg", 2, 10},
        {"acgtcccagagctgggtctgattcatatgtgcggagtttttgcgacgatccttgtcttga", 60, "fa", 2, 0},
        {"gataggaggtgtggctcgcgacccgcaggactcgaacggaagcatctatgggcgggcatc", 60, "agg", 3, 3},
        {"tttgcgaatgcgaacgttacccgacggcttcgcgagggaaccaattagaggctattacag", 60, "cg", 2, 7},
        {"cgctcttactcctgtctatccaatcgtccaatgtttgggaccagcgaaaccgaggttgtg", 60, "caat", 4, 2},
        {"gtcgcacctactgctgaactgttaaattgcgtcccagacacgccaggcatgccaatcaga", 60, "ca", 2, 7},
        {"gaaaggtaagggggccgtcgaggcacgcgactttattacgccacttgggtatgtcgttcg", 60, "ggg", 3, 2},
        {"aagagtgaccgaacaagattcttaacgtagtgcagctctgcaatgacactcatacgacgg", 60, "aag", 3, 2},
        {"atttaaccctgccagttcgcccatatccgagtaaccaatatcccttgggacggtacttac", 60, "taacc", 5, 2},
        {"cacgagcgacgcaggtataggtcacaggaatatcatcggtgaacattgggcattgccgcg", 60, "gcact", 5, 0},
        {"tgtgtccattatattggtcatgctacttcgctGgtagcttgcggacgttagcctcaattc", 60, "tg", 2, 5},
        {"tatcccagttgttatatcagaattaatctggacccatccgccttgaccttcccttgcgct", 60, "t\0", 2, 0},
        {"gccaagaccacttgagagatacggagatatcgagccatgggccctatccctgttctattt", 60, "gag", 3, 3},
        {"gctatgtctctatgagaaattcgtctcagagtcattagacgaattggtcctcgaaccgtc", 60, "tcgtc", 5, 1},
        {"abababababababababababababababababababababababababababababab", 60, "ababab", 6, 10},
        {"abababababababababababababababababababababababababababababab", 60, "bababa", 6, 9},
        {"abababababababababababababababababababababababababababababab", 60, "abc", 3, 0},
        {"tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 60, "tgatctcggcagcagtaagaaatgtcgccaaccaggggcgctcacaacgagtatagtaga", 60, 1},
        {"banana", 6, "ana", 3, 1},
        {"", 0, "", 0, 0},
        {"", 0, "a", 1, 0}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++){
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works correctly -> no assertions placed

        int actual = xString_count(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        assert(actual == test_cases[i].expected && "xString_count failed");

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}