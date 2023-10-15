// unit test for xString_removeAllDigits function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello123World", 13, "HelloWorld", 10},
        {"Remove1234567890Digits", 22, "RemoveDigits", 12},
        {"NoDigitsHere", 12, "NoDigitsHere", 12},
        {"", 0, "", 0},
        {"   ", 3, "   ", 3},
        {" \t \n ", 5, " \t \n ", 5},
        {"!@#$%^&*()", 10, "!@#$%^&*()", 10},
        {"1!2@3#4$5%", 10, "!@#$%", 5},
        {"abc123def456", 12, "abcdef", 6},
        {"A1B2C3D4", 8, "ABCD", 4},
        {"69420", 5, "", 0}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllDigits(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllDigits() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllDigits() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}