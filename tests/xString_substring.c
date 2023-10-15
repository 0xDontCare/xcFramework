// unit test for xString_substring function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    int start;
    int end;
    char *expected;
    int len;
    int lenExpected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, 0, "", 0, 0},
        {"", 0, 1, "", 0, 0},
        {"", 1, 0, "", 0, 0},
        {"", 1, 1, "", 0, 0},
        {"a", 0, 0, "", 1, 0},
        {"a", 0, 1, "a", 1, 1},
        {"a", 1, 0, "", 1, 0},
        {"a", 1, 1, "", 1, 0},
        {"abc", 0, 0, "", 3, 0},
        {"abc", 0, 1, "a", 3, 1},
        {"abc", 0, 2, "ab", 3, 2},
        {"abc", 0, 3, "abc", 3, 3},
        {"abc", 1, 0, "", 3, 0},
        {"abc", 1, 1, "", 3, 0},
        {"abc", 1, 2, "b", 3, 1},
        {"abc", 1, 3, "bc", 3, 2},
        {"abc", 2, 0, "", 3, 0},
        {"abc", 2, 1, "", 3, 0},
        {"abc", 2, 2, "", 3, 0},
        {"abc", 2, 3, "c", 3, 1},
        {"abc", 3, 0, "", 3, 0},
        {"abc", 3, 1, "", 3, 0},
        {"abc", 3, 2, "", 3, 0},
        {"abc", 3, 3, "", 3, 0},
        {"hello world", 0, 0, "", 11, 0},
        {"hello world", 0, 1, "h", 11, 1},
        {"hello world", 0, 2, "he", 11, 2},
        {"hello world", 0, 3, "hel", 11, 3},
        {"hello world", 0, 4, "hell", 11, 4},
        {"hello world", 0, 5, "hello", 11, 5},
        {"hello world", 0, 6, "hello ", 11, 6},
        {"hello world", 0, 7, "hello w", 11, 7},
        {"hello world", 0, 8, "hello wo", 11, 8},
        {"hello world", 0, 9, "hello wor", 11, 9},
        {"hello world", 0, 10, "hello worl", 11, 10},
        {"hello world", 0, 11, "hello world", 11, 11},
        {"hello world", 1, 0, "", 11, 0},
        {"hello world", 1, 1, "", 11, 0},
        {"hello world", 1, 2, "e", 11, 1},
        {"hello world", 1, 3, "el", 11, 2},
        {"hello world", 1, 4, "ell", 11, 3},
        {"hello world", 1, 5, "ello", 11, 4},
        {"hello world", 1, 6, "ello ", 11, 5},
        {"hello world", 1, 7, "ello w", 11, 6},
        {"hello world", 1, 8, "ello wo", 11, 7},
        {"hello world", 1, 9, "ello wor", 11, 8},
        {"hello world", 1, 10, "ello worl", 11, 9},
        {"hello world", 1, 11, "ello world", 11, 10},
        {"hello world", 2, 0, "", 11, 0},
        {"hello world", 2, 1, "", 11, 0},
        {"hello world", 2, 2, "", 11, 0},
        {"hello world", 2, 3, "l", 11, 1},
        {"hello world", 2, 4, "ll", 11, 2},
        {"hello world", 2, 5, "llo", 11, 3},
        {"hello world", 2, 6, "llo ", 11, 4},
        {"hello world", 2, 7, "llo w", 11, 5},
        {"hello world", 2, 8, "llo wo", 11, 6},
        {"hello world", 2, 9, "llo wor", 11, 7},
        {"hello world", 2, 10, "llo worl", 11, 8},
        {"hello world", 2, 11, "llo world", 11, 9}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);

    for(int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();

        xString_append(str, (unsigned char *)(test_cases[i].str), test_cases[i].len);

        assert(str->len == test_cases[i].len && "xString_append failed: length mismatch ");
        for(int j = 0; j < test_cases[i].len; j++) {
            assert(str->data[j] == test_cases[i].str[j] && "xString_append failed: content mismatch");
        }

        xString *substring = xString_substring(str, test_cases[i].start, test_cases[i].end);

        assert(substring->len == test_cases[i].lenExpected && "xString_substring failed: length mismatch ");
        for(int j = 0; j < test_cases[i].lenExpected; j++) {
            assert(substring->data[j] == test_cases[i].expected[j] && "xString_substring failed: content mismatch");
        }

        xString_free(str);
        xString_free(substring);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed\n");
    return 0;
}