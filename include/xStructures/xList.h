/**
 * @file xList.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Linked list implemenation in xStructures module.
 * @version 0.10
 * @date 26.07.2024.
 *
 * Module declares linked list structure and functions for interaction with it. All functions have prefix `xList_`.
 */

#ifndef XSTRUCTURES_LIST_H
#define XSTRUCTURES_LIST_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Linked list structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xList object.
 *
 * @warning
 * Even if structure itself is on stack, it should be properly freed using xList_free() function to free internal data.
 */
typedef struct xList_s xList;

/**
 * @brief
 * Creates empty xList object.
 *
 * @param elemSize Size of single element in bytes.
 * @return Pointer to xList object with no data.
 *
 * @note
 * If function fails to allocate memory or `elemSize` is zero, returned object will be invalid to use. You can use xList_isValid()
 * to check if created object is valid.
 */
xList *xList_new(xSize elemSize);

/**
 * @brief
 * Free xList object and its data from memory.
 *
 * @param list Pointer to xList object to free.
 *
 * @note
 * Once xList object is freed, it is invalidated and no longer usable with other functions.
 *
 * @note
 * All elements in the list are freed as well. If they contain other dynamically allocated data, it should be freed by user before.
 */
void xList_free(xList *list);

/**
 * @brief
 * Get number of elements in list.
 *
 * @param list Pointer to xList object.
 * @return xSize Number of elements in list.
 */
extern xSize xList_getSize(const xList *list);

/**
 * @brief
 * Get size of single element in list in bytes.
 *
 * @param list Pointer to xList object.
 * @return xSize Size of single element in list in bytes.
 */
extern xSize xList_getElemSize(const xList *list);

/**
 * @brief
 * Check if xList object is valid.
 *
 * @param list Pointer to xList object.
 * @return xBool Non-zero if object is valid, zero otherwise.
 */
extern xBool xList_isValid(const xList *list);

/**
 * @brief
 * Insert element at specified index in list.
 *
 * @param list Pointer to the list object.
 * @param data Pointer to data to insert.
 * @param index Index to insert data at.
 *
 * @note
 * If index is greater than list size, function will do nothing and return.
 */
void xList_insert(xList *list, const void *data, xSize index);

/**
 * @brief
 * Get element at specified index in list.
 *
 * @param list Pointer to the list object.
 * @param index Index of element to get.
 * @return void* Pointer to element at specified index.
 *
 * @note
 * If index is greater than list size, function will return NULL.
 *
 * @warning
 * Element should not be freed as returned address is not its base address. Trying to do so will result in undefined behavior.
 */
void *xList_get(const xList *list, xSize index);

/**
 * @brief
 * Remove element at specified index in list.
 *
 * @param list Pointer to the list object.
 * @param index Index of element to remove.
 * @return void* Pointer to removed element.
 *
 * @note
 * If index is greater than list size, function will return NULL and do nothing.
 *
 * @warning
 * User is responsible for freeing returned element.
 *
 */
void *xList_remove(xList *list, xSize index);

/**
 * @brief
 * Push element to the front of the list.
 *
 * @param list Pointer to the list object.
 * @param data Pointer to data to push to front of the list.
 */
void xList_pushFront(xList *list, const void *data);

/**
 * @brief
 * Push element to the back of the list.
 *
 * @param list Pointer to the list object.
 * @param data Pointer to data to push to back of the list.
 */
void xList_pushBack(xList *list, const void *data);

/**
 * @brief
 * Pop element from the front of the list.
 *
 * @param list Pointer to the list object.
 * @return void* Pointer to popped element.
 *
 * @note
 * If list is empty, function will return NULL.
 *
 * @warning
 * Caller is responsible for freeing popped element.
 */
void *xList_popFront(xList *list);

/**
 * @brief
 * Pop element from the back of the list.
 *
 * @param list Pointer to the list object.
 * @return void* Pointer to popped element.
 *
 * @note
 * If list is empty, function will return NULL.
 *
 * @warning
 * Caller is responsible for freeing popped element.
 */
void *xList_popBack(xList *list);

/**
 * @brief
 * Peek element from the front of the list.
 *
 * @param list Pointer to the list object.
 * @return void* Pointer to peeked element.
 *
 * @note
 * If list is empty, function will return NULL.
 *
 * @warning
 * Returned address is not base address of the element so it should not be freed. Trying to do so will result in undefined behavior.
 */
void *xList_peekFront(const xList *list);

/**
 * @brief
 * Peek element from the back of the list.
 *
 * @param list Pointer to the list object.
 * @return void* Pointer to peeked element.
 *
 * @note
 * If list is empty, function will return NULL.
 *
 * @warning
 * Returned address is not base address of the element so it should not be freed. Trying to do so will result in undefined behavior.
 */
void *xList_peekBack(const xList *list);

/**
 * @brief
 * Clear list by removing all elements.
 *
 * @param list Pointer to the list object.
 *
 * @note
 * If elements contain dynamically allocated data, it should be freed by user before calling this function.
 */
void xList_clear(xList *list);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_LIST_H
