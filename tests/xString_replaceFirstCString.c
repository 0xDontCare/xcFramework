// unit test for xString_replaceFirstCString function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *sub;
    char *replace;
    char *expected;
    int expected_len; 
};

int main(void) 
{
    struct test_case test_cases[] = {
        {"", 0, "", "", "", 0},
        {"", 0, "a", "", "", 0},
        {"a", 1, "", "", "a", 1},
        {"a", 1, "a", "b", "b", 1},
        {"aa", 2, "aa", "", "", 0},
        {"aa", 2, "a", "b", "ba", 2},
        {"abab", 4, "ab", "ba", "baab", 4},
        {"abababba", 8, "ab", "c", "cababba", 7},
        {"Hello, world! Hello, universe!", 30, "Hello", "Hi", "Hi, world! Hello, universe!", 27},
        {"This is a test.", 15, "missing", "found", "This is a test.", 15},
        {"Replace patterns in this string.", 32, "", "", "Replace patterns in this string.", 32},
        {"Apples are tasty and apples are healthy.", 40, "Apples", "Oranges", "Oranges are tasty and apples are healthy.", 41},
        {"Replace $ with \x80 in this string.", 32, "$", "\x80", "Replace \x80 with \x80 in this string.", 32},
        {"The cat chased the cattle.", 26, "cat", "dog", "The dog chased the cattle.", 26},
        {"Replace a with apple.", 21, " a", " apple", "Replace apple with apple.", 25},
        {"Replace Word with word.", 23, "Word", "word", "Replace word with word.", 23}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();

        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming that xString_append() works -> no assertions needed

        xString_replaceFirstCString(str, test_cases[i].sub, test_cases[i].replace);

        assert(str->len == test_cases[i].expected_len && "xString_replaceFirstString() failed: length mismatch");
        for(int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]) && "xString_replaceFirstString() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
