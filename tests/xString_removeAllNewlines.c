// unit test for xString_removeAllNewlines function in xString module

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
        {"Hello\nWorld", 11, "HelloWorld", 10},
        {"This is a\nmultiline\nstring.", 27, "This is amultilinestring.", 25},
        {"\n\n\n", 3, "", 0},
        {"NoNewlinesHere", 14, "NoNewlinesHere", 14},
        {"Line1\nLine2\nLine3\n\r", 19, "Line1Line2Line3", 15},
        {"Single line", 11, "Single line", 11},
        {"", 0, "", 0},
        {"A\nB\nC\nD\nE", 9, "ABCDE", 5},
        {"The quick brown\nfox\njumps\nover the lazy dog.", 44, "The quick brownfoxjumpsover the lazy dog.", 41},
        {"123\n456\n789\n\n\n0", 15, "1234567890", 10}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllNewlines(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllNewlines() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllNewlines() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}