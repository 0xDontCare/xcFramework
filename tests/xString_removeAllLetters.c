// unit test for xString_removeAllLetters function in xString module

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
        {"Hello123World", 13, "123", 3},
        {"RemoveLetters123", 16, "123", 3},
        {"NoLettersHere!@#$%^&*()", 23, "!@#$%^&*()", 10},
        {"", 0, "", 0},
        {"   ", 3, "   ", 3},
        {" \t \n ", 5, " \t \n ", 5},
        {"!@#$%^&*()", 10, "!@#$%^&*()", 10},
        {"123abc456def", 12, "123456", 6},
        {"A1B2C3D4", 8, "1234", 4}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllLetters(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllLetters() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllLetters() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}