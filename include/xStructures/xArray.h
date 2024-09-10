/**
 * @file xArray.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Dynamic array implementation in xStructures module.
 * @version 0.11
 * @date 24.07.2024.
 *
 * Module declares dynamic array structure and functions for managing it. All functions have prefix `xArray_`.
 */

#ifndef XSTRUCTURES_ARRAY_H
#define XSTRUCTURES_ARRAY_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Array structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xArray object.
 *
 * @warning
 * Even if structure itself is on stack, it should be properly freed using xArray_free() function to free internal data.
 */
typedef struct xArray_s xArray;

/**
 * @brief
 * Creates empty xArray object.
 *
 * @param elemSize Size of single element in bytes.
 * @return xArray object with no data.
 *
 * @note
 * If function fails to allocate memory, returned object will be invalid to use. You can use xArray_isValid() to check if object is
 * valid.
 */
xArray *xArray_new(xSize elemSize);

/**
 * @brief
 * Free xArray object and its data from memory.
 *
 * @param arr Pointer to xArray object to free.
 *
 * @note
 * Once xArray object is freed, it is invalidated and no longer usable with other functions.
 *
 * @warning
 * User is responsible for freeing individual elements if they are dynamically allocated.
 */
void xArray_free(xArray *arr);

/**
 * @brief
 * Get size of xArray object.
 *
 * @param arr Pointer to xArray object.
 * @return xSize Size of xArray object in number of elements.
 */
extern xSize xArray_getSize(const xArray *arr);

/**
 * @brief
 * Get allocated capacity of xArray object.
 *
 * @param arr Pointer to xArray object.
 * @return xSize Capacity of xArray object in number of elements.
 */
extern xSize xArray_getCapacity(const xArray *arr);

/**
 * @brief
 * Get size of single element in xArray object.
 *
 * @param arr Pointer to xArray object.
 * @return xSize Size of single element in bytes.
 */
extern xSize xArray_getElemSize(const xArray *arr);

/**
 * @brief
 * Get pointer to data stored in xArray object.
 *
 * @param arr Pointer to xArray object.
 * @return const void* Pointer to data stored in xArray object.
 *
 * @warning
 * Do not modify data directly. Use only provided functions for managing xArray object.
 */
extern const void *xArray_getData(const xArray *arr);

/**
 * @brief
 * Check if xArray object is valid.
 *
 * @param arr Pointer to xArray object.
 * @return xBool Non-zero if xArray object is valid, zero otherwise.
 */
extern xBool xArray_isValid(const xArray *arr);

/**
 * @brief
 * Resize xArray object to new size.
 *
 * @param arr Pointer to xArray object.
 * @param newSize New size of xArray object in number of elements.
 *
 * @note
 * If new size is smaller than current size, elements will be truncated to first `newSize` elements.
 * If new size is larger than current size, new elements will be uninitialized.
 */
void xArray_resize(xArray *arr, xSize newSize);

/**
 * @brief
 * Push element to back of xArray object.
 *
 * @param arr Pointer to xArray object.
 * @param elem Pointer to element to push to back of xArray object.
 *
 * @warning
 * It is assumed that size of new element is the same as one set while creating an array. Failing to provide such could result in
 * mangled data.
 *
 * @note
 * If adding element to the array fails, no action will be performed on the rest of the array.
 */
void xArray_push(xArray *arr, const void *elem);

/**
 * @brief
 * Remove last element of the array.
 *
 * @param arr Pointer to xArray object.
 * @return void* Pointer to removed element data.
 *
 * @note
 * If xArray object is empty, function will perform no action.
 *
 * @warning
 * Address of removed element still belongs to xArray object and should not be freed. Make sure to copy data if you want to keep or
 * modify it because array might overwrite it on next push.
 */
void *xArray_pop(xArray *arr);

/**
 * @brief
 * Insert element at specified index in xArray object.
 *
 * @param arr Pointer to xArray object.
 * @param index Index where to insert element.
 * @param elem Pointer to element to insert.
 *
 * @note
 * If index is out of bounds, function will do nothing (exception is index equal to size, which is equivalent to xArray_push()).
 */
void xArray_insert(xArray *arr, xSize index, const void *elem);

/**
 * @brief
 * Get element from xArray object at specified index.
 *
 * @param arr Pointer to xArray object.
 * @param index Index of element to get.
 * @return void* Pointer to element at specified index.
 *
 * @note
 * If index is out of bounds, function will return NULL.
 */
void *xArray_get(const xArray *arr, xSize index);

/**
 * @brief
 * Remove element from xArray object at specified index.
 *
 * @param arr Pointer to xArray object.
 * @param index Index of element to remove.
 *
 * @note
 * If index is out of bounds, function will do nothing.
 *
 * @warning
 * Caller is responsible for freeing memory of removed element.
 *
 * @note
 * If you want to remove last element, consider using xArray_pop() function.
 */
void xArray_remove(xArray *arr, xSize index);

/**
 * @brief
 * Clear xArray object and remove all elements.
 *
 * @param arr Pointer to xArray object.
 *
 * @note
 * Caller is responsible for freeing memory of removed elements if they were allocated in such way.
 */
void xArray_clear(xArray *arr);

/**
 * @brief
 * Sort xArray object using provided comparator function.
 *
 * @param arr Pointer to xArray object.
 * @param comparator Comparator function to use for sorting.
 *
 * @note
 * Comparator function should return negative value if first argument is less than second, zero if they are equal, and positive
 * value if first argument is greater than second.
 *
 * @note
 * If comparator function is NULL, function will do nothing.
 */
void xArray_sort(xArray *arr, int (*comparator)(const void *, const void *));

/**
 * @brief
 * Perform action on each element in xArray object.
 *
 * @param arr Pointer to xArray object.
 * @param callback Function to call for each element.
 *
 * @note
 * Callback function should accept single argument of type `const void *`.
 *
 * @note
 * If callback function is NULL, function will do nothing.
 */
void xArray_foreach(const xArray *arr, void (*callback)(const void *));

/**
 * @brief
 * Create copy of xArray object.
 *
 * @param arr Pointer to xArray object.
 * @return xArray Copy of xArray object.
 *
 * @note
 * If function fails to allocate memory, it will return invalid xArray object.
 *
 * @note
 * Reference to data is copied, so changes to data in one object will affect other object.
 */
xArray *xArray_copy(const xArray *arr);

/**
 * @brief
 * Append elements from other xArray object to xArray object.
 *
 * @param arr Pointer to xArray object.
 * @param other Pointer to xArray object to append.
 *
 * @note
 * Reference to data is copied, so changes to data in one object will affect other object.
 *
 * @note
 * If function fails to allocate memory, it will do nothing.
 */
void xArray_append(xArray *arr, const xArray *other);

/**
 * @brief
 * Filter elements in xArray object using predicate function.
 *
 * @param arr Pointer to xArray object.
 * @param predicate Predicate function to filter elements.
 * @return xArray* New xArray object with elements that satisfy predicate function.
 *
 * @note
 * Predicate function should return xBool::true if element should be included in new array, xBool::false otherwise.
 *
 * @note
 * If function fails to allocate memory, it will return invalid xArray object.
 */
xArray *xArray_filter(const xArray *arr, xBool (*predicate)(const void *));

/**
 * @brief
 * Map elements in xArray object using mapper function.
 *
 * @param arr Pointer to xArray object.
 * @param mapper Mapper function to map elements.
 * @return xArray* New xArray object with mapped elements.
 *
 * @note
 * Mapper function should return pointer to new element based on input element.
 *
 * @note
 * If mapper function returns NULL, element is assumed to be skipped.
 *
 * @note
 * If function fails to allocate memory, it will return invalid xArray object.
 */
xArray *xArray_map(const xArray *arr, void *(*mapper)(const void *));

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_ARRAY_H
