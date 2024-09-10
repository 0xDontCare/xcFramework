/**
 * @file xDefer.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Defer mechanism implementation.
 * @version 0.10
 * @date 01.08.2024.
 *
 */

// Only GCC and compatible compilers are supported.
#if !defined(__GNUC__) && !defined(__clang__) && !defined(__MINGW32__) && !defined(__MINGW64__)
#error "Only compilers with __attribute__((cleanup)) support are supported."
#endif

#ifndef XMEMORY_DEFER_H
#define XMEMORY_DEFER_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct DeferFunc_s {
    void (*func)(void *);
    void *arg;
} DeferFunc;

typedef struct DeferStack_s {
    DeferFunc *funcs;
    xSize size;
    xSize capacity;
} DeferStack;

#define DEFER_STACK_INIT_CAPACITY 16

/**
 * @brief
 * Initialize a defer stack.
 *
 * @param stack The defer stack to be initialized.
 *
 * @warning
 * This function should not be called directly. Use XDEFER_SCOPE instead to declare a defer scope.
 */
void xDefer_stackInit(DeferStack *stack);

/**
 * @brief
 * Push a function call with argument to the defer stack.
 *
 * @param stack Pointer to the defer stack.
 * @param func The function to be called.
 * @param arg The argument of the function.
 *
 * @warning
 * This function should not be called directly. Use DEFER instead to defer a function call.
 */
void xDefer_stackPush(DeferStack *stack, void (*func)(void *), void *arg);

/**
 * @brief
 * Pop all deferred functions in the stack and call them.
 *
 * @param stack Pointer to the defer stack.
 *
 * @warning
 * This function should not be called directly as it is used as an automatic cleanup function.
 */
void xDefer_stackPopAll(DeferStack *stack);

/**
 * @brief
 * Declare a defer scope. It must be used before deferring any function in the scope.
 */
#define XDEFER_SCOPE                                                               \
    DeferStack _xc_DeferStack_ __attribute__((cleanup(xDefer_stackPopAll))) = {0}; \
    xDefer_stackInit(&_xc_DeferStack_);

/**
 * @brief
 * Defer a function call with argument.
 *
 * @param func The function to be called.
 * @param arg The argument of the function.
 *
 * @note
 * Deferred functions will be called in reverse order from the order they are deferred in.
 *
 * @note
 * All deferred functions need to have the same signature: void (*)(void *).
 */
#define DEFER(func, arg) xDefer_stackPush(&_xc_DeferStack_, (void (*)(void *))(func), (void *)(arg))

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // XMEMORY_DEFER_H
