// unit test for xString_findLast function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    char *sub;
    int sub_len;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", "", 0, -1},
        {"", "a", 1, -1},
        {"a", "", 0, -1},
        {"a", "a", 1, 0},
        {"a", "b", 1, -1},
        {"ab", "a", 1, 0},
        {"ab", "b", 1, 1},
        {"ab", "ab", 2, 0},
        {"ab", "ba", 2, -1},
        {"abc", "a", 1, 0},
        {"abc", "b", 1, 1},
        {"abc", "c", 1, 2},
        {"abc", "ab", 2, 0},
        {"abc", "bc", 2, 1},
        {"abc", "abc", 3, 0},
        {"abc", "cba", 3, -1},
        {"abc", "abcd", 4, -1},
        {"abc", "defg", 4, -1},
        {"banana", "ana", 3, 3},
        {"This is a test.", "missing", 7, -1},
        {"Empty needles can't be found", "", 0, -1},
        {"Needle in a haystack.", "Needle", 6, 0},
        {"This is the end, my friend.", "friend.", 7, 20},
        {"The pattern is in the pattern", "pattern", 7, 22},
        {"Case sensitivity Matters CASE Sensitivity", "sensitivity", 11, 5},
        {"Short", "LongerNeedle", 12, -1}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_appendCString(str, test_cases[i].str);
        // assuming xString_appendCString works correctly -> no assertions placed

        int actual = xString_findLast(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        assert(actual == test_cases[i].expected);

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}