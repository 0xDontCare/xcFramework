// unit test for xString_replaceLastCString function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case
{
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
        {"aa", 2, "a", "b", "ab", 2},
        {"abab", 4, "ab", "ba", "abba", 4},
        {"abababba", 8, "ab", "c", "ababcba", 7},
        {"Hello, world! Hello, universe! Hello, galaxy!", 45, "Hello", "Hi", "Hello, world! Hello, universe! Hi, galaxy!", 42},
        {"This is a test.", 15, "missing", "found", "This is a test.", 15},
        {"Replace patterns in this string.", 32, "", "", "Replace patterns in this string.", 32},
        {"Apples are tasty and apples are healthy.", 40, "Apples", "Oranges", "Oranges are tasty and apples are healthy.", 41},
        {"Replace $ with \x80 in this string. $ is a symbol.", 47, "$", "\x80", "Replace $ with \x80 in this string. \x80 is a symbol.", 47},
        {"The cat chased the cattle.", 26, "cat", "dog", "The cat chased the dogtle.", 26},
        {"A is a word.", 12, "A", "Apple", "Apple is a word.", 16},
        {"Replace Word with a word. WORD is different.", 44, "Word", "word", "Replace word with a word. WORD is different.", 44}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...", num_test_cases);

    for(int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append() works -> no assertions needed

        xString_replaceLastCString(str, test_cases[i].sub, test_cases[i].replace);

        assert(str->len == test_cases[i].expected_len);
        for(int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]));
        }

        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}