#include "xMemory/xDefer.h"
#include <stdlib.h>  // malloc, realloc, free

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

void xDefer_stackInit(DeferStack *stack)
{
    // validate arguments
    if (!stack) {
        return;
    }

    // allocate stack and set property values
    stack->funcs = (DeferFunc *)malloc(DEFER_STACK_INIT_CAPACITY * sizeof(DeferFunc));
    stack->size = 0;
    stack->capacity = DEFER_STACK_INIT_CAPACITY;
}

void xDefer_stackPush(DeferStack *stack, void (*func)(void *), void *arg)
{
    // validate arguments
    if (!stack || !stack->funcs || !func) {
        return;
    }

    // if stack is at full capacity, reallocate to double the size
    if (stack->size == stack->capacity) {
        DeferFunc *newFuncs = (DeferFunc *)realloc(stack->funcs, 2 * stack->capacity * sizeof(DeferFunc));
        if (newFuncs == NULL) {
            return;
        }
        stack->funcs = newFuncs;
        stack->capacity *= 2;
    }

    // add element to the stack
    stack->funcs[stack->size].func = func;
    stack->funcs[stack->size].arg = arg;
    stack->size++;
}

void xDefer_stackPopAll(DeferStack *stack)
{
    // validate arguments
    if (!stack || !stack->funcs) {
        return;
    }

    // run all funcions on deferred stack and free stack memory
    while (stack->size > 0) {
        stack->size--;
        stack->funcs[stack->size].func(stack->funcs[stack->size].arg);
    }
    free(stack->funcs);
    stack->funcs = NULL;
}
