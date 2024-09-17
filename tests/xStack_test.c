/**
 * @file xStack_test.c
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief CUnit test for xStack module.
 * @version 0.1
 * @date 16.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <malloc.h>
#include "xBase/xTypes.h"
#include "xStructures/xStack.h"

void test_xStack_new(void)
{
    xStack *stack = NULL;

    // Test case 1: Valid stack element size
    stack = xStack_new(sizeof(xUInt32));
    CU_ASSERT_PTR_NOT_NULL(stack);
    CU_ASSERT_EQUAL(xStack_getSize(stack), 0);
    CU_ASSERT_EQUAL(xStack_getCapacity(stack), 0);
    CU_ASSERT_EQUAL(xStack_getElemSize(stack), sizeof(xUInt32));
    CU_ASSERT_EQUAL(xStack_isValid(stack), true);
    xStack_free(stack);

    // Test case 2: Invalid stack element size
    stack = xStack_new(0);  // should return NULL
    CU_ASSERT_PTR_NULL(stack);
}

void test_xStack_free(void)
{
    // this test should be analyzed using Valgrind and debugger to ensure proper handling

    xStack *stack = xStack_new(sizeof(xUInt32));

    // Test case 1: Free valid stack
    xStack_free(stack);

    // Test case 2: Free NULL stack
    xStack_free(NULL);  // should not crash
}

void test_xStack_getters(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));

    // Test case 1: Get size of stack
    CU_ASSERT_EQUAL(xStack_getSize(stack), 0);

    // Test case 2: Get capacity of stack
    CU_ASSERT_EQUAL(xStack_getCapacity(stack), 0);

    // Test case 3: Get element size of stack
    CU_ASSERT_EQUAL(xStack_getElemSize(stack), sizeof(xUInt32));

    // Test case 4: Check if stack is valid
    CU_ASSERT_EQUAL(xStack_isValid(stack), true);

    xStack_free(stack);

    // Test case 5: Get size of NULL stack
    CU_ASSERT_EQUAL(xStack_getSize(NULL), 0);

    // Test case 6: Get capacity of NULL stack
    CU_ASSERT_EQUAL(xStack_getCapacity(NULL), 0);

    // Test case 7: Get element size of NULL stack
    CU_ASSERT_EQUAL(xStack_getElemSize(NULL), 0);

    // Test case 8: Check if NULL stack is valid
    CU_ASSERT_EQUAL(xStack_isValid(NULL), false);
}

void test_xStack_push(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Push data to empty stack
    xStack_push(stack, &values[0]);
    CU_ASSERT_EQUAL(xStack_getSize(stack), 1);
    CU_ASSERT_TRUE(xStack_getCapacity(stack) >= 1);

    // Test case 2: Push data to stack
    for (xSize i = 1; i < sizeof(values) / sizeof(values[0]); i++) {
        xStack_push(stack, &values[i]);
        CU_ASSERT_EQUAL(xStack_getSize(stack), i + 1);
        CU_ASSERT_TRUE(xStack_getCapacity(stack) >= i + 1);
    }

    xStack_free(stack);

    // Test case 3: Push data to NULL stack
    xStack_push(NULL, &values[0]);  // should not crash

    // Test case 4: Push NULL data to stack
    stack = xStack_new(sizeof(xUInt32));
    xStack_push(stack, NULL);  // should not crash
    xStack_free(stack);
}

void test_xStack_pop(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Pop data from empty stack
    CU_ASSERT_PTR_NULL(xStack_pop(stack));

    // Test case 2: Pop data from stack
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xStack_push(stack, &values[i]);
    }
    for (xSize i = sizeof(values) / sizeof(values[0]); i > 0; i--) {
        xUInt32 *data = (xUInt32 *)xStack_pop(stack);
        CU_ASSERT_PTR_NOT_NULL(data);
        CU_ASSERT_EQUAL(*data, values[i - 1]);
        free(data);
    }

    xStack_free(stack);

    // Test case 3: Pop data from NULL stack
    CU_ASSERT_PTR_NULL(xStack_pop(NULL));
}

void test_xStack_peek(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Peek data from empty stack
    CU_ASSERT_PTR_NULL(xStack_peek(stack));

    // Test case 2: Peek data from stack
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xStack_push(stack, &values[i]);
    }
    for (xSize i = sizeof(values) / sizeof(values[0]); i > 0; i--) {
        const xUInt32 *data = (const xUInt32 *)xStack_peek(stack);
        CU_ASSERT_PTR_NOT_NULL(data);
        CU_ASSERT_EQUAL(*data, values[i - 1]);
        CU_ASSERT_PTR_EQUAL(data, xStack_peek(stack));
        xStack_pop(stack);
    }

    xStack_free(stack);

    // Test case 3: Peek data from NULL stack
    CU_ASSERT_PTR_NULL(xStack_peek(NULL));
}

void test_xStack_clear(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Clear empty stack
    xStack_clear(stack);
    CU_ASSERT_EQUAL(xStack_getSize(stack), 0);

    // Test case 2: Clear stack
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xStack_push(stack, &values[i]);
    }
    xStack_clear(stack);
    CU_ASSERT_EQUAL(xStack_getSize(stack), 0);

    xStack_free(stack);

    // Test case 3: Clear NULL stack
    xStack_clear(NULL);  // should not crash
}

void test_xStack_copy(void)
{
    xStack *stack = xStack_new(sizeof(xUInt32));
    xUInt32 values[] = {1, 2, 3, 4, 5};

    // Test case 1: Copy empty stack
    xStack *copy = xStack_copy(stack);
    CU_ASSERT_PTR_NOT_NULL(copy);
    CU_ASSERT_EQUAL(xStack_getSize(copy), 0);
    xStack_free(copy);

    // Test case 2: Copy stack
    for (xSize i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        xStack_push(stack, &values[i]);
    }
    copy = xStack_copy(stack);
    CU_ASSERT_PTR_NOT_NULL(copy);
    CU_ASSERT_EQUAL(xStack_getSize(copy), xStack_getSize(stack));
    
    xUInt32 *data1 = (xUInt32 *)xStack_pop(stack);
    xUInt32 *data2 = (xUInt32 *)xStack_pop(copy);
    while (data1 && data2) {
        CU_ASSERT_EQUAL(*data2, *data1);
        free(data1);
        free(data2);
        data1 = (xUInt32 *)xStack_pop(stack);
        data2 = (xUInt32 *)xStack_pop(copy);
    }
    CU_ASSERT_PTR_NULL(data1);
    CU_ASSERT_PTR_NULL(data2);
    xStack_free(copy);
    xStack_free(stack);

    // Test case 3: Copy NULL stack
    copy = xStack_copy(NULL);
    CU_ASSERT_PTR_NULL(copy);
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
    if (CU_add_test(pSuite, "xStack_new", test_xStack_new) == NULL ||
        CU_add_test(pSuite, "xStack_free", test_xStack_free) == NULL ||
        CU_add_test(pSuite, "xStack_getters", test_xStack_getters) == NULL ||
        CU_add_test(pSuite, "xStack_push", test_xStack_push) == NULL ||
        CU_add_test(pSuite, "xStack_pop", test_xStack_pop) == NULL ||
        CU_add_test(pSuite, "xStack_peek", test_xStack_peek) == NULL ||
        CU_add_test(pSuite, "xStack_clear", test_xStack_clear) == NULL ||
        CU_add_test(pSuite, "xStack_copy", test_xStack_copy) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
