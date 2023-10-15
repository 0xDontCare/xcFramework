// unit test for xString_findLastString function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    char *sub;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", "", -1},
        {"", "a", -1},
        {"a", "", -1},
        {"a", "a", 0},
        {"a", "b", -1},
        {"ab", "a", 0},
        {"ab", "b", 1},
        {"ab", "ab", 0},
        {"ab", "ba", -1},
        {"abc", "a", 0},
        {"abc", "b", 1},
        {"abc", "c", 2},
        {"abc", "ab", 0},
        {"abc", "bc", 1},
        {"abc", "abc", 0},
        {"abc", "cba", -1},
        {"abc", "abcd", -1},
        {"abc", "defg", -1},
        {"banana", "ana", 3},
        {"This is a test.", "missing", -1},
        {"Empty needles can't be found", "", -1},
        {"Needle in a haystack.", "Needle", 0},
        {"This is the end, my friend.", "friend.", 20},
        {"The pattern is in the pattern", "pattern", 22},
        {"Case sensitivity Matters CASE Sensitivity", "sensitivity", 5},
        {"Short", "LongerNeedle", -1}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString *sub = xString_new();

        xString_appendCString(str, test_cases[i].str);
        xString_appendCString(sub, test_cases[i].sub);
        // assuming xString_appendCString works correctly -> no assertions placed

        int actual = xString_findLastString(str, sub);
        assert(actual == test_cases[i].expected);

        xString_free(str);
        xString_free(sub);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}