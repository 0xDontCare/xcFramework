// unit test for xString_removeAllString function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int sub_len;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello, world!", 13, "o", 1, "Hell, wrld!", 11},
        {"banana", 6, "an", 2, "ba", 2},
        {"ababababa", 9, "aba", 3, "bba", 3},
        {"Programming is fun.", 19, "ing", 3, "Programm is fun.", 16},
        {"aaaaaa", 6, "aa", 2, "", 0},
        {"No patterns here", 16, "123", 3, "No patterns here", 16},
        {"", 0, "xyz", 3, "", 0},
        {"patternpatternpattern", 21, "pattern", 7, "", 0},
        {"aabbccdd", 8, "z", 1, "aabbccdd", 8},
        {"This is a test", 14, " ", 1, "Thisisatest", 11},
        {"aaaaaa", 6, "", 0, "aaaaaa", 6},
        {"aaaaaa", 6, "a", 1, "", 0},
        {"abcdefg", 7, "xyz", 3, "abcdefg", 7},
        {"patternpattern", 14, "patternpattern", 14, "", 0},
        {"AaAaAaA", 7, "a", 1, "AAAA", 4},
        {"oneonetwothree", 14, "one", 3, "twothree", 8}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString *sub = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        xString_append(sub, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllString(str, sub);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllString() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllString() failed: content mismatch");
        }

        xString_free(str);
        xString_free(sub);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}