#include "xBase/xAlloc.h"
#include "xBase/xTypes.h"

#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
#include <unistd.h>           // sbrk
#include "xBase/xMemtools.h"  // xMemCopy
#else
#include <stdlib.h>  // delegate to standard library functions
#endif

#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
// custom block descriptor and helper functions if on Unix-like system

struct xMemblock_s {
    xSize size;
    xMemblock *next;
    xBool isFree;
};

static xMemblock *blockList = NULL;

static void xMemblock_split(xMemblock *block, xSize size)
{
    // safety check if block can be split
    if (block->size <= size + XMEMBLOCK_SIZE) {
        return;
    }

    xMemblock *newBlock = (xMemblock *)((xUInt8 *)block + size + XMEMBLOCK_SIZE);
    newBlock->size = block->size - size - XMEMBLOCK_SIZE;
    newBlock->isFree = true;
    newBlock->next = block->next;

    block->size = size;
    block->isFree = false;
    block->next = newBlock;
}

static void xMemblock_mergeFree(void)
{
    // check if there are at least two blocks
    if (!blockList || !blockList->next) {
        return;
    }

    xMemblock *curr = blockList;
    while (curr && curr->next) {
        if (curr->isFree == true && curr->next->isFree == true) {
            curr->size += curr->next->size + XMEMBLOCK_SIZE;
            curr->next = curr->next->next;
        }
        curr = curr->next;
    }
}

xMemblock *xMemblock_findFree(xSize size)
{
    xMemblock *curr = blockList;

    while (curr && !(curr->isFree == true && curr->size >= size)) {
        curr = curr->next;
    }

    return curr;
}

static xMemblock *xMemblock_requestBlock(xSize size)
{
    xMemblock *block = (xMemblock *)sbrk(0);      // get current break position
    void *request = sbrk(size + XMEMBLOCK_SIZE);  // request new block

    if (request == (void *)(-1)) {
        // allocation failed
        return NULL;
    }

    block->size = size;
    block->isFree = false;
    block->next = NULL;
    return block;
}
#endif  // (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)

void *xMem_alloc(xSize size)
{
#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
    // custom implementation for Unix-like systems

    // check if size is valid
    if (!size) {
        return NULL;
    }

    xMemblock *block = NULL;
    if (!blockList) {
        // no free blocks, request new block
        block = xMemblock_requestBlock(size);
        if (!block) {
            // allocation failed
            return NULL;
        }
        blockList = block;
    } else {
        // try to find free block
        block = xMemblock_findFree(size);
        if (!block) {
            // no fitting block found, request new block
            block = xMemblock_requestBlock(size);
            if (!block) {
                // allocation failed
                return NULL;
            }
        } else {
            // fitting block found, split if necessary
            if (block->size > size + XMEMBLOCK_SIZE) {
                xMemblock_split(block, size);
            }
            block->isFree = false;
        }
    }

    return (void *)((xUInt8 *)block + XMEMBLOCK_SIZE);
#else
    // fallback to standard malloc function
    return malloc(size);
#endif
}

void xMem_free(void *ptr)
{
#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
    // custom implementation for Unix-like systems

    // check if pointer is valid
    if (!ptr) {
        return;
    }

    // get block descriptor in front of the pointer
    xMemblock *block = (xMemblock *)((xUInt8 *)ptr - XMEMBLOCK_SIZE);

    // check if block is already free
    if (block->isFree) {
        return;
    }

    // mark block as free and attempt to merge all free blocks
    block->isFree = true;
    xMemblock_mergeFree();
#else
    // fallback to standard free function
    free(ptr);
#endif
}

void *xMem_realloc(void *ptr, xSize size)
{
#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
    // custom implementation for Unix-like systems

    // check if pointer is valid
    if (!ptr) {
        return xMem_alloc(size);
    }

    // check if size is valid
    if (!size) {
        xMem_free(ptr);
        return NULL;
    }

    // get block descriptor in front of the pointer
    xMemblock *block = (xMemblock *)((xUInt8 *)ptr - XMEMBLOCK_SIZE);

    // check if block is already free
    if (block->isFree) {
        return NULL;
    }

    // check if new size is smaller than current size
    if (size <= block->size) {
        // split block if necessary
        if (block->size > size + XMEMBLOCK_SIZE) {
            xMemblock_split(block, size);
        }
        return ptr;
    } else {
        if (block->next && block->next->isFree == true && block->size + block->next->size + XMEMBLOCK_SIZE >= size) {
            // merge blocks together if total size is enough
            block->size += block->next->size + XMEMBLOCK_SIZE;
            block->next = block->next->next;

            return ptr;
        } else {
            // allocate new block and copy data
            void *newPtr = xMem_alloc(size);
            if (!newPtr) {
                return NULL;
            }

            // copy data from old block to new block and free old block
            xMemCopy(newPtr, ptr, block->size);
            xMem_free(ptr);
            return newPtr;
        }
    }
#else
    // fallback to standard realloc function
    return realloc(ptr, size);
#endif
}

void *xMem_calloc(xSize size)
{
#if (defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)) && !defined(XC_USE_STANDARD)
    void *ptr = xMem_alloc(size);
    if (ptr) {
        xMemSet(ptr, 0, size);
    }
    return ptr;
#else
    // fallback to standard calloc function
    return calloc(1, size);
#endif
}
