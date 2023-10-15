// unit test for xString_copy function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    char *expected;
    int len;
    int lenExpected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", "", 0, 0},
        {"a", "a", 1, 1},
        {"abc", "abc", 3, 3},
        {"hello world", "hello world", 11, 11},
        {"This is a long string", "This is a long string", 21, 21},
        {"null\0string", "null\0string", 11, 11},
        {"\0", "\0", 1, 1},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0a", "\0\0\0\0\0\0\0\0\0\0\0\0\0a", 14, 14},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0\0\0\0\0\0", 13, 13},
        {"banan", "banan", 5, 5},
        {"bananana", "bananana", 8, 8},
        {"banananana", "banananana", 10, 10}
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

        xString *copy = xString_copy(str);

        assert(copy->len == test_cases[i].lenExpected && "xString_copy failed: length mismatch ");
        for(int j = 0; j < test_cases[i].lenExpected; j++) {
            assert(copy->data[j] == test_cases[i].expected[j] && "xString_copy failed: content mismatch");
        }

        xString_free(str);
        xString_free(copy);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed\n");
    return 0;
}
