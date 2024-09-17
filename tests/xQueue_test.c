/**
 * @file xQueue_test.c
 * @author 0xDontCare (you@domain.com)
 * @brief CUnit test for xQueue module.
 * @version 0.1
 * @date 17.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include <stdlib.h>
#include "xBase/xTypes.h"
#include "xStructures/xQueue.h"

void test_xQueue_new(void)
{
    xQueue *queue = NULL;

    // Test case 1: Valid queue element size
    queue = xQueue_new(sizeof(xUInt32));
    CU_ASSERT_PTR_NOT_NULL(queue);
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 0);
    CU_ASSERT_EQUAL(xQueue_getElemSize(queue), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xQueue_isValid(queue), true);
    xQueue_free(queue);

    // Test case 2: Invalid queue element size
    queue = xQueue_new(0);  // should return NULL
    CU_ASSERT_PTR_NULL(queue);
}

void test_xQueue_free(void)
{
    // this test chould be analyzed through debugger and valgrind

    xQueue *queue = xQueue_new(sizeof(xUInt32));

    // Test case 1: Valid queue
    xQueue_free(queue);

    // Test case 2: NULL queue
    xQueue_free(NULL);  // should not crash
}

void test_xQueue_getters(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Get size of empty queue
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 0);

    // Test case 2: Get element size of queue
    CU_ASSERT_EQUAL(xQueue_getElemSize(queue), sizeof(xUInt32));

    // Test case 3: Get NULL queue size
    CU_ASSERT_EQUAL(xQueue_getSize(NULL), 0);

    // Test case 4: Get NULL queue element size
    CU_ASSERT_EQUAL(xQueue_getElemSize(NULL), 0);

    // Test case 5: Get size of non-empty queue
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    CU_ASSERT_EQUAL(xQueue_getSize(queue), sizeof(values) / sizeof(values[0]));

    // Cleanup
    xQueue_free(queue);
}

void test_xQueue_enqueue(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Enqueue to empty queue
    xQueue_enqueue(queue, &values[0]);
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 1);

    // Test case 2: Enqueue to non-empty queue
    for (xSize i = 1; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    CU_ASSERT_EQUAL(xQueue_getSize(queue), sizeof(values) / sizeof(values[0]));

    // Test case 3: Enqueue to NULL queue
    xQueue_enqueue(NULL, &values[0]);  // should not crash

    // Cleanup
    xQueue_free(queue);
}

void test_xQueue_dequeue(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Dequeue from empty queue
    CU_ASSERT_PTR_NULL(xQueue_dequeue(queue));

    // Test case 2: Dequeue from non-empty queue
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xUInt32 *value = (xUInt32 *)xQueue_dequeue(queue);
        CU_ASSERT_PTR_NOT_NULL(value);
        CU_ASSERT_EQUAL(*value, values[i]);
        free(value);
    }
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 0);

    // Test case 3: Dequeue from NULL queue
    CU_ASSERT_PTR_NULL(xQueue_dequeue(NULL));

    // Cleanup
    xQueue_free(queue);
}

void test_xQueue_peek(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Peek from empty queue
    CU_ASSERT_PTR_NULL(xQueue_peek(queue));

    // Test case 2: Peek from non-empty queue
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    CU_ASSERT_EQUAL(xQueue_getSize(queue), sizeof(values) / sizeof(values[0]));
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xUInt32 *value = (xUInt32 *)xQueue_peek(queue);
        CU_ASSERT_PTR_NOT_NULL(value);
        CU_ASSERT_EQUAL(*value, values[i]);
        free(xQueue_dequeue(queue));
    }

    // Test case 3: Peek from NULL queue
    CU_ASSERT_PTR_NULL(xQueue_peek(NULL));

    // Cleanup
    xQueue_free(queue);
}

void test_xQueue_clear(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Clear empty queue
    xQueue_clear(queue);
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 0);

    // Test case 2: Clear non-empty queue
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    xQueue_clear(queue);
    CU_ASSERT_EQUAL(xQueue_getSize(queue), 0);

    // Test case 3: Clear NULL queue
    xQueue_clear(NULL);  // should not crash

    // Cleanup
    xQueue_free(queue);
}

void test_xQueue_copy(void)
{
    xQueue *queue = xQueue_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Copy empty queue
    xQueue *copy = xQueue_copy(queue);
    CU_ASSERT_PTR_NULL(copy);

    // Test case 2: Copy non-empty queue
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xQueue_enqueue(queue, &values[i]);
    }
    copy = xQueue_copy(queue);
    CU_ASSERT_PTR_NOT_NULL(copy);
    CU_ASSERT_EQUAL(xQueue_getSize(copy), xQueue_getSize(queue));
    CU_ASSERT_EQUAL(xQueue_getElemSize(copy), xQueue_getElemSize(queue));
    CU_ASSERT_EQUAL(xQueue_isValid(copy), true);
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xUInt32 *value = (xUInt32 *)xQueue_dequeue(copy);
        CU_ASSERT_PTR_NOT_NULL(value);
        CU_ASSERT_EQUAL(*value, values[i]);
        free(value);
    }
    xQueue_free(copy);

    // Test case 3: Copy NULL queue
    copy = xQueue_copy(NULL);
    CU_ASSERT_PTR_NULL(copy);

    // Cleanup
    xQueue_free(queue);
}

int main(void)
{
    CU_pSuite pSuite = NULL;

    // Initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // Add a suite to the registry
    pSuite = CU_add_suite("xQueue_test", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite
    if (CU_add_test(pSuite, "test_xQueue_new", test_xQueue_new) == NULL ||
        CU_add_test(pSuite, "test_xQueue_free", test_xQueue_free) == NULL ||
        CU_add_test(pSuite, "test_xQueue_getters", test_xQueue_getters) == NULL ||
        CU_add_test(pSuite, "test_xQueue_enqueue", test_xQueue_enqueue) == NULL ||
        CU_add_test(pSuite, "test_xQueue_dequeue", test_xQueue_dequeue) == NULL ||
        CU_add_test(pSuite, "test_xQueue_peek", test_xQueue_peek) == NULL ||
        CU_add_test(pSuite, "test_xQueue_clear", test_xQueue_clear) == NULL ||
        CU_add_test(pSuite, "test_xQueue_copy", test_xQueue_copy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
