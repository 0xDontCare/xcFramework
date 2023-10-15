// unit test for xString_removeAllWhitespace function in xString module

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
        {"Hello world", 11, "Helloworld", 10},
        {"  Remove  Spaces  ", 18, "RemoveSpaces", 12},
        {"NoSpacesHere", 12, "NoSpacesHere", 12},
        {"", 0, "", 0},
        {"   ", 3, "", 0},
        {" \t \n ", 5, "", 0},
        {"123 456", 7, "123456", 6},
        {"!@#$%^&*()", 10, "!@#$%^&*()", 10},
        {"   Hello \t World   ", 19, "HelloWorld", 10},
        {"A B  C   D", 10, "ABCD", 4}};
    int num_test_cases = sizeof(test_cases) / sizeof(struct test_case);

    printf("Starting %d tests...\n", num_test_cases);
    for (int i = 0; i < num_test_cases; i++) {
        xString *str = xString_new();
        xString_append(str, (unsigned char *)test_cases[i].str, test_cases[i].str_len);
        // assuming xString_append works -> no assertions placed

        xString_removeAllWhitespace(str);

        assert(str->len == test_cases[i].expected_len && "xString_removeAllWhitespace() failed: length mismatch");
        for (int j = 0; j < test_cases[i].expected_len; j++) {
            assert(str->data[j] == (unsigned char)test_cases[i].expected[j] && "xString_removeAllWhitespace() failed: content mismatch");
        }

        xString_free(str);
        printf("Test %d passed...\n", i + 1);
    }

    printf("All tests passed!\n");
    return 0;
}