// unit test for xString_appendCString function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str1;
    char *str2;
    char *expected;
    int len1;
    int len2;
    int lenExpected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", "", "", 0, 0, 0},
        {"", "a", "a", 0, 1, 1},
        {"a", "", "a", 1, 0, 1},
        {"a", "b", "ab", 1, 1, 2},
        {"abc", "d", "abcd", 3, 1, 4},
        {"hello world", "!", "hello world!", 11, 1, 12},
        {"This is a long string", " with a newline\n", "This is a long string with a newline\n", 21, 16, 37},
        {"null\0string", "\0", "null", 4, 0, 4},
        {"\0", "\0", "", 0, 0, 0},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0a", "\0", "", 0, 0, 0},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0", "a", "a", 0, 1, 1},
        {"banan", "a", "banana", 5, 1, 6},
        {"banan", "ana", "bananana", 5, 3, 8},
        {"banan", "banan", "bananbanan", 5, 5, 10},
        {"banan", "bananana", "bananbananana", 5, 8, 13},
        {"banan", "banananana", "bananbanananana", 5, 10, 15}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);

    for(int i = 0; i < num_test_cases; i++) {
        xString *str1 = xString_new();

        // first append test
        xString_appendCString(str1, test_cases[i].str1);

        assert(str1->len == test_cases[i].len1 && "xString_appendCString 1 failed: length mismatch ");
        for(int j = 0; j < test_cases[i].len1; j++) {
            assert(str1->data[j] == test_cases[i].str1[j] && "xString_appendCString 1 failed: content mismatch");
        }

        // second append test
        xString_appendCString(str1, test_cases[i].str2);

        assert(str1->len == test_cases[i].lenExpected && "xString_appendCString 2 failed: length mismatch ");
        for(int j = 0; j < test_cases[i].lenExpected; j++) {
            assert(str1->data[j] == test_cases[i].expected[j] && "xString_appendCString 2 failed: content mismatch");
        }

        xString_free(str1);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed\n");
    return 0;
}