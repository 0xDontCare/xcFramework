/**
 * @file xString_test.c
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief CUnit test for xString module.
 * @version 0.1
 * @date 18.08.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include <math.h>
#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"
#include "xMemory/xDefer.h"
#include "xString/xString.h"

void test_cstrlen(void)
{
    // Test case 1: Length of string with multiple characters
    CU_ASSERT_EQUAL(cstrlen("Hello, World!"), 13);

    // Test case 2: Length of empty string
    CU_ASSERT_EQUAL(cstrlen(""), 0);

    // Test case 3: Length of string with single character
    CU_ASSERT_EQUAL(cstrlen("A"), 1);

    // Test case 4: Length of string with special characters
    CU_ASSERT_EQUAL(cstrlen("1234567890"), 10);

    // Test case 5: Length of string with non-printable characters
    CU_ASSERT_EQUAL(cstrlen("\x01\x52\x10"), 3);

    // Test case 6: NULL address string
    CU_ASSERT_EQUAL(cstrlen(NULL), 0);
}

void test_xString_new(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    // Test case 1: Initial state of xString object
    CU_ASSERT_NOT_EQUAL(str, NULL);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);
    CU_ASSERT_EQUAL(xString_getCapacity(str), 0);
    CU_ASSERT_EQUAL(xString_getData(str), NULL);
}

void test_xString_optimize(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    xString_optimize(str);
    CU_ASSERT_EQUAL(xString_getCapacity(str), 0);
}

void test_xString_preallocate(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    // Test case 1: Preallocate capacity for empty string
    xString_preallocate(str, 10);
    CU_ASSERT_EQUAL(xString_getCapacity(str), 10);

    // Test case 2: Expand preallocated capacity
    xString_preallocate(str, 5);
    CU_ASSERT_EQUAL(xString_getCapacity(str), 15);

    // Test case 3: Preallocate zero characters
    xString_preallocate(str, 0);
    CU_ASSERT_EQUAL(xString_getCapacity(str), 15);
}

void test_xString_append(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Append to NULL address string
    str = xString_append(NULL, "Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xString_getCapacity(str) >= 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    xString_clear(str);

    // Test case 2: Append to empty string
    str = xString_append(str, "Hello, ", 7);
    DEFER(xString_free, str);  // xString_append() creates new xString object which needs to be freed
    CU_ASSERT_EQUAL(xString_getLength(str), 7);
    CU_ASSERT_TRUE(xString_getCapacity(str) >= 7);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, ", 7));

    // Test case 3: Append to non-empty string
    str = xString_append(str, "World!", 6);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xString_getCapacity(str) >= 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 4: Append empty string
    str = xString_append(str, "", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xString_getCapacity(str) >= 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 5: Append to string with preallocated capacity
    str = xString_append(str, "!", 1);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 14);
    CU_ASSERT_TRUE(xString_getCapacity(str) >= 14);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!!", 14));
}

void test_xString_substring(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    xString *sub = NULL;
    DEFER(xString_free, str);

    // base string: "Hello, World!"
    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 1: Substring from the beginning of the string
    sub = xString_substring(str, 0, 5);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(sub), (const void *)"Hello", 5));

    // Test case 2: Substring from the middle of the string
    sub = xString_substring(str, 7, 12);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(sub), (const void *)"World", 5));

    // Test case 3: Substring from entire string
    sub = xString_substring(str, 0, 13);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(sub), (const void *)"Hello, World!", 13));

    // Test case 4: End index out of bounds
    sub = xString_substring(str, 0, 15);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(sub), (const void *)"Hello, World!", 13));

    // Test case 5: Start and end index at the same position
    sub = xString_substring(str, 0, 0);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 0);
    CU_ASSERT_EQUAL(xString_getData(sub), NULL);

    // Test case 6: Start index greater than end index
    sub = xString_substring(str, 13, 0);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 0);
    CU_ASSERT_EQUAL(xString_getData(sub), NULL);

    // Test case 7: Start index out of bounds
    sub = xString_substring(str, 14, 0);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 0);
    CU_ASSERT_EQUAL(xString_getData(sub), NULL);

    // Test case 8: NULL string address
    sub = xString_substring(NULL, 0, 5);
    DEFER(xString_free, sub);
    CU_ASSERT_EQUAL(xString_getLength(sub), 0);
    CU_ASSERT_EQUAL(xString_getData(sub), NULL);
}

void test_xString_find(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 1: Pattern found at the beginning of the string
    xSize index = xString_find(str, "Hello", 5);
    CU_ASSERT_EQUAL(index, 0);

    // Test case 2: Pattern found in the middle of the string
    index = xString_find(str, "World", 5);
    CU_ASSERT_EQUAL(index, 7);

    // Test case 3: Pattern found at the end of the string
    index = xString_find(str, "!", 1);
    CU_ASSERT_EQUAL(index, 12);

    // Test case 4: Pattern not found in the string
    index = xString_find(str, "abc", 3);
    CU_ASSERT_EQUAL(index, XSIZE_MAX);

    // Test case 5: Empty pattern
    index = xString_find(str, "", 0);
    CU_ASSERT_EQUAL(index, 0);

    // Test case 6: Pattern length greater than string length
    index = xString_find(str, "Hello, World!123", 16);
    CU_ASSERT_EQUAL(index, XSIZE_MAX);

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);

    // Test case 7: Pattern found multiple times in the string
    index = xString_find(str, "cab", 3);
    CU_ASSERT_EQUAL(index, 2);
}

void test_xString_findLast(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 1: Pattern found at the beginning of the string
    xSize index = xString_findLast(str, "Hello", 5);
    CU_ASSERT_EQUAL(index, 0);

    // Test case 2: Pattern found in the middle of the string
    index = xString_findLast(str, "World", 5);
    CU_ASSERT_EQUAL(index, 7);

    // Test case 3: Pattern found at the end of the string
    index = xString_findLast(str, "!", 1);
    CU_ASSERT_EQUAL(index, 12);

    // Test case 4: Pattern not found in the string
    index = xString_findLast(str, "abc", 3);
    CU_ASSERT_EQUAL(index, XSIZE_MAX);

    // Test case 5: Empty pattern
    index = xString_findLast(str, "", 0);
    CU_ASSERT_EQUAL(index, xString_getLength(str) - 1);

    // Test case 6: Pattern length greater than string length
    index = xString_findLast(str, "Hello, World!123", 16);
    CU_ASSERT_EQUAL(index, XSIZE_MAX);

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);

    // Test case 7: Pattern found multiple times in the string
    index = xString_findLast(str, "cab", 3);
    CU_ASSERT_EQUAL(index, 8);
}

void test_xString_count(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 1: Pattern found once in the string
    xSize count = xString_count(str, "Hello", 5);
    CU_ASSERT_EQUAL(count, 1);

    // Test case 2: Pattern found multiple times in the string
    count = xString_count(str, "o", 1);
    CU_ASSERT_EQUAL(count, 2);

    // Test case 3: Pattern not found in the string
    count = xString_count(str, "abc", 3);
    CU_ASSERT_EQUAL(count, 0);

    // Test case 4: Empty pattern
    count = xString_count(str, "", 0);
    CU_ASSERT_EQUAL(count, 0);

    // Test case 5: Pattern length greater than string length
    count = xString_count(str, "Hello, World!123", 16);
    CU_ASSERT_EQUAL(count, 0);

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);

    // Test case 6: Pattern found multiple times in the string with overlapping occurrences
    count = xString_count(str, "abc", 3);
    CU_ASSERT_EQUAL(count, 4);
}
void test_xString_count_overlapping(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 1: Pattern found once in the string
    xSize count = xString_count_overlapping(str, "Hello", 5);
    CU_ASSERT_EQUAL(count, 1);

    // Test case 2: Pattern found multiple times in the string
    count = xString_count_overlapping(str, "o", 1);
    CU_ASSERT_EQUAL(count, 2);

    // Test case 3: Pattern not found in the string
    count = xString_count_overlapping(str, "abc", 3);
    CU_ASSERT_EQUAL(count, 0);

    // Test case 4: Empty pattern
    count = xString_count_overlapping(str, "", 0);
    CU_ASSERT_EQUAL(count, 0);

    // Test case 5: Pattern length greater than string length
    count = xString_count_overlapping(str, "Hello, World!123", 16);
    CU_ASSERT_EQUAL(count, 0);

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);

    // Test case 6: Pattern found multiple times in the string with no overlapping occurrences
    count = xString_count_overlapping(str, "abc", 3);
    CU_ASSERT_EQUAL(count, 4);

    xString_clear(str);
    str = xString_append(str, "abababababababa", 15);
    DEFER(xString_free, str);

    // Test case 7: Pattern found multiple times in the string with overlapping occurrences
    count = xString_count_overlapping(str, "aba", 3);
    CU_ASSERT_EQUAL(count, 7);
}

void test_xString_replaceFirst(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 1: Pattern found at the beginning of the string
    str = xString_replaceFirst(str, "Hello", 5, "Hi", 2);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 10);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, World!", 10));

    // Test case 2: Pattern found in the middle of the string
    str = xString_replaceFirst(str, "World", 5, "Universe", 8);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe!", 13));

    // Test case 3: Pattern found at the end of the string
    str = xString_replaceFirst(str, "!", 1, "?", 1);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 4: Pattern not found in the string
    str = xString_replaceFirst(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 5: Empty pattern
    str = xString_replaceFirst(str, "", 0, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 6: Pattern length greater than string length
    str = xString_replaceFirst(str, "Hello, World!123", 16, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 7: Replace pattern with empty string
    str = xString_replaceFirst(str, "Universe", 8, "", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, ?", 5));

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"abcabcabcabc", 12));

    // Test case 8: Pattern found multiple times in the string
    str = xString_replaceFirst(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 12);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"123abcabcabc", 12));
}

void test_xString_replaceLast(void)
{
    XDEFER_SCOPE
    xString *str = xString_new();
    DEFER(xString_free, str);

    str = xString_append(str, "Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 1: Pattern found at the beginning of the string
    str = xString_replaceLast(str, "Hello", 5, "Hi", 2);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 10);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, World!", 10));

    // Test case 2: Pattern found in the middle of the string
    str = xString_replaceLast(str, "World", 5, "Universe", 8);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe!", 13));

    // Test case 3: Pattern found at the end of the string
    str = xString_replaceLast(str, "!", 1, "?", 1);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 4: Pattern not found in the string
    str = xString_replaceLast(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 5: Empty pattern
    str = xString_replaceLast(str, "", 0, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 6: Pattern length greater than string length
    str = xString_replaceLast(str, "Hello, World!123", 16, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 7: Replace pattern with empty string
    str = xString_replaceLast(str, "Universe", 8, "", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, ?", 5));

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"abcabcabcabc", 12));

    // Test case 8: Pattern found multiple times in the string
    str = xString_replaceLast(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 12);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"abcabcabc123", 12));
}

void test_xString_replaceAll(void)
{
    XDEFER_SCOPE
    xString *str = xString_fromCStringS("Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 1: Pattern found at the beginning of the string
    str = xString_replaceAll(str, "Hello", 5, "Hi", 2);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 10);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, World!", 10));

    // Test case 2: Pattern found in the middle of the string
    str = xString_replaceAll(str, "World", 5, "Universe", 8);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe!", 13));

    // Test case 3: Pattern found at the end of the string
    str = xString_replaceAll(str, "!", 1, "?", 1);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 4: Pattern not found in the string
    str = xString_replaceAll(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 5: Empty pattern
    str = xString_replaceAll(str, "", 0, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 6: Pattern length greater than string length
    str = xString_replaceAll(str, "Hello, World!123", 16, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe?", 13));

    // Test case 7: Replace pattern with empty string
    str = xString_replaceAll(str, "Universe", 8, "", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, ?", 5));

    xString_clear(str);
    str = xString_append(str, "abcabcabcabc", 12);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"abcabcabcabc", 12));

    // Test case 8: Pattern found multiple times in the string
    str = xString_replaceAll(str, "abc", 3, "123", 3);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 12);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"123123123123", 12));
}

void test_xString_remove(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Base string: "Hello, World!"
    str = xString_fromCStringS("Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 1: Remove segment from the beginning of the string
    str = xString_remove(str, 0, 5);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 8);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)", World!", 8));

    // Test case 2: Remove segment from the middle of the string
    str = xString_remove(str, 3, 5);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 6);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)", Wld!", 6));

    // Test case 3: Remove segment from the end of the string
    str = xString_remove(str, 2, 4);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 4);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)", d!", 4));

    // Test case 4: Remove entire string
    str = xString_remove(str, 0, xString_getLength(str));
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"", 0));

    // Reset base string
    str = xString_fromCStringS("Hello, World!", 13);
    DEFER(xString_free, str);

    // Test case 5: Remove segment with invalid indices
    str = xString_remove(str, 5, 2);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 6: Remove segment with start index greater than string length
    str = xString_remove(str, 20, 25);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 7: Remove segment with end index greater than string length
    str = xString_remove(str, 5, 20);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello", 5));
}

void test_xString_insert(void)
{
    XDEFER_SCOPE
    xString *str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    CU_ASSERT_TRUE_FATAL(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 1: Insert at the beginning of the string
    str = xString_insert(str, "Hi, ", 4, 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 17);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Hello, World!", 17));

    // Test case 2: Insert in the middle of the string
    str = xString_insert(str, "Universe, ", 10, 4);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 27);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe, Hello, World!", 27));

    // Test case 3: Insert at the end of the string
    str = xString_insert(str, "!", 1, 27);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 28);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe, Hello, World!!", 28));

    // Test case 4: Insert empty string
    str = xString_insert(str, "", 0, 28);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 28);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe, Hello, World!!", 28));

    // Test case 5: Insert at invalid index
    str = xString_insert(str, "123", 3, 30);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 28);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hi, Universe, Hello, World!!", 28));
}

void test_xString_compare(void)
{
    XDEFER_SCOPE
    xString *str1 = NULL;
    xString *str2 = NULL;

    // Test case 1: Equal strings
    str1 = xString_fromCString("Hello, World!");
    str2 = xString_fromCString("Hello, World!");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) == 0);

    // Test case 2: Different strings
    str2 = xString_fromCString("Hello, Universe!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) > 0);

    // Test case 3: Equal strings with different lengths
    str2 = xString_fromCString("Hello, World");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) > 0);

    // Test case 4: Equal strings with different lengths
    str2 = xString_fromCString("Hello, World!!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) < 0);

    // Test case 5: Empty strings
    str1 = xString_fromCString("");
    str2 = xString_fromCString("");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) == 0);

    // Test case 6: Empty string and non-empty string
    str2 = xString_fromCString("Hello, World!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) < 0);

    // Test case 7: Non-empty string and empty string
    str1 = xString_fromCString("Hello, World!");
    str2 = xString_fromCString("");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) > 0);

    // Test case 8: Upper case and lower case strings
    str1 = xString_fromCString("hello, world!");
    str2 = xString_fromCString("HELLO, WORLD!");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compare(str1, str2) > 0);
}

void test_xString_compareIgnoreCase(void)
{
    XDEFER_SCOPE
    xString *str1 = NULL;
    xString *str2 = NULL;

    // Test case 1: Equal strings
    str1 = xString_fromCString("Hello, World!");
    str2 = xString_fromCString("Hello, World!");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) == 0);

    // Test case 2: Different strings
    str2 = xString_fromCString("Hello, Universe!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) > 0);

    // Test case 3: Equal strings with different lengths
    str2 = xString_fromCString("Hello, World");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) > 0);

    // Test case 4: Equal strings with different lengths
    str2 = xString_fromCString("Hello, World!!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) < 0);

    // Test case 5: Empty strings
    str1 = xString_fromCString("");
    str2 = xString_fromCString("");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) == 0);

    // Test case 6: Empty string and non-empty string
    str2 = xString_fromCString("Hello, World!");
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) < 0);

    // Test case 7: Non-empty string and empty string
    str1 = xString_fromCString("Hello, World!");
    str2 = xString_fromCString("");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) > 0);

    // Test case 8: Upper case and lower case strings
    str1 = xString_fromCString("hello, world!");
    str2 = xString_fromCString("HELLO, WORLD!");
    DEFER(xString_free, str1);
    DEFER(xString_free, str2);
    CU_ASSERT_TRUE(xString_compareIgnoreCase(str1, str2) == 0);
}

void test_xString_toCString(void)
{
    XDEFER_SCOPE
    xString *str = NULL;
    char *cstr = NULL;

    // Test case 1: Convert xString to C string
    str = xString_fromCString("Hello, World!");
    cstr = xString_toCString(str);
    DEFER(xString_free, str);
    DEFER(free, cstr);
    CU_ASSERT_STRING_EQUAL(cstr, "Hello, World!");

    // Test case 2: Convert empty xString to C string
    str = xString_new();
    cstr = xString_toCString(str);
    DEFER(xString_free, str);
    DEFER(free, cstr);
    CU_ASSERT_EQUAL(cstr, NULL);

    // Test case 3: Convert NULL xString to C string
    cstr = xString_toCString(NULL);
    CU_ASSERT_EQUAL(cstr, NULL);

    // Test case 4: Convert xString with embedded NULL characters to C string
    str = xString_fromCString("Hello, \0World!");
    cstr = xString_toCString(str);
    DEFER(xString_free, str);
    DEFER(free, cstr);
    CU_ASSERT_STRING_EQUAL(cstr, "Hello, ");
}

void test_xString_toInt(void)
{
    XDEFER_SCOPE
    xString *str = xString_fromCString("12345");
    DEFER(xString_free, str);

    // Test case 1: Positive integer
    int value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 2: Negative integer
    str = xString_fromCString("-12345");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, -12345);

    // Test case 3: Zero
    str = xString_fromCString("0");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 4: Invalid integer
    str = xString_fromCString("12345abc");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 5: Empty string
    str = xString_fromCString("");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 6: Non-numeric string
    str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 7: Integer with leading zeros
    str = xString_fromCString("00012345");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 8: Integer with leading spaces
    str = xString_fromCString("   12345");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 9: Integer with trailing spaces
    str = xString_fromCString("12345   ");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 10: Out of range value
    str = xString_fromCString("2147483648");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, -2147483648);

    // Test case 11: Out of range negative value
    str = xString_fromCString("-2147483649");
    DEFER(xString_free, str);
    value = xString_toInt(str);
    CU_ASSERT_EQUAL(value, 2147483647);
}

void test_xString_toLong(void)
{
    XDEFER_SCOPE
    xString *str = xString_fromCString("12345");
    DEFER(xString_free, str);

    // Test case 1: Positive integer
    long long value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 2: Negative integer
    str = xString_fromCString("-12345");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, -12345);

    // Test case 3: Zero
    str = xString_fromCString("0");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 4: Invalid integer
    str = xString_fromCString("12345abc");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 5: Empty string
    str = xString_fromCString("");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 6: Non-numeric string
    str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 0);

    // Test case 7: Integer with leading zeros
    str = xString_fromCString("00012345");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 8: Integer with leading spaces
    str = xString_fromCString("   12345");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 9: Integer with trailing spaces
    str = xString_fromCString("12345   ");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, 12345);

    // Test case 10: Out of range value
    str = xString_fromCString("9223372036854775808");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, (xInt64)XINT64_MIN);

    // Test case 11: Out of range negative value
    str = xString_fromCString("-9223372036854775809");
    DEFER(xString_free, str);
    value = xString_toLong(str);
    CU_ASSERT_EQUAL(value, (xInt64)XINT64_MAX);
}

void test_xString_toFloat(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive float
    str = xString_fromCString("123.45");
    DEFER(xString_free, str);
    float value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0001);

    // Test case 2: Negative float
    str = xString_fromCString("-123.45");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, -123.45, 0.0001);

    // Test case 3: Zero
    str = xString_fromCString("0.0");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 0.0, 0.0001);

    // Test case 4: Invalid float
    str = xString_fromCString("123.45abc");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isnanf(value));

    // Test case 5: Empty string
    str = xString_fromCString("");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isnanf(value));

    // Test case 6: Non-numeric string
    str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isnanf(value));

    // Test case 7: Positive infinity
    str = xString_fromCString("Inf");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isinff(value) == 1);

    // Test case 8: Negative infinity
    str = xString_fromCString("-Inf");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isinff(value) == -1);

    // Test case 9: NaN
    str = xString_fromCString("NaN");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isnanf(value));

    // Test case 10: Float with leading zeros
    str = xString_fromCString("000123.45");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0001);

    // Test case 11: Float with leading spaces
    str = xString_fromCString("   123.45");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0001);

    // Test case 12: Float with trailing spaces
    str = xString_fromCString("123.45   ");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0001);

    // Test case 13: Out of range value
    str = xString_fromCString("360000000000000000000000000000000000000");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isinff(value) == 1);

    // Test case 14: Out of range negative value
    str = xString_fromCString("-360000000000000000000000000000000000000");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_TRUE(isinff(value) == -1);

    // Test case 15: Very small value (rounded to zero)
    str = xString_fromCString("0.000000000000000000000000000000000001");
    DEFER(xString_free, str);
    value = xString_toFloat(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 0.0, 0.0001);
}

void test_xString_toDouble(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive double
    str = xString_fromCString("123.45");
    DEFER(xString_free, str);
    double value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0000001);

    // Test case 2: Negative double
    str = xString_fromCString("-123.45");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, -123.45, 0.0000001);

    // Test case 3: Zero
    str = xString_fromCString("0.0");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 0.0, 0.0000001);

    // Test case 4: No decimal point
    str = xString_fromCString("12345");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 12345.0, 0.0000001);

    // Test case 5: No integer part
    str = xString_fromCString(".45");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 0.45, 0.0000001);

    // Test case 6: Invalid double
    str = xString_fromCString("123.45abc");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isnan(value));

    // Test case 7: Empty string
    str = xString_fromCString("");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isnan(value));

    // Test case 8: Non-numeric string
    str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isnan(value));

    // Test case 9: Positive infinity
    str = xString_fromCString("Inf");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isinf(value) == 1);

    // Test case 10: Negative infinity
    str = xString_fromCString("-Inf");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isinf(value) == -1);

    // Test case 11: NaN
    str = xString_fromCString("NaN");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isnan(value));

    // Test case 12: Double with leading zeros
    str = xString_fromCString("000123.45");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0000001);

    // Test case 13: Double with leading spaces
    str = xString_fromCString("   123.45");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0000001);

    // Test case 14: Double with trailing spaces
    str = xString_fromCString("123.45   ");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_DOUBLE_EQUAL(value, 123.45, 0.0000001);

    // Test case 15: Out of range value
    str = xString_fromCString(
        "19000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000.0");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isinf(value) == 1);

    // Test case 16: Out of range negative value
    str = xString_fromCString(
        "-1900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000.0");
    DEFER(xString_free, str);
    value = xString_toDouble(str);
    CU_ASSERT_TRUE(isinf(value) == -1)
}

void test_xString_fromCString(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Normal string
    str = xString_fromCString("Hello, World!");
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 2: Empty string
    str = xString_fromCString("");
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);
    CU_ASSERT_EQUAL(xString_getData(str), NULL);

    // Test case 3: NULL address
    str = xString_fromCString(NULL);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);
    CU_ASSERT_EQUAL(xString_getData(str), NULL);

    // Test case 4: String with embedded NULL characters
    str = xString_fromCString("Hello, \0World!");
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 7);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, ", 7));
}

void test_xString_fromCStringS(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Normal string
    str = xString_fromCStringS("Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 13);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, World!", 13));

    // Test case 2: Empty string
    str = xString_fromCStringS("", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);

    // Test case 3: NULL address
    str = xString_fromCStringS(NULL, 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 0);

    // Test case 4: String with embedded NULL characters
    str = xString_fromCStringS("Hello, \0World!", 14);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 14);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello, \0World!", 14));

    // Test case 5: Smaller copy length than actual string
    str = xString_fromCStringS("Hello, World!", 5);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"Hello", 5));
}

void test_xString_fromInt(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive integer
    str = xString_fromInt(12345);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 5);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"12345", 5));

    // Test case 2: Negative integer
    str = xString_fromInt(-12345);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 6);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"-12345", 6));

    // Test case 3: Zero
    str = xString_fromInt(0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 1);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"0", 1));
}

void test_xString_fromLong(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive long
    str = xString_fromLong(234567890123456789);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 18);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"234567890123456789", 18));

    // Test case 2: Negative long
    str = xString_fromLong(-234567890123456789);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 19);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"-234567890123456789", 19));

    // Test case 3: Zero
    str = xString_fromLong(0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_getLength(str), 1);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"0", 1));
}

void test_xString_fromFloat(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive float
    str = xString_fromFloat(123.45);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 6);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"123.45", 6));

    // Test case 2: Negative float
    str = xString_fromFloat(-123.45);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 7);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"-123.45", 7));

    // Test case 3: Zero
    str = xString_fromFloat(0.0);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 1);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"0", 1));
}

void test_xString_fromDouble(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Positive double
    str = xString_fromDouble(123.45);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 6);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"123.45", 6));

    // Test case 2: Negative double
    str = xString_fromDouble(-123.45);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 7);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"-123.45", 7));

    // Test case 3: Zero
    str = xString_fromDouble(0.0);
    DEFER(xString_free, str);
    CU_ASSERT_TRUE(xString_getLength(str) >= 1);
    CU_ASSERT_TRUE(xMemCmp(xString_getData(str), (const void *)"0", 1));
}

void test_xString_hash(void)
{
    XDEFER_SCOPE
    xString *str = NULL;

    // Test case 1: Normal string
    str = xString_fromCStringS("Hello, World!", 13);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0x6EF05BD7CC857C54);

    // Test case 2: Empty string
    str = xString_fromCStringS("", 0);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0xCBF29CE484222325);

    // Test case 3: NULL address
    CU_ASSERT_EQUAL(xString_hash(NULL), 0);

    // Test case 4: String with embedded NULL characters
    str = xString_fromCStringS("Hello, \0World!", 14);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0x78866f6ECA47C59E);

    // Test case 5: Smaller copy length than actual string
    str = xString_fromCStringS("Hello, World!", 5);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0x63F0BFACF2C00F6B);

    // Test case 6: Null character string
    str = xString_fromCStringS("\0", 1);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0xAF63BD4C8601B7DF);

    // Test case 7: Long string
    str = xString_fromCStringS(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur ut ipsum vitae lorem mollis condimentum sit amet "
        "dignissim neque. Mauris id lorem in mauris consectetur ullamcorper. Quisque egestas eros et consectetur sagittis. Aenean "
        "accumsan, justo et facilisis interdum, risus velit facilisis massa, eget vestibulum ipsum justo vitae arcu. Duis "
        "consequat pharetra porttitor. Aliquam dapibus dui nec auctor fermentum. Curabitur felis lorem, volutpat quis ipsum eu, "
        "finibus faucibus nunc. Morbi felis ante, convallis et augue at, accumsan imperdiet justo. Lorem ipsum dolor sit amet, "
        "consectetur adipiscing elit. Pellentesque vitae gravida lectus. Maecenas finibus dictum erat eget ullamcorper. Maecenas "
        "eget euismod ante. Praesent maximus laoreet pharetra. Quisque sodales sit amet mauris vel viverra. Proin justo mauris, "
        "convallis sed eros ut, vestibulum mattis risus. Nunc quis ipsum pulvinar, semper velit a, luctus ligula. Donec tincidunt "
        "felis id est convallis, ut malesuada lectus ullamcorper. Suspendisse tellus.",
        1023);
    DEFER(xString_free, str);
    CU_ASSERT_EQUAL(xString_hash(str), 0x7EBF6D96502617ad);

    // Test case 8: Known collision case (for 64-bit FNV-1a hashing function)
    str = xString_fromCStringS("8yn0iYCKYHlIj4-BwPqk", 20);
    xString *str2 = xString_fromCStringS("GReLUrM4wMqfg9yzV3KQ", 20);
    DEFER(xString_free, str);
    DEFER(xString_free, str2);
    CU_ASSERT_EQUAL(xString_hash(str), xString_hash(str2));
}

int main(void)
{
    CU_pSuite pSuite = NULL;

    // Initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Add a suite to the registry
    pSuite = CU_add_suite("xString", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite
    if (CU_add_test(pSuite, "cstrlen", test_cstrlen) == NULL || CU_add_test(pSuite, "xString_new", test_xString_new) == NULL ||
        CU_add_test(pSuite, "xString_optimize", test_xString_optimize) == NULL ||
        CU_add_test(pSuite, "xString_preallocate", test_xString_preallocate) == NULL ||
        CU_add_test(pSuite, "xString_append", test_xString_append) == NULL ||
        CU_add_test(pSuite, "xString_fromCString", test_xString_fromCString) == NULL ||
        CU_add_test(pSuite, "xString_fromCStringS", test_xString_fromCStringS) == NULL ||
        CU_add_test(pSuite, "xString_substring", test_xString_substring) == NULL ||
        CU_add_test(pSuite, "xString_find", test_xString_find) == NULL ||
        CU_add_test(pSuite, "xString_findLast", test_xString_findLast) == NULL ||
        CU_add_test(pSuite, "xString_count", test_xString_count) == NULL ||
        CU_add_test(pSuite, "xString_count_overlapping", test_xString_count_overlapping) == NULL ||
        CU_add_test(pSuite, "xString_replaceFirst", test_xString_replaceFirst) == NULL ||
        CU_add_test(pSuite, "xString_replaceLast", test_xString_replaceLast) == NULL ||
        CU_add_test(pSuite, "xString_replaceAll", test_xString_replaceAll) == NULL ||
        CU_add_test(pSuite, "xString_remove", test_xString_remove) == NULL ||
        CU_add_test(pSuite, "xString_insert", test_xString_insert) == NULL ||
        CU_add_test(pSuite, "xString_compare", test_xString_compare) == NULL ||
        CU_add_test(pSuite, "xString_compareIgnoreCase", test_xString_compareIgnoreCase) == NULL ||
        CU_add_test(pSuite, "xString_toCString", test_xString_toCString) == NULL ||
        CU_add_test(pSuite, "xString_toInt", test_xString_toInt) == NULL ||
        CU_add_test(pSuite, "xString_toLong", test_xString_toLong) == NULL ||
        CU_add_test(pSuite, "xString_toFloat", test_xString_toFloat) == NULL ||
        CU_add_test(pSuite, "xString_toDouble", test_xString_toDouble) == NULL ||
        CU_add_test(pSuite, "xString_fromInt", test_xString_fromInt) == NULL ||
        CU_add_test(pSuite, "xString_fromLong", test_xString_fromLong) == NULL ||
        CU_add_test(pSuite, "xString_fromFloat", test_xString_fromFloat) == NULL ||
        CU_add_test(pSuite, "xString_fromDouble", test_xString_fromDouble) == NULL ||
        CU_add_test(pSuite, "xString_hash", test_xString_hash) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set up the test framework and run the tests
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
