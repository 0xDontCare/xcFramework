// unit test for xString_findLastChar function in xString module

#include <assert.h>
#include <stdio.h>
#include "xString.h"

struct test_case {
    char *str;
    char sub;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 'a', -1},
        {"a", 'a', 0},
        {"b", 'a', -1},
        {"ab", 'a', 0},
        {"ab", 'b', 1},
        {"abc", 'a', 0},
        {"abc", 'b', 1},
        {"abc", 'c', 2},
        {"abc", 'd', -1},
        {"Hello World!", 'H', 0},
        {"Hello World!", 'W', 6},
        {"Hello World!", '!', 11},
        {"Hello World!", ' ', 5},
        {"Hello World!", '\0', -1},
        {"The quick brown fox jumps over the lazy dog.", 'T', 0},
        {"The quick brown fox jumps over the lazy dog.", 'q', 4},
        {"The quick brown fox jumps over the lazy dog.", 'b', 10},
        {"The quick brown fox jumps over the lazy dog.", 'f', 16},
        {"The quick brown fox jumps over the lazy dog.", 'j', 20},
        {"The quick brown fox jumps over the lazy dog.", 'o', 41},
        {"The quick brown fox jumps over the lazy dog.", 't', 31},
        {"The quick brown fox jumps over the lazy dog.", 'l', 35},
        {"The quick brown fox jumps over the lazy dog.", 'd', 40},
        {"The quick brown fox jumps over the lazy dog.", ' ', 39}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);
    
    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_appendCString(str, test_cases[i].str);
        // assuming xString_appendCString works correctly -> no assertions placed

        int actual = xString_findLastChar(str, test_cases[i].sub);
        assert(actual == test_cases[i].expected);

        xString_free(str);
        printf("Test %d passed!\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}