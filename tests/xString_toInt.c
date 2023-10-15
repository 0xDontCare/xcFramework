// unit test for xString_toInt function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    int expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"", 0, 0},
        {"1", 1, 1},
        {"44", 2, 44},
        {"-1", 2, -1},
        {"-0", 2, 0},
        {"11a", 3, 11},
        {"1a1", 3, 1},
        {"a11", 3, 0},
        {"65535", 5, 65535},
        {"-65535", 6, -65535}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        int actual = xString_toInt(str);

        assert(actual == test_cases[i].expected && "xString_toInt() failed: content mismatch");

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}