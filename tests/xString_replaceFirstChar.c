// unit test for xString_replaceFirstChar function in xString module

#include <stdio.h>
#include <assert.h>
#include "xString.h"

struct test_case
{
    char *strdata;
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
        {"aa", 2, 'a', 'b', "ba", 2},
        {"aac", 3, 'a', '\0', "\0ac", 3},
        {"Hello, world!", 13, ',', '-', "Hello- world!", 13},
        {"This is a test.", 15, 'x', 'y', "This is a test.", 15},
        {"apple", 5, 'a', 'b', "bpple", 5},
        {"$100", 4, '$', '\x80', "\x80""100", 4},
        {"    Spaces at the start.", 24, ' ', '_', "_   Spaces at the start.", 24},
        {"Repeat characters", 17, 'e', 'e', "Repeat characters", 17},
        {"Replace Word with word.", 23, 'W', 'w', "Replace word with word.", 23}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++)
    {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].strdata, test_cases[i].str_len);
        // assuming that xString_append() works -> no assertions needed

        xString_replaceFirstChar(str, test_cases[i].search, test_cases[i].replace);

        assert(str->len == test_cases[i].expected_len && "xString_replaceFirstChar() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++)
        {
            assert(*(str->data + j) == (unsigned char)(test_cases[i].expected[j]) && "xString_replaceFirstChar() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}
