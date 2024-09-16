/**
 * @file xStack.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Stack implementation in xStructures module.
 * @version 0.10
 * @date 16.09.2024.
 *
 * Module declares stack structure and functions for managing it. All functions have prefix `xStack_`.
 */

#ifndef XSTRUCTURES_STACK_H
#define XSTRUCTURES_STACK_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Stack structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xStack object.
 *
 */
typedef struct xStack_s xStack;

/**
 * @brief
 * Create empty xStack object for elements of given size.
 *
 * @param elemSize Size of single element in bytes.
 * @return Pointer to xStack object with no data.
 *
 * @note
 * If function fails to allocate memory or `elemSize` is zero, NULL be returned.
 */
xStack *xStack_new(xSize elemSize);

/**
 * @brief
 * Free xStack object and its data from memory.
 *
 * @param stack Pointer to xStack object to free.
 *
 * @note
 * Once xStack object is freed, it is invalidated and no longer usable with other functions.
 *
 * @warning
 * User is responsible for freeing internal heap-allocated data if elements contain pointers to them.
 */
void xStack_free(xStack *stack);

/**
 * @brief
 * Get size of xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return xSize Size of xStack object in number of elements.
 */
extern xSize xStack_getSize(const xStack *stack);

/**
 * @brief
 * Get capacity of xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return xSize Capacity of xStack object in number of elements.
 */
extern xSize xStack_getCapacity(const xStack *stack);

/**
 * @brief
 * Get size of single element in xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return xSize Size of single element in bytes.
 */
extern xSize xStack_getElemSize(const xStack *stack);

/**
 * @brief
 * Check if xStack object is valid.
 *
 * @param stack Pointer to xStack object.
 * @return xBool Non-zero if xStack object is valid, zero otherwise.
 */
extern xBool xStack_isValid(const xStack *stack);

/**
 * @brief
 * Push data to xStack object.
 *
 * @param stack Pointer to xStack object.
 * @param data Pointer to data to push.
 *
 * @note
 * If function fails to allocate memory or either `stack` or `data` is NULL, it will do nothing.
 *
 * @note
 * Upon pushing, a shallow copy of data is made, so if element has pointers to other data, any changes to that data will affect
 * elements in stack.
 */
void xStack_push(xStack *stack, const void *data);

/**
 * @brief
 * Pop data from xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return Pointer to popped data.
 *
 * @note
 * Caller is responsible for freeing memory of returned element.
 *
 * @note
 * If function fails to allocate memory or `stack` is NULL, it will return NULL and remove no data from stack.
 */
void *xStack_pop(xStack *stack);

/**
 * @brief
 * Get data from top of xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return Pointer to data at top of stack.
 *
 * @note
 * Unlike xStack_pop(), this function does not allocate memory for returned data. Instead, it returns pointer to data in stack.
 *
 * @warning
 * Do not free returned data as it is managed by xStack object. Proceding to free it will result in undefined behavior.
 */
const void *xStack_peek(const xStack *stack);

/**
 * @brief
 * Clear xStack object and remove all elements.
 *
 * @param stack Pointer to xStack object.
 *
 * @note
 * If elements contain pointers to other heap-allocated data, caller is responsible for freeing them.
 */
void xStack_clear(xStack *stack);

/**
 * @brief
 * Create copy of xStack object.
 *
 * @param stack Pointer to xStack object.
 * @return xStack* Copy of xStack object.
 *
 * @note
 * If elements contain pointers to other data, their references are copied, so changes to data in one object will affect other
 * object.
 *
 * @note
 * If function fails to allocate memory, it will return NULL.
 */
xStack *xStack_copy(const xStack *stack);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_STACK_H
