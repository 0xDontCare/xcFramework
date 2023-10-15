// unit test for xString_toCString function in xString module

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, "", 0},
        {" ", 1, " ", 1},
        {"aaa", 3, "aaa", 3},
        {"Hello, world!", 13, "Hello, world!", 13},
        {"Hello\0, world!", 14, "Hello", 5},
        {"\0Not in CString", 15, "", 0},
        {"\n\t\0\t\n", 5, "\n\t", 2}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        char *cstr = xString_toCString(str);
        int cstr_len = 0;
        while (cstr[cstr_len] != '\0') {
            cstr_len++;
        }

        assert(cstr_len == test_cases[i].expected_len && "xString_toCString() failed: length mismatch");
        assert(cstr[test_cases[i].expected_len] == '\0' && "xString_toCString() failed: bad null-terminator");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(cstr[j] == test_cases[i].expected[j] && "xString_toCString() failed: content mismatch");
        }

        free(cstr);
        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}