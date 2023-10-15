// unit test for xString_replaceLastChar function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case
{
    char *str;
    int str_len;
    unsigned char search;
    unsigned char replace;
    char *expected;
    int expected_len;
};

int main(void)
{
    struct test_case test_cases[] = {
        {"", 0, 'a', 'b', "", 0},
        {"a", 1, 'a', 'b', "b", 1},
        {"aa", 2, 'a', 'b', "ab", 2},
        {"aac", 3, 'a', '\0', "a\0c", 3},
        {"banana", 6, 'a', 'b', "bananb", 6},
        {"abccefgh", 8, 'c', 'd', "abcdefgh", 8},
        {"", 0, 'x', 'y', "", 0},
        {"123451", 6, '1', '6', "123456", 6},
        {"aaaaaaaa", 8, 'b', 'c', "aaaaaaaa", 8}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);

    for(int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append() works -> no assertions needed

        xString_replaceLastChar(str, test_cases[i].search, test_cases[i].replace);

        assert(str->len == test_cases[i].expected_len && "xString_replaceLastChar failed: length mismatch");
        for(int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]) && "xString_replaceLastChar failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}