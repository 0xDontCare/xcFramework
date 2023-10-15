// unit test for xString_appendChar function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    char c;
    char *expected;
    int len;
    int lenExpected;
};

int main(void){
    struct test_case test_cases[] = {
        {"", 'a', "a", 0, 1},
        {"a", 'b', "ab", 1, 2},
        {"abc", 'd', "abcd", 3, 4},
        {"hello world", '!', "hello world!", 11, 12},
        {"This is a long string", ' ', "This is a long string ", 21, 22},
        {"null\0string", '\0', "null\0string\0", 11, 12},
        {"\0", '\0', "\0\0", 1, 2},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0", '\0', "\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 13, 14},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0a", '\0', "\0\0\0\0\0\0\0\0\0\0\0\0\0a\0", 14, 15},
        {"\0\0\0\0\0\0\0\0\0\0\0\0\0", 'a', "\0\0\0\0\0\0\0\0\0\0\0\0\0a", 13, 14},
        {"banan", 'a', "banana", 5, 6}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);

    for(int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();

        // first append test
        xString_append(str, (unsigned char *)(test_cases[i].str), test_cases[i].len);

        assert(str->len == test_cases[i].len && "xString_append failed: length mismatch ");
        for(int j = 0; j < test_cases[i].len; j++) {
            assert(str->data[j] == test_cases[i].str[j] && "xString_append failed: content mismatch");
        }

        // second append test
        xString_appendChar(str, test_cases[i].c);

        assert(str->len == test_cases[i].lenExpected && "xString_appendChar failed: length mismatch ");
        for(int j = 0; j < test_cases[i].lenExpected; j++) {
            assert(str->data[j] == test_cases[i].expected[j] && "xString_appendChar failed: content mismatch");
        }

        xString_free(str);

        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}