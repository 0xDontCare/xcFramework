// unit test for xString_replaceLast function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case
{
    char* str;
    int str_len;
    char* sub;
    int sub_len;
    char* replace;
    int replace_len;
    char* expected;
    int expected_len;
};

int main(void)
{
    struct test_case test_cases[] = {
        {"", 0, "", 0, "", 0, "", 0},
        {"", 0, "a", 1, "", 0, "", 0},
        {"a", 1, "", 0, "", 0, "a", 1},
        {"a", 1, "a", 1, "b", 1, "b", 1},
        {"aa", 2, "aa", 2, "", 0, "", 0},
        {"aa", 2, "a", 1, "b", 1, "ab", 2},
        {"abab", 4, "ab", 2, "ba", 2, "abba", 4},
        {"abababba", 8, "ab", 2, "c", 1, "ababcba", 7},
        {"Hello, world! Hello, universe! Hello, galaxy!", 45, "Hello", 5, "Hi", 2, "Hello, world! Hello, universe! Hi, galaxy!", 42},
        {"This is a test.", 15, "missing", 7, "found", 5, "This is a test.", 15},
        {"Replace patterns in this string.", 32, "", 0, "", 0, "Replace patterns in this string.", 32},
        {"Apples are tasty and apples are healthy.", 40, "Apples", 6, "Oranges", 7, "Oranges are tasty and apples are healthy.", 41},
        {"Replace $ with \x80 in this string. $ is a symbol.", 47, "$", 1, "\x80", 1, "Replace $ with \x80 in this string. \x80 is a symbol.", 47},
        {"The cat chased the cattle.", 26, "cat", 3, "dog", 3, "The cat chased the dogtle.", 26},
        {"A is a word.", 12, "A", 1, "Apple", 5, "Apple is a word.", 16},
        {"Replace Word with a word. WORD is different.", 44, "Word", 4, "word", 4, "Replace word with a word. WORD is different.", 44}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for(int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append() works -> no assertions needed

        xString_replaceLast(str, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len, (unsigned char *)test_cases[i].replace, test_cases[i].replace_len);

        assert(str->len == test_cases[i].expected_len && "xString_replaceLast() failed: length mismatch");
        for(int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]) && "xString_replaceLast() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}