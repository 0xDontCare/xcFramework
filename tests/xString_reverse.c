// unit test for xString_reverse function in xString module

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
        {"aAbBABab", 8, "baBABbAa", 8},
        {"Hello, world!", 13, "!dlrow ,olleH", 13},
        {"Hello\0, world!", 14, "!dlrow ,\0olleH", 14},
        {"\0Not in CString", 15, "gnirtSC ni toN\0", 15},
        {"\n\t\0\t\n", 5, "\n\t\0\t\n", 5},
        {"SampleMailADDRESS@Provider.com", 30, "moc.redivorP@SSERDDAliaMelpmaS", 30}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);

        xString_reverse(str);

        assert(str->len == test_cases[i].expected_len && "xString_reverse() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_reverse() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}