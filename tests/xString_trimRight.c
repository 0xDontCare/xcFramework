// unit test for xString_trimRight function in xString module

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
        {"   Hello, World!   ", 19, "   Hello, World!", 16},
        {"  \t\t  This is a test  \t  ", 25, "  \t\t  This is a test", 20},
        {"   Hello, world!", 16, "   Hello, world!", 16},
        {"\t\t\tThis is a test", 17, "\t\t\tThis is a test", 17},
        {"Hello, world!    ", 17, "Hello, world!", 13},
        {"This is a test  \t ", 18, "This is a test", 14},
        {"NoWhitespacesTest", 17, "NoWhitespacesTest", 17},
        {" ", 1, "", 0},
        {"Hello,   world!", 15, "Hello,   world!", 15},
        {" \tHello, world!\n\r\t", 18, " \tHello, world!", 15},
        {"", 0, "", 0}

    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_trimRight(str);

        assert(str->len == test_cases[i].expected_len && "xString_trimRight() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_trimRight() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}