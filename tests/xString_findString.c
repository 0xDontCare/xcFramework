// unit test for xString_findString function in xString module

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
        {"Hello World!", "Hello", 0},
        {"Hello World!", "World", 6},
        {"Hello World!", "!", 11},
        {"Hello World!", "Hello World!", 0},
        {"Hello World!", "Hello World! ", -1},
        {"Hello World!", "Hello World!  ", -1},
        {"The quick brown fox jumps over the lazy dog.", "The", 0},
        {"The quick brown fox jumps over the lazy dog.", "quick", 4},
        {"The quick brown fox jumps over the lazy dog.", "brown", 10},
        {"The quick brown fox jumps over the lazy dog.", "fox", 16},
        {"The quick brown fox jumps over the lazy dog.", "jumps", 20},
        {"The quick brown fox jumps over the lazy dog.", "over", 26},
        {"The quick brown fox jumps over the lazy dog.", "the", 31},
        {"The quick brown fox jumps over the lazy dog.", "lazy", 35},
        {"The quick brown fox jumps over the lazy dog.", "dog.", 40},
        {"The quick brown fox jumps over the lazy dog.", "The quick brown fox jumps over the lazy dog.", 0},
        {"The quick brown fox jumps over the lazy dog.", "The quick brown  fox jumps over the lazy dog.", -1},
        {"The quick brown fox jumps over the lazy dog.", " ", 3}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++){
        xString *str1 = xString_new();
        xString *str2 = xString_new();

        xString_appendCString(str1, test_cases[i].str);
        xString_appendCString(str2, test_cases[i].sub);
        // assuming xString_appendCString works correctly -> no assertions placed

        int actual = xString_findString(str1, str2);
        assert(actual == test_cases[i].expected && "xString_findString failed!");

        xString_free(str1);
        xString_free(str2);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}