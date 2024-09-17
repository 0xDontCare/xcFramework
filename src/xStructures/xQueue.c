#include "xStructures/xQueue.h"
#include <stdlib.h>  // malloc, realloc, free
#include "xBase/xTypes.h"
#include "xStructures/xList.h"  // under the hood, queue is implemented using linked list

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xQueue_s {
    xList *internalList;
};

xQueue *xQueue_new(xSize elemSize)
{
    // validate passed argument
    if (elemSize == 0) {
        return NULL;
    }

    // allocate memory for the queue
    xQueue *queue = (xQueue *)malloc(sizeof(xQueue));
    if (!queue) {
        return NULL;
    }

    // allocate memory for the internal list structure
    queue->internalList = xList_new(elemSize);
    if (!xQueue_isValid(queue)) {
        free(queue);
        return NULL;
    }

    return queue;
}

void xQueue_free(xQueue *queue)
{
    // validate passed argument
    if (!queue) {
        return;
    }

    // free the internal list and queue itself
    if (xQueue_isValid(queue)) {
        xList_free(queue->internalList);
    }

    queue->internalList = NULL;
    free(queue);
}

inline xSize xQueue_getSize(const xQueue *queue) { return (queue) ? xList_getSize(queue->internalList) : 0; }

inline xSize xQueue_getElemSize(const xQueue *queue) { return (queue) ? xList_getElemSize(queue->internalList) : 0; }

inline xBool xQueue_isValid(const xQueue *queue) { return (queue && xList_isValid(queue->internalList)) ? true : false; }

void xQueue_enqueue(xQueue *queue, const void *data)
{
    // validate passed arguments
    if (!xQueue_isValid(queue) || !data) {
        return;
    }

    // push the data to the back of the internal list
    xList_pushBack(queue->internalList, data);
}

void *xQueue_dequeue(xQueue *queue)
{
    // validate passed argument
    if (!xQueue_isValid(queue)) {
        return NULL;
    }

    // pop the front element from the internal list and return it
    return xList_popFront(queue->internalList);
}

const void *xQueue_peek(const xQueue *queue)
{
    // validate passed argument
    if (!xQueue_isValid(queue)) {
        return NULL;
    }

    // return the front element of the internal list without removing it
    return (const void *)xList_peekFront(queue->internalList);
}

void xQueue_clear(xQueue *queue)
{
    // validate passed argument
    if (!xQueue_isValid(queue)) {
        return;
    }

    // clear the internal list
    xList_clear(queue->internalList);
}

xQueue *xQueue_copy(const xQueue *queue)
{
    // validate passed argument
    if (!xQueue_isValid(queue)) {
        return NULL;
    }

    // create a new queue and copy the internal list
    xQueue *newQueue = (xQueue *)malloc(sizeof(xQueue));
    if (!newQueue) {
        return NULL;
    }
    newQueue->internalList = xList_copy(queue->internalList);

    return newQueue;
}
