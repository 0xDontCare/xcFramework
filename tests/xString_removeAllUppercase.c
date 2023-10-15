// unit test for xString_removeAllUppercase function in xString module

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
        {"Hello World", 11, "ello orld", 9},
        {"UPPERCASE", 9, "", 0},
        {"1234ABCD", 8, "1234", 4},
        {"NoUpperCaseHere", 15, "opperaseere", 11},
        {"AllCAPS", 7, "ll", 2},
        {"", 0, "", 0},
        {"lowercase", 9, "lowercase", 9},
        {"123!@#", 6, "123!@#", 6},
        {"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz", 52, "abcdefghijklmnopqrstuvwxyz", 26}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllUppercase(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllUppercase() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllUppercase() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}