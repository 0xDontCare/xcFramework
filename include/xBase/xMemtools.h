/**
 * @file xMemtools.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Memory manipulation functions for xcFramework.
 * @version 0.2
 * @date 06.08.2024.
 *
 * Module implements memory manipulation functions like copying, setting and comparing memory blocks.
 */

#ifndef XBASE_MEMTOOLS_H
#define XBASE_MEMTOOLS_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief
 * Spread single byte value along larger type.
 */
#define XMEM_SPREAD_64(x) ((xUInt64)(x) * 0x0101010101010101ULL)
#define XMEM_SPREAD_32(x) ((xUInt32)(x) * 0x01010101UL)
#define XMEM_SPREAD_16(x) ((xUInt16)(x) * 0x0101U)

/**
 * @brief
 * Copies memory block from source to destination.
 *
 * @param dest Destination memory block.
 * @param src Source memory block.
 * @param size Size of memory block in bytes.
 *
 * @note
 * If source and destination memory blocks overlap, data could be corrupted. Use xMemMove() instead.
 */
void xMemCopy(void *dest, const void *src, xSize size);

/**
 * @brief
 * Copies memory block from source to destination with checking for overlap.
 *
 * @param dest Destination memory block.
 * @param src Source memory block.
 * @param size Size of memory block in bytes.
 *
 * @note
 * This function is slower than xMemCopy() because it checks for overlap. Use this function if you are not sure if memory blocks
 * overlap.
 */
void xMemMove(void *dest, const void *src, xSize size);

/**
 * @brief
 * Sets memory block with specified value.
 *
 * @param dest Destination memory block.
 * @param value Value to set.
 * @param size Size of memory block in bytes.
 */
void xMemSet(void *dest, xUInt8 value, xSize size);

/**
 * @brief
 * Compares two memory blocks.
 *
 * @param block1 First memory block.
 * @param block2 Second memory block.
 * @param size Size of memory block in bytes.
 *
 * @return xBool::true if memory blocks are equal, xBool::false otherwise.
 */
xBool xMemCmp(const void *block1, const void *block2, xSize size);

/**
 * @brief
 * Calculate hash of data within memory block using general purpose hash function.
 *
 * @param block Address of memory block.
 * @param size Size of memory block in bytes.
 * @return xUInt64 Hash value.
 *
 * @note
 * This function is not suitable for cryptographic purposes.
 */
xUInt64 xMemHash(const void *block, xSize size);

/**
 * @brief
 * Swap data between two memory blocks.
 *
 * @param a Pointer to first memory block.
 * @param b Pointer to second memory block.
 * @param size Size of memory block in bytes.
 *
 * @note
 * This function performs XOR swapping between two memory blocks of the same size.
 *
 * @warning
 * This function is not suitable for swapping memory blocks of different sizes.
 */
void xMemSwap(void *a, void *b, xSize size);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // XBASE_MEMTOOLS_H
