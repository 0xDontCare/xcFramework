// unit test for xString_insertChar function in xString module

#include <assert.h>
#include <stdio.h>

#include "xString.h"

struct test_case {
    char *str;
    int str_len;
    char inchr;
    int insertIndex;
    char *expected;
    int expected_len;
};

int main(void) {
    struct test_case test_cases[] = {
        {"Hello, world!", 13, '!', 13, "Hello, world!!", 14},
        {"", 0, 'a', 0, "a", 1},
        {"abc", 3, 'd', 3, "abcd", 4},
        {"123456789", 9, 'x', 4, "1234x56789", 10},
        {"Hello, world!", 13, '$', 5, "Hello$, world!", 14},
        {"Hello, world!", 13, ' ', 6, "Hello,  world!", 14},
        {"Hello, world!", 13, '!', 14, "Hello, world!", 13},
        {"Hello, world!", 13, '!', -1, "Hello, world!", 13},
        {"", 0, 'a', -1, "", 0},
        {"Hello, world!", 13, '\n', 6, "Hello,\n world!", 14},
        {"Hello, world!", 13, '\t', 6, "Hello,\t world!", 14},
        {"Hello, world!", 13, '\0', 5, "Hello\0, world!", 14},
        {"Price is 500.", 13, '\x80', 9, "Price is \x80""500.", 14}
    };
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_insertChar(str, (unsigned char)test_cases[i].inchr, test_cases[i].insertIndex);

        assert(str->len == test_cases[i].expected_len && "xString_insertChar() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_insertChar() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}