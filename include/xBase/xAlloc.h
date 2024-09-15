/**
 * @file xAlloc.h
 * @author 0xDontCare (you@domain.com)
 * @brief Memory allocation and deallocation functions for xcFramework.
 * @version 0.1
 * @date 08.08.2024.
 *
 * Module provides memory allocation and deallocation functions independent of standard library for xcFramework.
 * Functions should not be mixed with standard library functions like malloc, free, etc.
 */

#ifndef XBASE_ALLOC_H
#define XBASE_ALLOC_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
/**
 * @brief Memory block structure used by custom allocator implementation.
 */
typedef struct xMemblock_s xMemblock;

#define XMEMBLOCK_SIZE sizeof(xMemblock)
#endif  // __linux__ || __APPLE__ || __FreeBSD__

/**
 * @brief
 * Allocate memory block of specified size.
 *
 * @param size Size of memory block in bytes.
 * @return void* Pointer to allocated memory block.
 *
 * @warning
 * Memory block allocated by this function may not be compatible with standard library free function. Use xMem_free instead.
 *
 * @note
 * If memory allocation fails, function will return NULL.
 */
void *xMem_alloc(xSize size);

/**
 * @brief
 * Deallocate memory block.
 *
 * @param ptr Pointer to memory block.
 *
 * @warning
 * Memory block deallocated by this function may not be compatible with blocks allocated by standard library allocation functions
 * like malloc, calloc, etc.
 *
 * @note
 * If pointer is NULL, function will do nothing.
 *
 * @warning
 * If pointer is not NULL, but points to memory block which was not allocated by xMem_alloc or has been already deallocated will
 * cause undefined behavior.
 */
void xMem_free(void *ptr);

/**
 * @brief
 * Reallocate memory block to new size.
 *
 * @param ptr Pointer to memory block.
 * @param size New size of memory block in bytes.
 * @return void* Pointer to reallocated memory block.
 *
 * @note
 * If pointer is NULL, function will behave like xMem_alloc.
 * If size is 0, function will behave like xMem_free.
 * If reallocation fails, function will return NULL.
 *
 * @warning
 * Memory block reallocated by this function may not be compatible with blocks allocated by standard library allocation functions
 * like malloc, calloc, etc.
 */
void *xMem_realloc(void *ptr, xSize size);

/**
 * @brief
 * Allocate memory block of specified size and set all bytes to zero.
 *
 * @param size Size of memory block in bytes.
 * @return void* Pointer to allocated memory block.
 *
 * @note
 * If memory allocation fails, function will return NULL.
 *
 * @warning
 * Memory block allocated by this function may not be compatible with standard library free function. Use xMem_free instead.
 *
 * @remarks
 * Function is equivalent to calling xMem_alloc followed by xMemSet with zero value.
 */
void *xMem_calloc(xSize size);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // XBASE_ALLOC_H
