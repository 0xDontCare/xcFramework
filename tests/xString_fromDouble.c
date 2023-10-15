// unit test for xString_fromDouble function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    float input;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {3.14, "3.14", 4},
        {0., "0.0", 3},
        {-5.67, "-5.67", 5},
        {12345.6789, "12345.678", 9},
        {42.0, "42.0", 4},
        {10.0, "10.0", 4},
        {-0.001, "-0.001", 6},
        {3.3333, "3.3333", 6},
        {.25, "0.25", 4}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *actual = xString_fromDouble(test_cases[i].input);

        assert(actual->len >= test_cases[i].expected_len && "xString_fromDouble() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(actual->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_fromDouble() failed: content mismatch");
        }

        xString_free(actual);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}