// unit test for xString_removeLastBlock function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    int sub_len;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, "", 0, "", 0},
        {"", 0, "a", 1, "", 0},
        {"a", 1, "", 0, "a", 1},
        {"a", 1, "a", 1, "", 0},
        {"aa", 2, "aa", 2, "", 0},
        {"aa", 2, "a", 1, "a", 1},
        {"abab", 4, "ab", 2, "ab", 2},
        {"abababba", 8, "ab", 2, "ababba", 6},
        {"Hello, world! Hello, universe!", 30, "Hello", 5, "Hello, world! , universe!", 25},
        {"This is a test.", 15, "missing", 8, "This is a test.", 15},
        {"Remove patterns in this string.", 31, "", 0, "Remove patterns in this string.", 31},
        {"Apples are tasty and apples are healthy.", 40, "Apples", 6, " are tasty and apples are healthy.", 34},
        {"Remove \"$\" in this $ string.", 28, "$", 1, "Remove \"$\" in this  string.", 27},
        {"The cat chased the cattle.", 26, "cat", 3, "The cat chased the tle.", 23},
        {"Replace a with apple.", 21, " a", 2, "Replace a withpple.", 19},
        {"Replace Word with word.", 23, "Word", 4, "Replace  with word.", 19}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeLastBlock(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);

        assert(str->len == test_cases[i].expected_len && "xString_removeLastBlock() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeLastBlock() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}