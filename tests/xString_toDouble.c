// unit test for xString_toDouble function in xString module

#define EPSILON 1e-11

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    double expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"3.14", 4, 3.14},
        {"0", 1, 0.},
        {"-5.67", 5, -5.67},
        {"12345.6789", 10, 12345.6789},
        {"42", 2, 42.0},
        {"10.0", 4, 10.0},
        {"-0.001", 6, -0.001},
        {"abc", 3, 0.0},
        {"", 0, 0.0},
        {"1.0.0", 5, 1.0},
        {"", 0, 0.0},
        {"3.3333", 6, 3.3333},
        {"-3.56ac", 7, -3.56},
        {"-ab.25", 6, 0.0},
        {".25", 3, 0.25}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        double difference = xString_toDouble(str) - test_cases[i].expected;
        difference = (difference < 0.f) ? -difference : difference;

        assert(difference < EPSILON && "xString_toDouble() failed: content mismatch");

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}