// unit test for xString_toLowercase function in xString module

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
        {"aAbBABab", 8, "aabbabab", 8},
        {"Hello, world!", 13, "hello, world!", 13},
        {"Hello\0, world!", 14, "hello\0, world!", 14},
        {"\0Not in CString", 15, "\0not in cstring", 15},
        {"\n\t\0\t\n", 5, "\n\t\0\t\n", 5},
        {"SampleMailADDRESS@Provider.com", 30, "samplemailaddress@provider.com", 30}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        xString_toLowercase(str);

        assert(str->len == test_cases[i].expected_len && "xString_toLowercase() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_toLowercase() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}