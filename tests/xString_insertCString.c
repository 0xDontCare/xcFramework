// unit test for xString_insertCString function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *instr;
    int insertIndex;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, "abc", 0, "abc", 3},
        {"abc", 3, "", 3, "abc", 3},
        {"abc", 3, "def", 3, "abcdef", 6},
        {"abc", 3, "def", 2, "abdefc", 6},
        {"abcdef", 6, "ghi", 6, "abcdefghi", 9},
        {"abcdef", 6, "ghi", 3, "abcghidef", 9},
        {"abcdef", 6, "ghi", 0, "ghiabcdef", 9},
        {"hello world", 11, "!", 11, "hello world!", 12},
        {"hello world", 11, "!", 0, "!hello world", 12},
        {"abcd", 4, "xyz", 10, "abcd", 4}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_insertCString(str, test_cases[i].instr, test_cases[i].insertIndex);

        assert(str->len == test_cases[i].expected_len && "xString_insertCString() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_insertCString() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}