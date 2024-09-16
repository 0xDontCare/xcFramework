/**
 * @file xList_test.c
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief CUnit test for xList module.
 * @version 0.1
 * @date 16.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include "xBase/xTypes.h"
#include "xStructures/xList.h"

void test_xList_new(void)
{
    xList *list = NULL;

    // Test case 1: Valid list element size
    list = xList_new(sizeof(xUInt32));
    CU_ASSERT_PTR_NOT_NULL(list);
    CU_ASSERT_EQUAL(xList_getSize(list), 0);
    CU_ASSERT_EQUAL(xList_getElemSize(list), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xList_isValid(list), true);
    xList_free(list);

    // Test case 2: Invalid list element size
    list = xList_new(0);  // should return NULL
    CU_ASSERT_PTR_NULL(list);
}

void test_xList_free(void)
{
    // this test chould be analyzed through debugger and valgrind

    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Valid list
    xList_free(list);

    // Test case 2: NULL list
    xList_free(NULL);  // should not crash
}

void test_xList_getters(void)
{
    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Get size of empty list
    CU_ASSERT_EQUAL(xList_getSize(list), 0);

    // Test case 2: Get element size of list
    CU_ASSERT_EQUAL(xList_getElemSize(list), sizeof(xUInt32));

    // Test case 3: Get NULL list size
    CU_ASSERT_EQUAL(xList_getSize(NULL), 0)

    // Test case 4: Get NULL list element size
    CU_ASSERT_EQUAL(xList_getElemSize(NULL), 0)

    // Cleanup
    xList_free(list);
}

void test_xList_pushFront(void)
{
    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Push element to empty list
    xUInt32 value = 0x12345678;
    xList_pushFront(list, &value);
    CU_ASSERT_EQUAL(xList_getSize(list), 1);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), value);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), value);

    // Test case 2: Push element to non-empty list
    value = 0x9ABCDEF0;
    xList_pushFront(list, &value);
    CU_ASSERT_EQUAL(xList_getSize(list), 2);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), value);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), 0x12345678);

    // Test case 3: Push element to NULL list
    xList_pushFront(NULL, &value);  // should not crash

    // Test case 4: Push NULL element to list
    xList_pushFront(list, NULL);              // should not crash
    CU_ASSERT_EQUAL(xList_getSize(list), 2);  // nothing should change

    // Cleanup
    xList_free(list);
}

void test_xList_pushBack(void)
{
    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Push element to empty list
    xUInt32 value = 0x12345678;
    xList_pushBack(list, &value);
    CU_ASSERT_EQUAL(xList_getSize(list), 1);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), value);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), value);

    // Test case 2: Push element to non-empty list
    value = 0x9ABCDEF0;
    xList_pushBack(list, &value);
    CU_ASSERT_EQUAL(xList_getSize(list), 2);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), 0x12345678);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), value);

    // Test case 3: Push element to NULL list
    xList_pushBack(NULL, &value);  // should not crash

    // Test case 4: Push NULL element to list
    xList_pushBack(list, NULL);               // should not crash
    CU_ASSERT_EQUAL(xList_getSize(list), 2);  // nothing should change

    // Cleanup
    xList_free(list);
}

void test_xList_insert(void)
{
    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Insert element to empty list
    xUInt32 value = 0x12345678;
    xList_insert(list, &value, 0);
    CU_ASSERT_EQUAL(xList_getSize(list), 1);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), value);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), value);

    // Test case 2: Insert element to non-empty list
    value = 0x9ABCDEF0;
    xList_insert(list, &value, 1);
    CU_ASSERT_EQUAL(xList_getSize(list), 2);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), 0x12345678);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), value);

    // Test case 3: Insert element to NULL list
    xList_insert(NULL, &value, 0);  // should not crash

    // Test case 4: Insert NULL element to list
    xList_insert(list, NULL, 0);              // should not crash
    CU_ASSERT_EQUAL(xList_getSize(list), 2);  // nothing should change

    // Test case 5: Insert element at invalid index
    xList_insert(list, &value, 3);            // should not crash
    CU_ASSERT_EQUAL(xList_getSize(list), 2);  // nothing should change

    // Cleanup
    xList_free(list);
}

void test_xList_peekFront(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Peek element from empty list
    CU_ASSERT_EQUAL(xList_peekFront(list), NULL);

    // Test case 2: Peek element from non-empty list
    xList_pushBack(list, &values[0]);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), values[0]);

    // Test case 3: Peek element from list witn many elements
    for (xSize i = 1; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekFront(list), values[0]);

    // Test case 3: Peek element from NULL list
    CU_ASSERT_PTR_NULL(xList_peekFront(NULL));

    // Cleanup
    xList_free(list);
}

void test_xList_peekBack(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Peek element from empty list
    CU_ASSERT_EQUAL(xList_peekBack(list), NULL);

    // Test case 2: Peek element from non-empty list
    xList_pushBack(list, &values[0]);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), values[0]);

    // Test case 3: Peek element from list witn many elements
    for (xSize i = 1; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_peekBack(list), values[4]);

    // Test case 3: Peek element from NULL list
    CU_ASSERT_PTR_NULL(xList_peekBack(NULL));

    // Cleanup
    xList_free(list);
}

void test_xList_get(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Get element from empty list
    CU_ASSERT_EQUAL(xList_get(list, 0), NULL);

    // Test case 2: Get element from non-empty list
    xList_pushBack(list, &values[0]);
    CU_ASSERT_EQUAL(*(xUInt32 *)xList_get(list, 0), values[0]);

    // Test case 3: Get element from list witn many elements
    for (xSize i = 1; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    for (xSize i = 0; i < 5; i++) {
        CU_ASSERT_EQUAL(*(xUInt32 *)xList_get(list, i), values[i]);
    }

    // Test case 3: Get element from NULL list
    CU_ASSERT_PTR_NULL(xList_get(NULL, 0));

    // Test case 4: Get element at invalid index
    CU_ASSERT_PTR_NULL(xList_get(list, 5));
    CU_ASSERT_PTR_NULL(xList_get(list, 15));

    // Cleanup
    xList_free(list);
}

void test_xList_popFront(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Pop element from empty list
    CU_ASSERT_PTR_NULL(xList_popFront(list));

    // Test case 2: Pop element from non-empty list
    for (xSize i = 0; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    for (xSize i = 0; i < 5; i++) {
        xUInt32 *popped = xList_popFront(list);
        CU_ASSERT_EQUAL(*popped, values[i]);
        free(popped);
    }
    CU_ASSERT_EQUAL(xList_getSize(list), 0);

    // Test case 3: Pop element from NULL list
    CU_ASSERT_PTR_NULL(xList_popFront(NULL));

    // Cleanup
    xList_free(list);
}

void test_xList_popBack(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Pop element from empty list
    CU_ASSERT_PTR_NULL(xList_popBack(list));

    // Test case 2: Pop element from non-empty list
    for (xSize i = 0; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    for (xSize i = 0; i < 5; i++) {
        xUInt32 *popped = xList_popBack(list);
        CU_ASSERT_EQUAL(*popped, values[4 - i]);
        free(popped);
    }
    CU_ASSERT_EQUAL(xList_getSize(list), 0);

    // Test case 3: Pop element from NULL list
    CU_ASSERT_PTR_NULL(xList_popBack(NULL));

    // Cleanup
    xList_free(list);
}

void test_xList_remove(void)
{
    xList *list = xList_new(sizeof(xUInt32));

    // Test case 1: Remove element from empty list
    CU_ASSERT_EQUAL(xList_remove(list, 0), NULL);

    // Test case 2: Remove element from non-empty list
    xUInt32 value = 0x12345678;
    xList_pushBack(list, &value);
    xUInt32 *removed = xList_remove(list, 0);
    CU_ASSERT_EQUAL(*removed, value);
    CU_ASSERT_EQUAL(xList_getSize(list), 0);
    free(removed);

    // Test case 3: Remove element from NULL list
    CU_ASSERT_PTR_NULL(xList_remove(NULL, 0));

    // Test case 4: Remove element at invalid index
    CU_ASSERT_PTR_NULL(xList_remove(list, 0));

    // Cleanup
    xList_free(list);
}

void test_xList_clear(void)
{
    xList *list = xList_new(sizeof(xUInt32));
    xUInt32 values[] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0, 0x369BCEF0};

    // Test case 1: Clear empty list
    xList_clear(list);
    CU_ASSERT_EQUAL(xList_getSize(list), 0);

    // Test case 2: Clear non-empty list
    for (xSize i = 0; i < 5; i++) {
        xList_pushBack(list, &values[i]);
    }
    xList_clear(list);
    CU_ASSERT_EQUAL(xList_getSize(list), 0);

    // Test case 3: Clear NULL list
    xList_clear(NULL);  // should not crash

    // Cleanup
    xList_free(list);
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
    if (CU_add_test(pSuite, "xList_new", test_xList_new) == NULL || CU_add_test(pSuite, "xList_free", test_xList_free) == NULL ||
        CU_add_test(pSuite, "xList_getters", test_xList_getters) == NULL ||
        CU_add_test(pSuite, "xList_pushFront", test_xList_pushFront) == NULL ||
        CU_add_test(pSuite, "xList_pushBack", test_xList_pushBack) == NULL ||
        CU_add_test(pSuite, "xList_insert", test_xList_insert) == NULL ||
        CU_add_test(pSuite, "xList_peekFront", test_xList_peekFront) == NULL ||
        CU_add_test(pSuite, "xList_peekBack", test_xList_peekBack) == NULL ||
        CU_add_test(pSuite, "xList_get", test_xList_get) == NULL ||
        CU_add_test(pSuite, "xList_popFront", test_xList_popFront) == NULL ||
        CU_add_test(pSuite, "xList_popBack", test_xList_popBack) == NULL ||
        CU_add_test(pSuite, "xList_remove", test_xList_remove) == NULL ||
        CU_add_test(pSuite, "xList_clear", test_xList_clear) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
