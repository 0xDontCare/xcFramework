/**
 * @file xMemtools_test.c
 * @author 0xDontCare (https://www.github.com/0xDontCare)
 * @brief CUnit test for all functions within xMemtools module.
 * @version 0.1
 * @date 04.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"
#include "xMemory/xDefer.h"

void test_xMemCmp(void)
{
    // Test case 1: Comparing memory block with multiple characters
    CU_ASSERT_TRUE(xMemCmp("Hello, World!", "Hello, World!", 13));

    // Test case 2: Comparing empty memory block
    CU_ASSERT_TRUE(xMemCmp("", "", 0));

    // Test case 3: Comparing memory block with single character
    CU_ASSERT_TRUE(xMemCmp("A", "A", 1));

    // Test case 4: Comparing memory block with special characters
    CU_ASSERT_TRUE(xMemCmp("1234567890", "1234567890", 10));

    // Test case 5: Comparing memory block with non-printable characters
    CU_ASSERT_TRUE(xMemCmp("\x01\x52\x10", "\x01\x52\x10", 3));

    // Test case 6: Comparing memory block with NULL address
    CU_ASSERT_TRUE(xMemCmp(NULL, NULL, 0));

    // Test case 7: Comparing memory block with different characters
    CU_ASSERT_FALSE(xMemCmp("Hello, World!", "Hello, Universe!", 13));

    // Test case 8: Comparing memory block with different lengths
    CU_ASSERT_FALSE(xMemCmp("Hello, World!", "Hello, World", 13));

    // Test case 9: Block1 is NULL
    CU_ASSERT_FALSE(xMemCmp(NULL, "Hello, World!", 13));

    // Test case 10: Block2 is NULL
    CU_ASSERT_FALSE(xMemCmp("Hello, World!", NULL, 13));
}

void test_xMemCopy(void)
{
    // Test case 1: Copying memory block with multiple characters
    char src1[] = "Hello, World!";
    char dest1[14];
    xMemCopy(dest1, src1, 13);
    CU_ASSERT_TRUE(xMemCmp(dest1, src1, 13));

    // Test case 2: Copying empty memory block
    char src2[] = "";
    char dest2[1];
    xMemCopy(dest2, src2, 0);
    CU_ASSERT_TRUE(xMemCmp(dest2, src2, 0));

    // Test case 3: Copying memory block with single character
    char src3[] = "A";
    char dest3[2];
    xMemCopy(dest3, src3, 1);
    CU_ASSERT_TRUE(xMemCmp(dest3, src3, 1));

    // Test case 4: Copying memory block with special characters
    char src4[] = "1234567890";
    char dest4[11];
    xMemCopy(dest4, src4, 10);
    CU_ASSERT_TRUE(xMemCmp(dest4, src4, 10));

    // Test case 5: Copying block of memory containing larger elements
    xUInt32 src5[] = {0x12345678, 0x9ABCDEF0};
    xUInt32 dest5[2];
    xMemCopy(dest5, src5, 2 * sizeof(xUInt32));
    CU_ASSERT_TRUE(xMemCmp(dest5, src5, 2 * sizeof(xUInt32)));

    // Test case 6: Copying memory block with non-printable characters
    char src6[] = "\x01\x52\x10";
    char dest6[3];
    xMemCopy(dest6, src6, 3);
    CU_ASSERT_TRUE(xMemCmp(dest6, src6, 3));

    // Test case 7: Copying memory block with NULL source
    char src7[] = "Hello, World!";
    char dest7[14];
    xMemCopy(dest7, NULL, 13);
    CU_ASSERT_TRUE(xMemCmp(dest7, src7, 0));

    // Test case 8: Copying memory block with NULL destination
    char src8[] = "Hello, World!";
    char dest8[14];
    xMemCopy(NULL, src8, 13);
    CU_ASSERT_TRUE(xMemCmp(dest8, src8, 0));

    // Test case 9: Copying partial memory block
    char src9[] = "Hello, World!";
    char dest9[14];
    xMemCopy(dest9, src9, 5);
    CU_ASSERT_TRUE(xMemCmp(dest9, src9, 5));

    // Test case 10: Copying memory block with overlapping source and destination
    char *src10 = (char *)malloc(13);
    CU_ASSERT_TRUE_FATAL(src10 != NULL);
    char *dest = src10 + 5;

    xMemCopy(src10, "Hello, World!", 13);
    CU_ASSERT_TRUE(xMemCmp(src10, "Hello, World!", 13));

    xMemCopy(dest, src10, 8);
    CU_ASSERT_TRUE(xMemCmp(dest, "HelloHel", 8));
    CU_ASSERT_TRUE(xMemCmp(src10, "HelloHelloHel", 13));
    free(src10);
}

void test_xMemMove(void)
{
    // Test case 1: Moving memory block with multiple characters
    char src1[] = "Hello, World!";
    char dest1[14];
    xMemMove(dest1, src1, 13);
    CU_ASSERT_TRUE(xMemCmp(dest1, src1, 13));

    // Test case 2: Moving empty memory block
    char src2[] = "";
    char dest2[1];
    xMemMove(dest2, src2, 0);
    CU_ASSERT_TRUE(xMemCmp(dest2, src2, 0));

    // Test case 3: Moving memory block with single character
    char src3[] = "A";
    char dest3[2];
    xMemMove(dest3, src3, 1);
    CU_ASSERT_TRUE(xMemCmp(dest3, src3, 1));

    // Test case 4: Moving memory block with special characters
    char src4[] = "1234567890";
    char dest4[11];
    xMemMove(dest4, src4, 10);
    CU_ASSERT_TRUE(xMemCmp(dest4, src4, 10));

    // Test case 5: Moving block of memory containing larger elements
    xUInt32 src5[] = {0x12345678, 0x9ABCDEF0};
    xUInt32 dest5[2];
    xMemMove(dest5, src5, 2 * sizeof(xUInt32));
    CU_ASSERT_TRUE(xMemCmp(dest5, src5, 2 * sizeof(xUInt32)));

    // Test case 6: Moving memory block with non-printable characters
    char src6[] = "\x01\x52\x10";
    char dest6[3];
    xMemMove(dest6, src6, 3);
    CU_ASSERT_TRUE(xMemCmp(dest6, src6, 3));

    // Test case 7: Moving memory block with NULL source
    char src7[] = "Hello, World!";
    char dest7[14];
    xMemMove(dest7, NULL, 13);
    CU_ASSERT_TRUE(xMemCmp(dest7, src7, 0));

    // Test case 8: Moving memory block with NULL destination
    char src8[] = "Hello, World!";
    char dest8[14];
    xMemMove(NULL, src8, 13);
    CU_ASSERT_TRUE(xMemCmp(dest8, src8, 0));

    // Test case 9: Moving partial memory block
    char src9[] = "Hello, World!";
    char dest9[14];
    xMemMove(dest9, src9, 5);
    CU_ASSERT_TRUE(xMemCmp(dest9, src9, 5));

    // Test case 10: Moving memory block with overlapping source and destination
    char *src10 = (char *)malloc(13);
    CU_ASSERT_TRUE_FATAL(src10 != NULL);
    char *dest = src10 + 5;

    xMemMove(src10, "Hello, World!", 13);
    CU_ASSERT_TRUE(xMemCmp(src10, "Hello, World!", 13));

    xMemMove(dest, src10, 8);
    CU_ASSERT_TRUE(xMemCmp(dest, "Hello, World!", 8));
    CU_ASSERT_TRUE(xMemCmp(src10, "HelloHello, World!", 13));
    free(src10);
}

void test_xMemSet(void)
{
    // Test case 1: Setting memory block with multiple characters
    char dest1[14];
    xMemSet(dest1, 'A', 13);
    CU_ASSERT_TRUE(xMemCmp(dest1, "AAAAAAAAAAAAA", 13));

    // Test case 2: Setting empty memory block
    char dest2[1];
    xMemSet(dest2, 'A', 0);
    CU_ASSERT_TRUE(xMemCmp(dest2, "", 0));

    // Test case 3: Setting memory block with single character
    char dest3[2];
    xMemSet(dest3, 'A', 1);
    CU_ASSERT_TRUE(xMemCmp(dest3, "A", 1));

    // Test case 4: Setting memory block with special characters
    char dest4[11];
    xMemSet(dest4, 'A', 10);
    CU_ASSERT_TRUE(xMemCmp(dest4, "AAAAAAAAAA", 10));

    // Test case 5: Setting block of memory containing larger elements
    xUInt32 dest5[2];
    xMemSet(dest5, (xUInt8)0x0A, 2 * sizeof(xUInt32));
    CU_ASSERT_TRUE(dest5[0] == 0x0A0A0A0A && dest5[1] == 0x0A0A0A0A);

    // Test case 6: Setting memory block with non-printable characters
    char dest6[3];
    xMemSet(dest6, '\x01', 3);
    CU_ASSERT_TRUE(xMemCmp(dest6, "\x01\x01\x01", 3));

    // Test case 7: Setting memory block with NULL destination
    char dest7[14];
    xMemSet(NULL, 'A', 13);
    CU_ASSERT_TRUE(xMemCmp(dest7, "", 0));

    // Test case 8: Setting partial memory block
    char dest8[14];
    xMemSet(dest8, 'A', 5);
    CU_ASSERT_TRUE(xMemCmp(dest8, "AAAAA", 5));
}

void test_xMemHash(void)
{
    // TESTS ARE APPLICABLE FOR 64-BIT FNV-1A HASH FUNCTION ONLY
    // ADAPT TESTS FOR OTHER HASH FUNCTIONS IF IMPLEMENTATION CHANGES

    // Test case 1: Hashing memory block with multiple characters
    CU_ASSERT_EQUAL(xMemHash("Hello, World!", 13), 0x6EF05BD7CC857C54);

    // Test case 2: Hashing empty memory block
    CU_ASSERT_EQUAL(xMemHash("", 0), 0xCBF29CE484222325);

    // Test case 3: Hashing memory block with single character
    CU_ASSERT_EQUAL(xMemHash("A", 1), 0xAF63FC4C860222EC);

    // Test case 4: Hashing memory block with special characters
    CU_ASSERT_EQUAL(xMemHash("1234567890", 10), 0x63510217CACFB1A4);

    // Test case 5: Hashing block of memory containing larger elements
    // note: on big-endian systems, hash will be different and thus test will fail
    xUInt32 src5[] = {0x12345678, 0x9ABCDEF0};
    CU_ASSERT_EQUAL(xMemHash(src5, 2 * sizeof(xUInt32)), 0xFF670306131F3495);

    // Test case 6: Hashing memory block with non-printable characters
    CU_ASSERT_EQUAL(xMemHash("\x01\x52\x10", 3), 0xD1BA33186813D8FE);

    // Test case 7: Hashing memory block with NULL address
    CU_ASSERT_EQUAL(xMemHash(NULL, 0), 0xcbf29ce484222325);

    // Test case 8: Partial hashing of memory block
    CU_ASSERT_EQUAL(xMemHash("Hello, World!", 5), 0x63F0BFACF2C00F6B);
}

void test_xMemSwap(void)
{
    // Test case 1: Swapping memory block with multiple characters
    char src1[] = "Hello, World!";
    char dest1[14];
    xMemSwap(dest1, src1, 13);
    CU_ASSERT_TRUE(xMemCmp(dest1, "Hello, World!", 13));

    // Test case 2: Swapping empty memory block
    char src2[] = "";
    char dest2[1];
    xMemSwap(dest2, src2, 0);
    CU_ASSERT_TRUE(xMemCmp(dest2, "", 0));

    // Test case 3: Swapping memory block with single character
    char src3[] = "A";
    char dest3[2];
    xMemSwap(dest3, src3, 1);
    CU_ASSERT_TRUE(xMemCmp(dest3, "A", 1));

    // Test case 4: Swapping two blocks of different contents
    char src4[17] = "Hello, World!";
    char dest4[17] = "Hello, Universe!";
    xMemSwap(dest4, src4, 16);
    CU_ASSERT_TRUE(xMemCmp(dest4, "Hello, World!", 12));
    CU_ASSERT_TRUE(xMemCmp(src4, "Hello, Universe!", 16));

    // Test case 5: Swapping memory of arrays containing larger elements
    xUInt32 src5[] = {0x12345678, 0x9ABCDEF0};
    xUInt32 dest5[] = {0x9ABCDEF0, 0x12345678};
    xMemSwap(dest5, src5, 2 * sizeof(xUInt32));
    CU_ASSERT_TRUE(src5[0] == 0x9ABCDEF0 && src5[1] == 0x12345678);
    CU_ASSERT_TRUE(dest5[0] == 0x12345678 && dest5[1] == 0x9ABCDEF0);
}

int main(void)
{
    CU_pSuite pSuite = NULL;

    // initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // add a suite to the registry
    pSuite = CU_add_suite("xMemtools_test", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // add the tests to the suite
    if (CU_add_test(pSuite, "xMemCmp", test_xMemCmp) == NULL || CU_add_test(pSuite, "xMemCopy", test_xMemCopy) == NULL ||
        CU_add_test(pSuite, "xMemMove", test_xMemMove) == NULL || CU_add_test(pSuite, "xMemSet", test_xMemSet) == NULL ||
        CU_add_test(pSuite, "xMemHash", test_xMemHash) == NULL || CU_add_test(pSuite, "xMemSwap", test_xMemSwap) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
