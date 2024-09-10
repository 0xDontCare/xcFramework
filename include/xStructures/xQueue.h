/**
 * @file xQueue.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Queue implementation in xStructures module.
 * @version 0.1
 * @date 05.08.2024.
 *
 * Module declares dynamic queue structure along with funcitons for managing it. All functions have prefix `xQueue_`.
 */

#ifndef XSTRUCTURES_QUEUE_H
#define XSTRUCTURES_QUEUE_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xQueue_s xQueue;

xQueue xQueue_new(xSize elemSize);

void xQueue_free(xQueue *queue);

inline xSize xQueue_getSize(const xQueue *queue);

inline xSize xQueue_getCapacity(const xQueue *queue);

inline xSize xQueue_getElemSize(const xQueue *queue);

// inline const void *xQueue_getData(const xQueue *queue);

inline xBool xQueue_isValid(const xQueue *queue);

void xQueue_enqueue(xQueue *queue, const void *data);

void *xQueue_dequeue(xQueue *queue);

const void *xQueue_peek(const xQueue *queue);

void xQueue_clear(xQueue *queue);

xQueue xQueue_copy(const xQueue *queue);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_QUEUE_H
