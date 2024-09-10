#include "xStructures/xStack.h"
#include <stdlib.h>  // malloc, free, realloc
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xStack_s {
    void **data;
    xSize elemSize;
    xSize stackSize;
    xSize stackCapacity;
};

xStack xStack_new(xSize elemSize)
{
    if (elemSize == 0) {
        return (xStack){0};
    }

    xStack stack = {0};
    stack.elemSize = elemSize;
    return stack;
}

void xStack_free(xStack *stack)
{
    if (!stack || !stack->data) {
        return;
    }

    free(stack->data);
    stack->data = NULL;
    stack->stackSize = 0;
    stack->stackCapacity = 0;
    stack->elemSize = 0;
}

inline xSize xStack_getSize(const xStack *stack) { return (stack) ? stack->stackSize : 0; }

inline xSize xStack_getCapacity(const xStack *stack) { return (stack) ? stack->stackCapacity : 0; }

inline xSize xStack_getElemSize(const xStack *stack) { return (stack) ? stack->elemSize : 0; }

inline xBool xStack_isValid(const xStack *stack) { return (stack && stack->data && stack->elemSize) ? true : false; }

void xStack_push(xStack *stack, const void *data)
{
    if (!xStack_isValid(stack) || !data) {
        return;
    }

    if (stack->stackCapacity < stack->stackSize + 1) {
        void **newData =
            (void **)realloc(stack->data, ((stack->stackCapacity == 0) ? 1 : stack->stackCapacity * 2) * sizeof(void *));
        if (!newData) {
            return;
        }
        stack->data = newData;
        stack->stackCapacity = (stack->stackCapacity == 0) ? 1 : stack->stackCapacity * 2;
    }

    stack->data[stack->stackSize] = (void *)data;
    stack->stackSize++;
}

void *xStack_pop(xStack *stack)
{
    if (!xStack_isValid(stack) || stack->stackSize == 0) {
        return NULL;
    }

    stack->stackSize--;
    void *ret = stack->data[stack->stackSize];
    stack->data[stack->stackSize] = NULL;

    return ret;
}

const void *xStack_peek(const xStack *stack)
{
    if (!xStack_isValid(stack) || stack->stackSize == 0) {
        return NULL;
    }

    return (const void *)(stack->data[stack->stackSize - 1]);
}

void xStack_clear(xStack *stack)
{
    for (xSize i = 0; i < stack->stackSize; i++) {
        stack->data[i] = NULL;
    }

    stack->stackSize = 0;
}

xStack xStack_copy(const xStack *stack)
{
    if (!xStack_isValid(stack)) {
        return xStack_new(0);
    }

    xStack newStack = xStack_new(stack->elemSize);
    if (xStack_isValid(&newStack)) {
        for (xSize i = 0; i < stack->stackSize; i++) {
            xStack_push(&newStack, stack->data[i]);
        }
    }

    return newStack;
}
