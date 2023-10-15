// unit test for xString_compareIgnoreCase function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str1;
    int str1_len;
    char *str2;
    int str2_len;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"apple", 5, "Banana", 6, -1},
        {"123abc", 6, "abc123", 6, -1},
        {"", 0, "", 0, 0},
        {"", 0, "apple", 5, -1},
        {"banana", 6, "", 0, 1}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str1 = xString_new();
        xString *str2 = xString_new();
        xString_append(str1, (unsigned char *)test_cases[i].str1, test_cases[i].str1_len);
        xString_append(str2, (unsigned char *)test_cases[i].str2, test_cases[i].str2_len);
        // assuming xString_append works -> no assertions placed

        int actual = xString_compareIgnoreCase(str1, str2);

        assert(actual == test_cases[i].expected && "xString_compareIgnoreCase() failed: invalid output");

        xString_free(str1);
        xString_free(str2);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}