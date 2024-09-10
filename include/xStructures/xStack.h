/**
 * @file xStack.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Stack implementation in xStructures module.
 * @version 0.1
 * @date 05.08.2024.
 *
 * Module declares stack structure and functions for managing it. All functions have prefix `xStack_`.
 */

#ifndef XSTRUCTURES_STACK_H
#define XSTRUCTURES_STACK_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xStack_s xStack;

xStack xStack_new(xSize elemSize);

void xStack_free(xStack *stack);

inline xSize xStack_getSize(const xStack *stack);

inline xSize xStack_getCapacity(const xStack *stack);

inline xSize xStack_getElemSize(const xStack *stack);

// inline const void *xStack_getData(const xStack *stack);

inline xBool xStack_isValid(const xStack *stack);

void xStack_push(xStack *stack, const void *data);

void *xStack_pop(xStack *stack);

const void *xStack_peek(const xStack *stack);

void xStack_clear(xStack *stack);

xStack xStack_copy(const xStack *stack);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_STACK_H
