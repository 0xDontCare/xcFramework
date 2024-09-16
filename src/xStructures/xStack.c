#include "xStructures/xStack.h"
#include <stdlib.h>  // malloc, free, realloc
#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

// TODO: implement ability to change underlying data structure (e.g. array, linked list)
struct xStack_s {
    void *data;
    xSize elemSize;
    xSize stackSize;
    xSize stackCapacity;
};

xStack *xStack_new(xSize elemSize)
{
    // validate arguments
    if (elemSize == 0) {
        return NULL;
    }

    // allocate memory for stack
    xStack *stack = (xStack *)malloc(sizeof(xStack));
    if (!stack) {
        return NULL;
    }

    // initialize attributes and return stack
    stack->elemSize = elemSize;
    stack->stackSize = 0;
    stack->stackCapacity = 0;
    stack->data = NULL;
    return stack;
}

void xStack_free(xStack *stack)
{
    // validate arguments
    if (!stack) {
        return;
    }

    // free data of stack if allocated
    if (stack->data) {
        free(stack->data);
    }

    // reset attributes and free stack
    stack->data = NULL;
    stack->stackSize = 0;
    stack->stackCapacity = 0;
    stack->elemSize = 0;
    free(stack);
}

inline xSize xStack_getSize(const xStack *stack) { return (stack) ? stack->stackSize : 0; }

inline xSize xStack_getCapacity(const xStack *stack) { return (stack) ? stack->stackCapacity : 0; }

inline xSize xStack_getElemSize(const xStack *stack) { return (stack) ? stack->elemSize : 0; }

inline xBool xStack_isValid(const xStack *stack) { return (stack && stack->elemSize) ? true : false; }

void xStack_push(xStack *stack, const void *data)
{
    // validate arguments
    if (!xStack_isValid(stack) || !data) {
        return;
    }

    // reallocate memory if stack is full
    if (stack->stackCapacity < stack->stackSize + 1) {
        void *newData =
            (void *)realloc(stack->data, ((stack->stackCapacity == 0) ? 1 : stack->stackCapacity * 2) * stack->elemSize);
        if (!newData) {
            return;
        }
        stack->data = newData;
        stack->stackCapacity = (stack->stackCapacity == 0) ? 1 : stack->stackCapacity * 2;
    }

    // copy data to stack and increment stack size
    xMemCopy((void *)((char *)stack->data + stack->stackSize * stack->elemSize), data, stack->elemSize);
    stack->stackSize++;
}

void *xStack_pop(xStack *stack)
{
    // validate arguments
    if (!xStack_isValid(stack) || stack->stackSize == 0) {
        return NULL;
    }

    // allocate memory for returned element
    void *ret = malloc(stack->elemSize);
    if (!ret) {
        return NULL;
    }

    // copy data from stack to return element, clear data in stack and decrement stack size
    stack->stackSize--;
    xMemCopy(ret, (void *)((char *)stack->data + stack->stackSize * stack->elemSize), stack->elemSize);
    xMemSet((void *)((char *)stack->data + stack->stackSize * stack->elemSize), 0, stack->elemSize);  // could be removed??

    return ret;
}

const void *xStack_peek(const xStack *stack)
{
    // validate argument
    if (!xStack_isValid(stack) || stack->stackSize == 0) {
        return NULL;
    }

    return (const void *)((char *)stack->data + (stack->stackSize - 1) * stack->elemSize);
}

void xStack_clear(xStack *stack)
{
    // validate argument
    if (!xStack_isValid(stack) || stack->stackSize == 0) {
        return;
    }

    // clear data in stack and reset stack size
    xMemSet(stack->data, 0, stack->stackSize * stack->elemSize);
    stack->stackSize = 0;
}

xStack *xStack_copy(const xStack *stack)
{
    // validate argument
    if (!xStack_isValid(stack)) {
        return xStack_new(0);
    }

    // allocate new stack, copy data from source stack and return
    xStack *newStack = xStack_new(stack->elemSize);
    if (!xStack_isValid(newStack)) {
        return NULL;
    }

    newStack->data = (void *)malloc(stack->stackSize * stack->elemSize);
    if (!newStack->data) {
        xStack_free(newStack);
        return NULL;
    }
    xMemCopy(newStack->data, stack->data, stack->stackSize * stack->elemSize);
    newStack->stackSize = stack->stackSize;
    newStack->stackCapacity = stack->stackSize;

    return newStack;
}
