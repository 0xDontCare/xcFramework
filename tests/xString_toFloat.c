// unit test for xString_toFloat function in xString module

#define EPSILON 1e-6f

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    float expected;
};

int main(void) {
    struct test_case test_cases[] = {
        {"3.14", 4, 3.14f},
        {"0", 1, 0.f},
        {"-5.67", 5, -5.67f},
        {"12345.6789", 10, 12345.6789f},
        {"42", 2, 42.f},
        {"10.0", 4, 10.f},
        {"-0.001", 6, -0.001f},
        {"abc", 3, 0.f},
        {"", 0, 0.f},
        {"1.0.0", 5, 1.f},
        {"", 0, 0.f},
        {"3.3333", 6, 3.3333f},
        {"-3.56ac", 7, -3.56f},
        {"-ab.25", 6, 0.f},
        {".25", 3, 0.25f}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        float difference = xString_toFloat(str) - test_cases[i].expected;
        difference = (difference < 0.f) ? -difference : difference;

        assert(difference < EPSILON && "xString_toFloat() failed: content mismatch");

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}