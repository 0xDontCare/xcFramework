// unit test for xString_removeAllCString function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello, world!", 13, "o", "Hell, wrld!", 11},
        {"banana", 6, "an", "ba", 2},
        {"ababababa", 9, "aba", "bba", 3},
        {"Programming is fun.", 19, "ing", "Programm is fun.", 16},
        {"aaaaaa", 6, "aa", "", 0},
        {"No patterns here", 16, "123", "No patterns here", 16},
        {"", 0, "xyz", "", 0},
        {"patternpatternpattern", 21, "pattern", "", 0},
        {"aabbccdd", 8, "z", "aabbccdd", 8},
        {"This is a test", 14, " ", "Thisisatest", 11},
        {"aaaaaa", 6, "", "aaaaaa", 6},
        {"aaaaaa", 6, "a", "", 0},
        {"abcdefg", 7, "xyz", "abcdefg", 7},
        {"patternpattern", 14, "patternpattern", "", 0},
        {"AaAaAaA", 7, "a", "AAAA", 4},
        {"oneonetwothree", 14, "one", "twothree", 8}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllCString(str, test_cases[i].sub);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllCString() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllCString() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}