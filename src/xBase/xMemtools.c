#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"

void xMemCopy(void *dest, const void *src, xSize size)
{
    // check parameter validity
    if (dest == NULL || src == NULL || size == 0) {
        return;
    }

    // cast memory to dereferencable byte arrays
    xUInt8 *destP = (xUInt8 *)dest;
    const xUInt8 *srcP = (const xUInt8 *)src;

    // TODO: copying in larger chunks

    // copy memory block chunk by chunk
    for (xSize i = 0; i < size; i++) {
        destP[i] = srcP[i];
    }
}

void xMemMove(void *dest, const void *src, xSize size)
{
    // check parameter validity
    if (dest == NULL || src == NULL || size == 0) {
        return;
    }

    // cast memory to dereferencable byte arrays
    xUInt8 *destP = (xUInt8 *)dest;
    const xUInt8 *srcP = (const xUInt8 *)src;

    // TODO: moving in larger chunks

    // Check if source and destination memory blocks overlap
    if (destP < srcP) {
        for (xSize i = 0; i < size; i++) {
            destP[i] = srcP[i];
        }
    } else {
        for (xSize i = size; i > 0; i--) {
            destP[i - 1] = srcP[i - 1];
        }
    }
}

void xMemSet(void *dest, xUInt8 value, xSize size)
{
    // check parameter validity
    if (dest == NULL || size == 0) {
        return;
    }

    // cast memory to dereferencable byte array
    xUInt8 *destP = (xUInt8 *)dest;

    // TODO: setting in larger chunks

    // set memory block to the specified value
    for (xSize i = 0; i < size; i++) {
        destP[i] = value;
    }
}

xBool xMemCmp(const void *block1, const void *block2, xSize size)
{
    // check parameter validity
    if (block1 == NULL) {
        return (block2 == NULL) ? true : false;
    } else if (block2 == NULL) {
        return false;
    } else if (size == 0) {
        // if size is actually zero (there is nothing to compare), return true by default
        return true;
    }

    // cast memory to dereferencable byte arrays
    const xUInt8 *block1P = (const xUInt8 *)block1;
    const xUInt8 *block2P = (const xUInt8 *)block2;

    // TODO: comparing in larger chunks

    // compare memory blocks chunk by chunk
    for (xSize i = 0; i < size; i++) {
        if (block1P[i] != block2P[i]) {
            return false;
        }
    }

    return true;
}

xUInt64 xMemHash(const void *block, xSize size)
{
    // FNV-1a hash algorithm (64-bit)
    const xUInt64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
    const xUInt64 FNV_PRIME = 1099511628211ULL;

    // check parameter validity
    if (size == 0) {
        // if size is actually zero (regardless of block address), return the basis offset
        return FNV_OFFSET_BASIS;
    } else if (block == NULL) {
        // if block address is invalid (but size is not zero), return zero
        return 0;
    }

    // cast memory block to byte array
    const xUInt8 *blockP = (const xUInt8 *)block;
    xUInt64 hash = FNV_OFFSET_BASIS;

    // perform hash calculation
    for (xSize i = 0; i < size; i++) {
        hash ^= blockP[i];
        hash *= FNV_PRIME;
    }

    return hash;
}

void xMemSwap(void *a, void *b, xSize size)
{
    // check parameter validity
    if (a == NULL || b == NULL || size == 0) {
        return;
    }

    // cast memory to dereferencable byte arrays
    xUInt8 *aP = (xUInt8 *)a;
    xUInt8 *bP = (xUInt8 *)b;

    // TODO: swapping in larger chunks

    // swap memory blocks chunk by chunk using XOR
    for (xSize i = 0; i < size; i++) {
        aP[i] ^= bP[i];
        bP[i] ^= aP[i];
        aP[i] ^= bP[i];
    }
}
