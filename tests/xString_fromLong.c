// unit test for xString_fromLong function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    long input;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {0l, "0", 1},
        {1l, "1", 1},
        {44l, "44", 2},
        {-1l, "-1", 2},
        {-0l, "0", 1},
        {65535l, "65535", 5},
        {-65536l, "-65536", 6},
        {2147483647l, "2147483647", 10}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *actual = xString_fromLong(test_cases[i].input);

        assert(actual->len == test_cases[i].expected_len && "xString_fromLong() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(actual->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_fromLong() failed: content mismatch");
        }

        xString_free(actual);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}