// unit test for xString_removeAllLowercase function in xString module

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
        {"Hello World", 11, "H W", 3},
        {"UPPERCASE", 9, "UPPERCASE", 9},
        {"1234ABCD", 8, "1234ABCD", 8},
        {"NoUpperCaseHere", 15, "NUCH", 4},
        {"AllCAPS", 7, "ACAPS", 5},
        {"", 0, "", 0},
        {"lowercase", 9, "", 0},
        {"123!@#", 6, "123!@#", 6},
        {"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz", 52, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllLowercase(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllLowercase() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllLowercase() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}