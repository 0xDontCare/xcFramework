// unit test for xString_removeAllSpecial function in xString module

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
        {"Hello! @123# World$", 19, "Hello 123 World", 15},
        {"RemoveSpecial$%^&*() Characters", 31, "RemoveSpecial Characters", 24},
        {"NoSpecialCharsHere", 18, "NoSpecialCharsHere", 18},
        {"", 0, "", 0},
        {"   ", 3, "   ", 3},
        {" \t \n ", 5, " \t \n ", 5},
        {"!@#$%^&*()", 10, "", 0},
        {"A1! B2@ C3# D4$", 15, "A1 B2 C3 D4", 11}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllSpecial(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllSpecial() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllSpecial() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}