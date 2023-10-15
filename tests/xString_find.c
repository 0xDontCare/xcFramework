// unit test for xString_find function in xString module

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
        {"Hello World!", "Hello", 5, 0},
        {"Hello World!", "World", 5, 6},
        {"Hello World!", "!", 1, 11},
        {"Hello World!", "Hello World!", 12, 0},
        {"Hello World!", "Hello World! ", 13, -1},
        {"Hello World!", "Hello World!  ", 14, -1},
        {"The quick brown fox jumps over the lazy dog.", "The", 3, 0},
        {"The quick brown fox jumps over the lazy dog.", "quick", 5, 4},
        {"The quick brown fox jumps over the lazy dog.", "brown", 5, 10},
        {"The quick brown fox jumps over the lazy dog.", "fox", 3, 16},
        {"The quick brown fox jumps over the lazy dog.", "jumps", 5, 20},
        {"The quick brown fox jumps over the lazy dog.", "over", 4, 26},
        {"The quick brown fox jumps over the lazy dog.", "the", 3, 31},
        {"The quick brown fox jumps over the lazy dog.", "lazy", 4, 35},
        {"The quick brown fox jumps over the lazy dog.", "dog.", 4, 40},
        {"The quick brown fox jumps over the lazy dog.", "The quick brown fox jumps over the lazy dog.", 44, 0},
        {"The quick brown fox jumps over the lazy dog.", "The quick brown  fox jumps over the lazy dog.", 45, -1},
        {"The quick brown fox jumps over the lazy dog.", " ", 1, 3}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_appendCString(str, test_cases[i].str);
        // assuming xString_appendCString works correctly -> no assertions placed

        int actual = xString_find(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        assert(actual == test_cases[i].expected && "xString_find failed!");

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
