#include "xStructures/xQueue.h"
#include <stdlib.h>  // malloc, realloc, free
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xQueue_s {
    void **data;
    xSize elemSize;
    xSize queueSize;
    xSize queueCapacity;
};

xQueue xQueue_new(xSize elemSize)
{
    if (elemSize == 0) {
        return (xQueue){0};
    }

    xQueue queue = {0};
    queue.elemSize = elemSize;
    return queue;
}

void xQueue_free(xQueue *queue)
{
    if (!queue || !queue->data) {
        return;
    }

    free(queue->data);
    queue->data = NULL;
    queue->queueSize = 0;
    queue->queueCapacity = 0;
    queue->elemSize = 0;
}

inline xSize xQueue_getSize(const xQueue *queue) { return (queue) ? queue->queueSize : 0; }

inline xSize xQueue_getCapacity(const xQueue *queue) { return (queue) ? queue->queueCapacity : 0; }

inline xSize xQueue_getElemSize(const xQueue *queue) { return (queue) ? queue->elemSize : 0; }

inline xBool xQueue_isValid(const xQueue *queue) { return (queue && queue->data && queue->elemSize) ? true : false; }

void xQueue_enqueue(xQueue *queue, const void *data)
{
    if (!xQueue_isValid(queue) || !data) {
        return;
    }

    if (queue->queueCapacity < queue->queueSize + 1) {
        void **newData =
            (void **)realloc(queue->data, ((queue->queueCapacity == 0) ? 1 : queue->queueCapacity * 2) * sizeof(void *));
        if (!newData) {
            return;
        }

        queue->data = newData;
        queue->queueCapacity = (queue->queueCapacity == 0) ? 1 : queue->queueCapacity * 2;
    }

    queue->data[queue->queueSize] = (void *)data;
    queue->queueSize++;
}

void *xQueue_dequeue(xQueue *queue)
{
    if (!xQueue_isValid(queue) || queue->queueSize == 0) {
        return NULL;
    }

    void *ret = queue->data[0];
    for (xSize i = 1; i < queue->queueSize; i++) {
        queue->data[i - 1] = queue->data[i];
    }

    queue->queueSize--;
    return ret;
}

const void *xQueue_peek(const xQueue *queue)
{
    if (!xQueue_isValid(queue) || queue->queueSize == 0) {
        return NULL;
    }

    return (const void *)queue->data[0];
}

void xQueue_clear(xQueue *queue)
{
    for (xSize i = 0; i < queue->queueSize; i++) {
        queue->data[i] = NULL;
    }

    queue->queueSize = 0;
}

xQueue xQueue_copy(const xQueue *queue)
{
    if (!xQueue_isValid(queue)) {
        return xQueue_new(0);
    }

    xQueue newQueue = xQueue_new(queue->elemSize);
    if (xQueue_isValid(&newQueue)) {
        for (xSize i = 0; i < queue->queueSize; i++) {
            xQueue_enqueue(&newQueue, queue->data[i]);
        }
    }

    return newQueue;
}
