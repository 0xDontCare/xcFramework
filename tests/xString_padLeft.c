// unit test for xString_padLeft function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char padChar;
    int targetLen;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello", 5, '0', 8, "000Hello", 8},
        {"12345", 5, '-', 7, "--12345", 7},
        {"", 0, '*', 5, "*****", 5},
        {"Testing", 7, ' ', 7, "Testing", 7},
        {"Overflow", 8, '@', 6, "Overflow", 8},
        {"Sample", 6, '#', 10, "####Sample", 10}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_padLeft(str, (unsigned char)test_cases[i].padChar, test_cases[i].targetLen);

        assert(str->len == test_cases[i].expected_len && "xString_padLeft() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_padLeft() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}