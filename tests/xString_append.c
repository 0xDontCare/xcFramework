// unit test for xString_append function in xString module

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

int main(void){
    struct test_case test_cases[] = {
        {"", "", "", 0, 0, 0},
        {"", "a", "a", 0, 1, 1},
        {"a", "", "a", 1, 0, 1},
        {"a", "b", "ab", 1, 1, 2},
        {"abc", "def", "abcdef", 3, 3, 6},
        {"hello world", "!", "hello world!", 11, 1, 12},
        {"!", "hello world", "!hello world", 1, 11, 12},
        {"This is a long string", " with a newline\n", "This is a long string with a newline\n", 21, 17, 38},
        {"null\0string", "null\0string", "null\0stringnull\0string", 11, 11, 22},
        {"\0", "\0", "\0\0", 1, 1, 2},
        {"\0a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m", "n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0", "\0a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0mn\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0", 26, 26, 52}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);

    for(int i = 0; i < num_test_cases; i++) {
        xString *str1 = xString_new();

        // first append test
        xString_append(str1, (unsigned char *)(test_cases[i].str1), test_cases[i].len1);

        assert(str1->len == test_cases[i].len1 && "xString_append 1 failed: length mismatch ");
        for(int j = 0; j < test_cases[i].len1; j++) {
            assert(str1->data[j] == test_cases[i].str1[j] && "xString_append 1 failed: content mismatch");
        }

        // second append test
        xString_append(str1, (unsigned char *)(test_cases[i].str2), test_cases[i].len2);

        assert(str1->len == test_cases[i].lenExpected && "xString_append 2 failed: length mismatch ");
        for(int j = 0; j < test_cases[i].lenExpected; j++) {
            assert(str1->data[j] == test_cases[i].expected[j] && "xString_append 2 failed: content mismatch");
        }

        xString_free(str1);

        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
    