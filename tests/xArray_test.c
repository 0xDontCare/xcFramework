/**
 * @file xArray_test.c
 * @author 0xDontCare (https://www.github.com/0xDontCare)
 * @brief CUnit test for xArray module.
 * @version 0.1
 * @date 05.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"
#include "xMemory/xDefer.h"
#include "xStructures/xArray.h"

void test_xArray_new(void)
{
    XDEFER_SCOPE
    xArray *arr = NULL;

    // Test case 1: Valid array element size
    arr = xArray_new(sizeof(xUInt32));
    DEFER(xArray_free, arr);
    CU_ASSERT_NOT_EQUAL(arr, NULL);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);

    // Test case 2: Invalid array element size
    arr = xArray_new(0);  // should return NULL
    // DEFER(xArray_free, arr);
    CU_ASSERT_EQUAL(arr, NULL);
}

void test_xArray_free(void)
{
    XDEFER_SCOPE
    xArray *arr = xArray_new(sizeof(xUInt32));
    DEFER(xArray_free, arr);

    // Test case 1: Free valid array
    xArray_free(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(arr), 0);
    CU_ASSERT_EQUAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL(xArray_isValid(arr), false);

    // Test case 2: Free already freed array
    xArray_free(arr);  // should not crash on double free

    // Test case 3: Free NULL array
    xArray_free(NULL);
}

void test_xArray_push(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Push element to empty array
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);

    // Test case 2: Push element to non-empty array
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(arr) && *((xUInt32 *)xArray_getData(arr) + 1) == 0x9ABCDEF0);

    // Test case 3: Push element to NULL array
    xArray_push(NULL, &value);  // should not crash

    // Test case 4: Push NULL element to array
    xArray_push(arr, NULL);  // should not crash

    // Test case 5: Push element to array with insufficient capacity
    arr = xArray_new(sizeof(xUInt32));
    xArray_resize(arr, 0);
    value = 0x12345678;
    xArray_push(arr, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);

    // Test case 6: Push element to array with insufficient capacity
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(arr) && *((xUInt32 *)xArray_getData(arr) + 1) == 0x9ABCDEF0);

    // Test case 7: Push element to array with insufficient capacity
    value = 0x12345678;
    xArray_push(arr, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 3);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 3);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(arr) && *((xUInt32 *)xArray_getData(arr) + 1) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_getData(arr) && *((xUInt32 *)xArray_getData(arr) + 2) == 0x12345678);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_pop(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Pop element from empty array
    CU_ASSERT_EQUAL(xArray_pop(arr), NULL);

    // Test case 2: Pop element from array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    void *ret = xArray_pop(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);
    CU_ASSERT_EQUAL(*(xUInt32 *)ret, value);

    // Test case 3: Pop element from array with multiple elements
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    value = 0x12345678;
    xArray_push(arr, &value);
    ret = xArray_pop(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)ret == 0x12345678);

    // Test case 4: Try to read old value if new value is pushed
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);  // will overwrite value on previous pointer
    CU_ASSERT_EQUAL(*(xUInt32 *)ret, 0x9ABCDEF0);

    // Test case 5: Pop element from NULL array
    CU_ASSERT_EQUAL(xArray_pop(NULL), NULL);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_getters(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Get size of empty array
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);

    // Test case 2: Get capacity of empty array
    CU_ASSERT_EQUAL(xArray_getCapacity(arr), 0);

    // Test case 3: Get element size of array
    CU_ASSERT_EQUAL(xArray_getElemSize(arr), sizeof(xUInt32));

    // Test case 4: Get data of empty array
    CU_ASSERT_EQUAL(xArray_getData(arr), NULL);

    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);

    // Test case 5: Get size of non-empty array
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);

    // Test case 6: Get capacity of non-empty array
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);

    // Test case 7: Get element size of array
    CU_ASSERT_EQUAL(xArray_getElemSize(arr), sizeof(xUInt32));

    // Test case 8: Get data of non-empty array
    CU_ASSERT_EQUAL(*(xUInt32 *)xArray_getData(arr), value);

    // Test case 9: Get data of NULL array
    CU_ASSERT_EQUAL(xArray_getData(NULL), NULL);

    // Test case 10: Get size of NULL array
    CU_ASSERT_EQUAL(xArray_getSize(NULL), 0);

    // Test case 11: Get capacity of NULL array
    CU_ASSERT_EQUAL(xArray_getCapacity(NULL), 0);

    // Test case 12: Get element size of NULL array
    CU_ASSERT_EQUAL(xArray_getElemSize(NULL), 0);

    // Test case 13: Get data of NULL array
    CU_ASSERT_EQUAL(xArray_getData(NULL), NULL);
}

void test_xArray_resize(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Resize empty array to 0
    xArray_resize(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);

    // Test case 2: Resize empty array to 1
    xArray_resize(arr, 1);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);

    // Test case 3: Resize array to 0
    xArray_resize(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);

    // Test case 4: Resize array to 1
    xArray_resize(arr, 1);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);

    // Test case 5: Resize array to 2
    xArray_resize(arr, 2);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);

    // Test case 6: Resize array to 1
    xArray_resize(arr, 1);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);

    // Test case 7: Resize array to 0
    xArray_resize(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);

    // Test case 8: Resize array to 0
    xArray_resize(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);

    // Test case 9: Resize array to 1
    xArray_resize(arr, 1);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);

    // Test case 10: Resize array to 0
    xArray_resize(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_get(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Get element from empty array
    CU_ASSERT_EQUAL(xArray_get(arr, 0), NULL);

    // Test case 2: Get element from array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x12345678);

    // Test case 3: Get element from array with multiple elements
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(arr, 1) && *(xUInt32 *)xArray_get(arr, 1) == 0x9ABCDEF0);

    // Test case 4: Get element from NULL array
    CU_ASSERT_FALSE(xArray_get(NULL, 0));
    // CU_ASSERT_EQUAL(xArray_get(NULL, 0), NULL);

    // Test case 5: Get element from out of bounds index
    CU_ASSERT_FALSE(xArray_get(arr, 5));
    // CU_ASSERT_EQUAL(xArray_get(arr, 5), NULL);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_insert(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Insert element to empty array
    xUInt32 value = 0x12345678;
    xArray_insert(arr, 0, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == value);

    // Test case 2: Insert element to beginning of array
    value = 0x9ABCDEF0;
    xArray_insert(arr, 0, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(arr, 1) && *(xUInt32 *)xArray_get(arr, 1) == 0x12345678);

    // Test case 3: Insert element to middle of array
    value = 0x12345678;
    xArray_insert(arr, 1, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 3);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 3);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 1) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 2) == 0x12345678);

    // Test case 4: Insert element to end of array
    value = 0x9ABCDEF0;
    xArray_insert(arr, 3, &value);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 4);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 4);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(arr, 1) && *(xUInt32 *)xArray_get(arr, 1) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(arr, 2) && *(xUInt32 *)xArray_get(arr, 2) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(arr, 3) && *(xUInt32 *)xArray_get(arr, 3) == 0x9ABCDEF0);

    // Test case 5: Insert element to NULL array
    xArray_insert(NULL, 0, &value);  // should not crash

    // Test case 6: Insert NULL element to array
    xArray_insert(arr, 0, NULL);  // should not crash
}

void test_xArray_remove(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Remove element from empty array
    xArray_remove(arr, 0);  // should not crash

    // Test case 2: Remove element from array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    xArray_remove(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);
    CU_ASSERT_TRUE(xArray_getData(arr) != NULL);

    // Test case 3: Remove element from array with multiple elements
    value = 0x12345678;
    xArray_push(arr, &value);
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    xArray_remove(arr, 0);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x9ABCDEF0);

    // Test case 4: Remove element from array with multiple elements
    value = 0x12345678;
    xArray_push(arr, &value);
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    xArray_remove(arr, 1);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(arr, 1) && *(xUInt32 *)xArray_get(arr, 1) == 0x9ABCDEF0);

    // Test case 5: Remove element from out of bounds index
    xArray_push(arr, &value);
    xArray_remove(arr, 1);  // should not crash

    // Test case 6: Remove element from NULL array
    xArray_remove(NULL, 0);  // should not crash

    // Cleanup
    xArray_free(arr);
}

void test_xArray_clear(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Clear empty array
    xArray_clear(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);
    CU_ASSERT_TRUE(xArray_getData(arr) == NULL);  // array data block was not allocated before

    // Test case 2: Clear array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    xArray_clear(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);
    CU_ASSERT_TRUE(xArray_getData(arr) != NULL);  // array data block stays allocated

    // Test case 3: Clear array with multiple elements
    value = 0x12345678;
    xArray_push(arr, &value);
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    xArray_clear(arr);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    // CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 0);
    CU_ASSERT_TRUE(xArray_getData(arr) != NULL);

    // Test case 4: Clear NULL array
    xArray_clear(NULL);  // should not crash

    // Cleanup
    xArray_free(arr);
}

void test_xArray_isValid(void)
{
    xArray *arr = xArray_new(sizeof(xUInt32));

    // Test case 1: Check valid array
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);

    // Test case 2: Check invalid array
    // changing element size to 0
    *((xSize *)((char *)arr + sizeof(void *))) =
        0;  // this assumes that the element size is stored after the data pointer in the structure
    CU_ASSERT_EQUAL(xArray_isValid(arr), false);

    // Test case 3: Check NULL array
    CU_ASSERT_EQUAL(xArray_isValid(NULL), false);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_copy(void)
{
    // dummy array setup
    xArray *arr = xArray_new(sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL_FATAL(xArray_isValid(arr), true);

    // Test case 1: Copy empty array
    xArray *copy = xArray_copy(arr);
    CU_ASSERT_TRUE(xArray_getSize(copy) == xArray_getSize(arr));
    CU_ASSERT_TRUE(xArray_getCapacity(copy) == xArray_getCapacity(arr));
    CU_ASSERT_TRUE(xArray_getElemSize(copy) == xArray_getElemSize(arr));
    CU_ASSERT_TRUE(xArray_getData(copy) == NULL);
    CU_ASSERT_TRUE(xArray_isValid(copy) == true)
    xArray_free(copy);

    // Test case 2: Copy array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    copy = xArray_copy(arr);
    CU_ASSERT_TRUE(xArray_getSize(copy) == xArray_getSize(arr));
    CU_ASSERT_TRUE(xArray_getCapacity(copy) >= xArray_getSize(arr));
    CU_ASSERT_TRUE(xArray_getData(copy) && *(xUInt32 *)xArray_getData(copy) == 0x12345678);
    xArray_free(copy);

    // Test case 3: Copy array with multiple elements
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    copy = xArray_copy(arr);
    CU_ASSERT_TRUE(xArray_getSize(copy) == xArray_getSize(arr));
    CU_ASSERT_TRUE(xArray_getCapacity(copy) >= xArray_getSize(arr));
    CU_ASSERT_TRUE(xArray_getData(copy) && *(xUInt32 *)xArray_getData(copy) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(copy) && *((xUInt32 *)xArray_getData(copy) + 1) == 0x9ABCDEF0);
    xArray_free(copy);

    // Test case 4: Copy NULL array
    copy = xArray_copy(NULL);
    CU_ASSERT_EQUAL(xArray_getSize(copy), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(copy), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(copy), 0);
    CU_ASSERT_EQUAL(xArray_getData(copy), NULL);
    CU_ASSERT_EQUAL(xArray_isValid(copy), false);
    xArray_free(copy);

    // Cleanup
    xArray_free(arr);
}

void test_xArray_append(void)
{
    // dummy arrays setup
    xArray *arr = xArray_new(sizeof(xUInt32));
    xArray *other = xArray_new(sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL_FATAL(xArray_isValid(arr), true);
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(other), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(other), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(other), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(other), NULL);
    CU_ASSERT_EQUAL_FATAL(xArray_isValid(other), true);

    // Test case 1: Append empty array to empty array
    xArray_append(arr, other);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);

    // Test case 2: Append empty array to array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    xArray_append(arr, other);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 1);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 1);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == value);
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);

    // Test case 3: Append empty array to array with multiple elements
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    xArray_append(arr, other);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_TRUE(xArray_getData(arr) && *(xUInt32 *)xArray_getData(arr) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(arr) && *((xUInt32 *)xArray_getData(arr) + 1) == 0x9ABCDEF0);
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);

    // Test case 4: Append array with elements to empty array
    xArray_append(other, arr);
    CU_ASSERT_EQUAL(xArray_getSize(other), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(other) >= 2);
    CU_ASSERT_TRUE(xArray_getData(other) && *(xUInt32 *)xArray_getData(other) == 0x12345678);
    CU_ASSERT_TRUE(xArray_getData(other) && *((xUInt32 *)xArray_getData(other) + 1) == 0x9ABCDEF0);

    // Test case 5: Append array with multiple elements to another with multiple elements
    value = 0xDEADBEEF;
    xArray_push(other, &value);
    value = 0xCAFEBABE;
    xArray_push(other, &value);
    xArray_append(other, arr);
    CU_ASSERT_EQUAL(xArray_getSize(other), 6);
    CU_ASSERT_TRUE(xArray_getCapacity(other) >= 6);
    CU_ASSERT_TRUE(xArray_get(other, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(other, 1) && *(xUInt32 *)xArray_get(other, 1) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(other, 2) && *(xUInt32 *)xArray_get(other, 2) == 0xDEADBEEF);
    CU_ASSERT_TRUE(xArray_get(other, 3) && *(xUInt32 *)xArray_get(other, 3) == 0xCAFEBABE);
    CU_ASSERT_TRUE(xArray_get(other, 4) && *(xUInt32 *)xArray_get(other, 4) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(other, 5) && *(xUInt32 *)xArray_get(other, 5) == 0x9ABCDEF0);

    // Test case 6: Append NULL array to array
    xArray_append(arr, NULL);
    CU_ASSERT_EQUAL(xArray_getSize(arr), 2);
    CU_ASSERT_TRUE(xArray_getCapacity(arr) >= 2);
    CU_ASSERT_EQUAL(xArray_isValid(arr), true);
    CU_ASSERT_TRUE(xArray_get(arr, 0) && *(xUInt32 *)xArray_get(arr, 0) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(arr, 1) && *(xUInt32 *)xArray_get(arr, 1) == 0x9ABCDEF0);

    // Test case 7: Append array to NULL array
    xArray_append(NULL, arr);  // should not crash

    // Test case 8: Append NULL array to NULL array
    xArray_append(NULL, NULL);  // should not crash

    // Cleanup
    xArray_free(arr);
    xArray_free(other);
}

// foreach mock callback
xUInt32 foreach_mock_count = 0;
void *foreach_processed[100];
void foreach_mock(const void *elem) { foreach_processed[foreach_mock_count++] = (void *)elem; }

void test_xArray_foreach(void)
{
    // dummy array setup
    xArray *arr = xArray_new(sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(arr), NULL);
    CU_ASSERT_EQUAL_FATAL(xArray_isValid(arr), true);
    xMemSet(foreach_processed, 0, sizeof(foreach_processed));

    // Test case 1: Foreach empty array
    xArray_foreach(arr, foreach_mock);
    CU_ASSERT_EQUAL(foreach_mock_count, 0);

    // Test case 2: Foreach array with single element
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    xArray_foreach(arr, foreach_mock);
    CU_ASSERT_EQUAL(foreach_mock_count, 1);
    CU_ASSERT_EQUAL(*(xUInt32 *)foreach_processed[0], value);
    foreach_mock_count = 0;

    // Test case 3: Foreach array with multiple elements
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    xArray_foreach(arr, foreach_mock);
    CU_ASSERT_EQUAL(foreach_mock_count, 2);
    CU_ASSERT_EQUAL(*(xUInt32 *)foreach_processed[0], 0x12345678);
    CU_ASSERT_EQUAL(*(xUInt32 *)foreach_processed[1], 0x9ABCDEF0);
    foreach_mock_count = 0;

    // Test case 4: Foreach NULL array
    xArray_foreach(NULL, foreach_mock);  // should not crash
    CU_ASSERT_EQUAL(foreach_mock_count, 0);

    // Test case 5: Foreach array with NULL callback
    xArray_foreach(arr, NULL);  // should not crash
    CU_ASSERT_EQUAL(foreach_mock_count, 0);

    // Cleanup
    xArray_free(arr);
}

// mock predicates for filter tests
xBool predicate_mock_even(const void *elem) { return (*(xUInt32 *)elem % 2) == 0; }
xBool predicate_mock_all(const void *elem)
{
    (void)elem;
    return true;
}
xBool predicate_mock_none(const void *elem)
{
    (void)elem;
    return false;
}

void test_xArray_filter(void)
{
    // dummy array setup
    xArray *arr = xArray_new(sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(arr), NULL);

    // Test case 1: Filter empty array with even predicate
    xArray *filtered = xArray_filter(arr, predicate_mock_even);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(filtered), NULL);
    xArray_free(filtered);

    // Test case 2: Filter empty array with all predicate
    filtered = xArray_filter(arr, predicate_mock_all);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(filtered), NULL);
    xArray_free(filtered);

    // Test case 3: Filter empty array with none predicate
    filtered = xArray_filter(arr, predicate_mock_none);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(filtered), NULL);
    xArray_free(filtered);

    // adding elements to array
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    value = 0xDEADBEEF;
    xArray_push(arr, &value);
    value = 0xCAFEBABE;
    xArray_push(arr, &value);

    // Test case 4: Filter array with even predicate
    filtered = xArray_filter(arr, predicate_mock_even);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 3);
    CU_ASSERT_TRUE(xArray_getCapacity(filtered) >= 3);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_TRUE(xArray_get(filtered, 0) && *(const xUInt32 *)xArray_get(filtered, 0) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(filtered, 1) && *(const xUInt32 *)xArray_get(filtered, 1) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(filtered, 2) && *(const xUInt32 *)xArray_get(filtered, 2) == 0xCAFEBABE);
    xArray_free(filtered);

    // Test case 5: Filter array with all predicate
    filtered = xArray_filter(arr, predicate_mock_all);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 4);
    CU_ASSERT_TRUE(xArray_getCapacity(filtered) >= 4);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_TRUE(xArray_get(filtered, 0) && *(const xUInt32 *)xArray_get(filtered, 0) == 0x12345678);
    CU_ASSERT_TRUE(xArray_get(filtered, 1) && *(const xUInt32 *)xArray_get(filtered, 1) == 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(filtered, 2) && *(const xUInt32 *)xArray_get(filtered, 2) == 0xDEADBEEF);
    CU_ASSERT_TRUE(xArray_get(filtered, 3) && *(const xUInt32 *)xArray_get(filtered, 3) == 0xCAFEBABE);
    xArray_free(filtered);

    // Test case 6: Filter array with none predicate
    filtered = xArray_filter(arr, predicate_mock_none);
    CU_ASSERT_EQUAL(xArray_getSize(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(filtered), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(filtered), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(filtered), NULL);
    xArray_free(filtered);

    // Test case 7: Filter NULL array
    filtered = xArray_filter(NULL, predicate_mock_even);
    CU_ASSERT_FALSE(xArray_isValid(filtered));
    xArray_free(filtered);

    // Test case 8: Filter with NULL predicate
    filtered = xArray_filter(arr, NULL);
    CU_ASSERT_FALSE(xArray_isValid(filtered));
    xArray_free(filtered);
}

// mock mapping functions for map tests
void *map_mock_double(const void *elem)
{
    xUInt32 *result = malloc(sizeof(xUInt32));
    *result = *(const xUInt32 *)elem * 2;
    return result;
}
void *map_mock_square(const void *elem)
{
    xUInt32 *result = malloc(sizeof(xUInt32));
    *result = *(const xUInt32 *)elem * *(const xUInt32 *)elem;
    return result;
}
void *map_mock_null(const void *elem)
{
    (void)elem;
    return NULL;
}

void test_xArray_map(void)
{
    // dummy array setup
    xArray *arr = xArray_new(sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getSize(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getCapacity(arr), 0);
    CU_ASSERT_EQUAL_FATAL(xArray_getElemSize(arr), sizeof(xUInt32));
    CU_ASSERT_EQUAL_FATAL(xArray_getData(arr), NULL);

    // Test case 1: Map empty array with double function
    xArray *mapped = xArray_map(arr, map_mock_double);
    CU_ASSERT_EQUAL(xArray_getSize(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(mapped), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(mapped), NULL);
    xArray_free(mapped);

    // Test case 2: Map empty array with square function
    mapped = xArray_map(arr, map_mock_square);
    CU_ASSERT_EQUAL(xArray_getSize(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(mapped), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(mapped), NULL);
    xArray_free(mapped);

    // adding elements to array
    xUInt32 value = 0x12345678;
    xArray_push(arr, &value);
    value = 0x9ABCDEF0;
    xArray_push(arr, &value);
    value = 0xDEADBEEF;
    xArray_push(arr, &value);
    value = 0xCAFEBABE;
    xArray_push(arr, &value);

    // Test case 3: Map array with double function
    mapped = xArray_map(arr, map_mock_double);
    CU_ASSERT_EQUAL(xArray_getSize(mapped), 4);
    CU_ASSERT_TRUE(xArray_getCapacity(mapped) >= 4);
    CU_ASSERT_EQUAL(xArray_getElemSize(mapped), sizeof(xUInt32));
    CU_ASSERT_TRUE(xArray_get(mapped, 0) && *(const xUInt32 *)xArray_get(mapped, 0) == 0x12345678 * 2);
    CU_ASSERT_TRUE(xArray_get(mapped, 1) && *(const xUInt32 *)xArray_get(mapped, 1) == 0x9ABCDEF0 * 2);
    CU_ASSERT_TRUE(xArray_get(mapped, 2) && *(const xUInt32 *)xArray_get(mapped, 2) == 0xDEADBEEF * 2);
    CU_ASSERT_TRUE(xArray_get(mapped, 3) && *(const xUInt32 *)xArray_get(mapped, 3) == 0xCAFEBABE * 2);
    xArray_free(mapped);

    // Test case 4: Map array with square function
    mapped = xArray_map(arr, map_mock_square);
    CU_ASSERT_EQUAL(xArray_getSize(mapped), 4);
    CU_ASSERT_TRUE(xArray_getCapacity(mapped) >= 4);
    CU_ASSERT_EQUAL(xArray_getElemSize(mapped), sizeof(xUInt32));
    CU_ASSERT_TRUE(xArray_get(mapped, 0) && *(const xUInt32 *)xArray_get(mapped, 0) == 0x12345678U * 0x12345678U);
    CU_ASSERT_TRUE(xArray_get(mapped, 1) && *(const xUInt32 *)xArray_get(mapped, 1) == 0x9ABCDEF0 * 0x9ABCDEF0);
    CU_ASSERT_TRUE(xArray_get(mapped, 2) && *(const xUInt32 *)xArray_get(mapped, 2) == 0xDEADBEEF * 0xDEADBEEF);
    CU_ASSERT_TRUE(xArray_get(mapped, 3) && *(const xUInt32 *)xArray_get(mapped, 3) == 0xCAFEBABE * 0xCAFEBABE);
    xArray_free(mapped);

    // Test case 5: Map array with NULL function
    mapped = xArray_map(arr, map_mock_null);
    CU_ASSERT_EQUAL(xArray_getSize(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getCapacity(mapped), 0);
    CU_ASSERT_EQUAL(xArray_getElemSize(mapped), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xArray_getData(mapped), NULL);
    xArray_free(mapped);

    // Test case 6: Map NULL array
    mapped = xArray_map(NULL, map_mock_double);
    CU_ASSERT_FALSE(xArray_isValid(mapped));
    xArray_free(mapped);

    // Test case 7: Map with NULL function
    mapped = xArray_map(arr, NULL);
    CU_ASSERT_FALSE(xArray_isValid(mapped));
    xArray_free(mapped);
}

int main(void)
{
    CU_pSuite pSuite = NULL;

    // initialize CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // add a suite to the registry
    pSuite = CU_add_suite("xArray_test", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // add the tests to the suite
    if (CU_add_test(pSuite, "xArray_new", test_xArray_new) == NULL ||
        CU_add_test(pSuite, "xArray_push", test_xArray_push) == NULL ||
        CU_add_test(pSuite, "xArray_pop", test_xArray_pop) == NULL ||
        CU_add_test(pSuite, "xArray_getters", test_xArray_getters) == NULL ||
        CU_add_test(pSuite, "xArray_resize", test_xArray_resize) == NULL ||
        CU_add_test(pSuite, "xArray_get", test_xArray_get) == NULL ||
        CU_add_test(pSuite, "xArray_insert", test_xArray_insert) == NULL ||
        CU_add_test(pSuite, "xArray_remove", test_xArray_remove) == NULL ||
        CU_add_test(pSuite, "xArray_clear", test_xArray_clear) == NULL ||
        CU_add_test(pSuite, "xArray_isValid", test_xArray_isValid) == NULL ||
        CU_add_test(pSuite, "xArray_copy", test_xArray_copy) == NULL ||
        CU_add_test(pSuite, "xArray_append", test_xArray_append) == NULL ||
        CU_add_test(pSuite, "xArray_foreach", test_xArray_foreach) == NULL ||
        CU_add_test(pSuite, "xArray_filter", test_xArray_filter) == NULL ||
        CU_add_test(pSuite, "xArray_map", test_xArray_map) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
