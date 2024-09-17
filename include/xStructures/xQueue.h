/**
 * @file xQueue.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Queue implementation in xStructures module.
 * @version 0.10
 * @date 17.09.2024.
 *
 * Module declares dynamic queue structure along with funcitons for managing it. All functions have prefix `xQueue_`.
 */

#ifndef XSTRUCTURES_QUEUE_H
#define XSTRUCTURES_QUEUE_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Queue structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xQueue object.
 */
typedef struct xQueue_s xQueue;

/**
 * @brief
 * Create empty xQueue object.
 *
 * @param elemSize Size of single element in bytes.
 * @return Pointer to xQueue object with no data.
 *
 * @note
 * If function fails to allocate memory or `elemSize` is zero, returned object will be invalid to use. You can use xQueue_isValid()
 * to check if created object is valid.
 */
xQueue *xQueue_new(xSize elemSize);

/**
 * @brief
 * Free xQueue object and its data from memory.
 *
 * @param queue Pointer to xQueue object to free.
 *
 * @note
 * Once xQueue object is freed, it is invalidated and no longer usable with other functions.
 *
 * @note
 * All elements in the queue are freed as well. If they contain other dynamically allocated data, it should be freed by user before.
 */
void xQueue_free(xQueue *queue);

/**
 * @brief
 * Get number of elements in queue.
 *
 * @param queue Pointer to xQueue object.
 * @return xSize Number of elements in queue.
 */
extern xSize xQueue_getSize(const xQueue *queue);

// extern xSize xQueue_getCapacity(const xQueue *queue);

/**
 * @brief
 * Get size of single element in queue in bytes.
 *
 * @param queue Pointer to xQueue object.
 * @return xSize Size of single element in queue in bytes.
 */
extern xSize xQueue_getElemSize(const xQueue *queue);

/**
 * @brief
 * Check if xQueue object is valid.
 *
 * @param queue Pointer to xQueue object.
 * @return xBool Non-zero if object is valid, zero otherwise.
 */
extern xBool xQueue_isValid(const xQueue *queue);

/**
 * @brief
 * Enqueue data to the queue.
 *
 * @param queue Pointer to the queue object.
 * @param data Pointer to data to enqueue.
 *
 * @note
 * If data is NULL or queue is invalid, function will do nothing.
 */
void xQueue_enqueue(xQueue *queue, const void *data);

/**
 * @brief
 * Dequeue data from the queue and return it.
 *
 * @param queue Pointer to the queue object.
 * @return Pointer to dequeued data.
 *
 * @note
 * If queue is invalid or empty, function will return NULL.
 *
 * @warning
 * Caller is responsible for freeing returned data.
 */
void *xQueue_dequeue(xQueue *queue);

/**
 * @brief
 * Peek data from the queue without removing it.
 *
 * @param queue Pointer to the queue object.
 * @return const void* Pointer to peeked data.
 *
 * @note
 * If queue is invalid or empty, function will return NULL.
 *
 * @warning
 * Returned data should not be freed as it is not a copy. Trying to do so will result in undefined behavior.
 */
const void *xQueue_peek(const xQueue *queue);

/**
 * @brief
 * Clear the queue.
 *
 * @param queue Pointer to the queue object.
 *
 * @note
 * If element in the queue contain dynamically allocated data, it should be freed by user before calling this function.
 */
void xQueue_clear(xQueue *queue);

/**
 * @brief
 * Create a copy of the queue.
 *
 * @param queue Pointer to the queue object.
 * @return Pointer to the new queue object.
 *
 * @note
 * If queue is invalid, function will return NULL.
 *
 * @note
 * If elements in the queue contain dynamically allocated data, it should be freed by user before freeing the copied queue.
 */
xQueue *xQueue_copy(const xQueue *queue);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_QUEUE_H
