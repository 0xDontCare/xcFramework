// unit test for xString_remove function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    int rem_start;
    int rem_end;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello, world!", 13, 5, 12, "Hello!", 6},
        {"abcdefg", 7, 0, 2, "cdefg", 5},
        {"12345", 5, 3, 5, "123", 3},
        {"testing", 7, 0, 7, "", 0},
        {"Hello, world!", 13, 15, 20, "Hello, world!", 13},
        {"abcdefg", 7, 3, 3, "abcdefg", 7},
        {"Hello, world!", 13, 9, 5, "Hello, world!", 13},
        {"", 0, 0, 2, "", 0}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_remove(str, test_cases[i].rem_start, test_cases[i].rem_end);

        assert(str->len == test_cases[i].expected_len && "xString_remove() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_remove() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}