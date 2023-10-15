// unit test for xString_replaceFirstString function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case
{
    char *str;
    int str_len;
    char *sub;
    int sub_len;
    char *replace;
    int replace_len;
    char *expected;
    int expected_len;
};

int main(void)
{
    struct test_case test_cases[] = {
        {"", 0, "", 0, "", 0, "", 0},
        {"", 0, "a", 1, "", 0, "", 0},
        {"a", 1, "", 0, "", 0, "a", 1},
        {"a", 1, "a", 1, "b", 1, "b", 1},
        {"aa", 2, "aa", 2, "", 0, "aa", 2},
        {"aa", 2, "a", 1, "b", 1, "ba", 2},
        {"abab", 4, "ab", 2, "ba", 2, "baab", 4},
        {"abababba", 8, "ab", 2, "c", 1, "cababba", 7},
        {"Hello, world! Hello, universe!", 30, "Hello", 5, "Hi", 2, "Hi, world! Hello, universe!", 27},
        {"This is a test.", 15, "missing", 8, "found", 5, "This is a test.", 15},
        {"Replace patterns in this string.", 32, "", 0, "", 0, "Replace patterns in this string.", 32},
        {"Apples are tasty and apples are healthy.", 40, "Apples", 6, "Oranges", 7, "Oranges are tasty and apples are healthy.", 41},
        {"Replace $ with \x80 in this string.", 32, "$", 1, "\x80", 1, "Replace \x80 with \x80 in this string.", 32},
        {"The cat chased the cattle.", 26, "cat", 3, "dog", 3, "The dog chased the cattle.", 26},
        {"Replace a with apple.", 21, " a", 2, " apple", 6, "Replace apple with apple.", 25},
        {"Replace Word with word.", 23, "Word", 4, "word", 4, "Replace word with word.", 23}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Running %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();
        xString *sub = xString_new();
        xString *replace = xString_new();

        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        xString_append(sub, (unsigned char *)test_cases[i].sub, test_cases[i].sub_len);
        xString_append(replace, (unsigned char *)test_cases[i].replace, test_cases[i].replace_len);
        // assuming that xString_append() works -> no assertions needed

        xString_replaceFirstString(str, sub, replace);

        assert(str->len == test_cases[i].expected_len && "xString_replaceFirstString() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]) && "xString_replaceFirstString() failed: content mismatch");
        }

        xString_free(str);
        xString_free(sub);
        xString_free(replace);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
