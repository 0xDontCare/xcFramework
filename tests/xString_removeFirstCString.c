// unit test for xString_removeFirstCString function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, "", "", 0},
        {"", 0, "a", "", 0},
        {"a", 1, "", "a", 1},
        {"a", 1, "a", "", 0},
        {"aa", 2, "aa", "", 0},
        {"aa", 2, "a", "a", 1},
        {"abab", 4, "ab", "ab", 2},
        {"abababba", 8, "ab", "ababba", 6},
        {"Hello, world! Hello, universe!", 30, "Hello", ", world! Hello, universe!", 25},
        {"This is a test.", 15, "missing", "This is a test.", 15},
        {"Remove patterns in this string.", 31, "", "Remove patterns in this string.", 31},
        {"Apples are tasty and apples are healthy.", 40, "Apples", " are tasty and apples are healthy.", 34},
        {"Remove \"$\" in this string.", 26, "$", "Remove \"\" in this string.", 25},
        {"The cat chased the cattle.", 26, "cat", "The  chased the cattle.", 23},
        {"Replace a with apple.", 21, " a", "Replace with apple.", 19},
        {"Replace Word with word.", 23, "Word", "Replace  with word.", 19}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeFirstCString(str, test_cases[i].sub);

        assert(str->len == test_cases[i].expected_len && "xString_removeFirstCString() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeFirstCString() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}