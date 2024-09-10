#include "xStructures/xArray.h"
#include <stdlib.h>  // standard library (for malloc, free)
#include "xBase/xMemtools.h"
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xArray_s {
    void *data;
    xSize elemSize;
    xSize arrSize;
    xSize arrCapacity;
};

xArray *xArray_new(xSize elemSize)
{
    // validate passed argument
    if (elemSize == 0) {
        return NULL;
    }

    // allocate array base struct
    xArray *arr = NULL;
    if (!(arr = (xArray *)malloc(sizeof(xArray)))) {
        return NULL;
    }

    // initialize array arguments
    arr->data = NULL;
    arr->elemSize = elemSize;
    arr->arrSize = 0;
    arr->arrCapacity = 0;

    return arr;
}

void xArray_free(xArray *arr)
{
    if (!arr || !arr->data) {
        return;
    }

    free(arr->data);
    free(arr);
}

inline xSize xArray_getSize(const xArray *arr) { return (arr) ? arr->arrSize : 0; }

inline xSize xArray_getCapacity(const xArray *arr) { return (arr) ? arr->arrCapacity : 0; }

inline xSize xArray_getElemSize(const xArray *arr) { return (arr) ? arr->elemSize : 0; }

inline const void *xArray_getData(const xArray *arr) { return (arr) ? arr->data : NULL; }

inline xBool xArray_isValid(const xArray *arr) { return (arr && arr->elemSize) ? true : false; }

void xArray_resize(xArray *arr, xSize newSize)
{
    // check if valid array is passed
    if (!xArray_isValid(arr)) {
        return;
    }

    // truncate array if target size is smaller than current array size
    if (newSize <= arr->arrSize) {
        arr->arrSize = newSize;
        return;
    }

    // calculate new array capacity in powers of 2
    xSize newCapacity = arr->arrCapacity ? arr->arrCapacity : 1;
    while (newCapacity < newSize) {
        newCapacity *= 2;
    }

    // reallocate memory block of the array
    void *newData = (void *)realloc(arr->data, newCapacity * arr->elemSize);
    if (!newData) {
        return;
    }

    // update array attributes
    arr->data = newData;
    arr->arrCapacity = newCapacity;
}

void xArray_push(xArray *arr, const void *elem)
{
    // validate arguments
    if (!xArray_isValid(arr) || !elem) {
        return;
    }

    // ensure that there is enough space for new element
    xArray_resize(arr, arr->arrSize + 1);
    if (arr->arrCapacity < arr->arrSize + 1) {
        // we failed to expand array size, cancel every operation
        return;
    }

    // copy over new element data into array
    xMemCopy((void *)((char *)arr->data + (arr->arrSize * arr->elemSize)), elem, arr->elemSize);
    arr->arrSize++;
}

void *xArray_pop(xArray *arr)
{
    // validate arguments
    if (!xArray_isValid(arr) || !arr->data || !arr->arrSize) {
        // array is either invalid or empty
        return NULL;
    }

    // calculate position of return value and shrink array
    void *ret = (void *)((char *)arr->data + (arr->elemSize * (arr->arrSize - 1)));
    xArray_resize(arr, arr->arrSize - 1);

    return ret;
}

void xArray_insert(xArray *arr, xSize index, const void *item)
{
    // validate arguments
    if (!xArray_isValid(arr) || !item || index > arr->arrSize) {
        // invalid data given, do nothing
        return;
    } else if (index == arr->arrSize) {
        // insert at the end of the array
        xArray_push(arr, item);
        return;
    }
    // ensure there is enough space
    xArray_resize(arr, arr->arrSize + 1);
    if (arr->arrCapacity < arr->arrSize + 1) {
        // failed to expand array memory, do nothing
        return;
    }

    // shift elements to the right of insertion point
    xMemMove((void *)((char *)arr->data + ((index + 1) * arr->elemSize)), (void *)((char *)arr->data + (index * arr->elemSize)),
             (arr->arrSize - index) * arr->elemSize);

    // copy over inserted element to given index
    xMemCopy((void *)((char *)arr->data + (index * arr->elemSize)), item, arr->elemSize);

    arr->arrSize++;
}

void *xArray_get(const xArray *arr, xSize index)
{
    // validate arguments
    if (!xArray_isValid(arr) || !arr->data || index >= arr->arrSize) {
        return NULL;
    }

    return (void *)((char *)arr->data + (index * arr->elemSize));
}

void xArray_remove(xArray *arr, xSize index)
{
    // validate arguments
    if (!xArray_isValid(arr) || index >= arr->arrSize) {
        return;
    }

    // shift elements to the left and shrink array
    xMemMove((void *)((char *)arr->data + (index * arr->elemSize)), (void *)((char *)arr->data + ((index + 1) * arr->elemSize)),
             arr->elemSize);
    xArray_resize(arr, arr->arrSize - 1);
}

void xArray_clear(xArray *arr) { xArray_resize(arr, 0); }

/**
 * @brief
 * Sort array using bubble sort algorithm.
 *
 * @param arr Pointer to array data.
 * @param arrSize Size of array.
 * @param elemSize Size of array element.
 * @param cmp Comparator function (returns positive value if first element is greater, negative if second element is greater, zero
 * if equal).
 */
static void xArray_bubbleSort(void *arr, xSize arrSize, xSize elemSize, int (*cmp)(const void *, const void *))
{
    for (xSize i = 0; i < arrSize - 1; i++) {
        for (xSize j = 0; j < arrSize - i - 1; j++) {
            if (cmp((void *)((char *)arr + (j * elemSize)), (void *)((char *)arr + ((j + 1) * elemSize))) > 0) {
                xMemSwap((void *)((char *)arr + (j * elemSize)), (void *)((char *)arr + ((j + 1) * elemSize)), elemSize);
            }
        }
    }
}

/**
 * @brief
 * Sort array using quick sort algorithm with cutoff to bubble sort.
 *
 * @param arr Pointer to array data.
 * @param left Left index.
 * @param right Right index.
 * @param cmp Comparator function.
 */
static void xArray_quickSort(void *arr, xSize left, xSize right, xSize elemSize, int (*cmp)(const void *, const void *))
{
    if (left >= right) {
        return;
    }

    if (right - left < 10) {
        xArray_bubbleSort((void *)((char *)arr + (left * elemSize)), right - left + 1, elemSize, cmp);
        return;
    }

    xSize i = left;
    xSize j = right;
    void *pivot = (void *)((char *)arr + ((left + right) / 2 * elemSize));

    while (i <= j) {
        while (cmp((void *)((char *)arr + (i * elemSize)), pivot) < 0) {
            i++;
        }
        while (cmp((void *)((char *)arr + (j * elemSize)), pivot) > 0) {
            j--;
        }

        if (i <= j) {
            xMemSwap((void *)((char *)arr + (i * elemSize)), (void *)((char *)arr + (j * elemSize)), elemSize);
            i++;
            j--;
        }
    }

    if (left < j) {
        xArray_quickSort(arr, left, j, elemSize, cmp);
    }
    if (i < right) {
        xArray_quickSort(arr, i, right, elemSize, cmp);
    }
}

void xArray_sort(xArray *arr, int (*cmp)(const void *, const void *))
{
    // validate arguments
    if (!xArray_isValid(arr) || !arr->data || !cmp || arr->arrSize == 0) {
        return;
    }

    // sort array using quick sort algorithm
    xArray_quickSort(arr->data, 0, arr->arrSize - 1, arr->elemSize, cmp);
}

void xArray_foreach(const xArray *arr, void (*callback)(const void *))
{
    if (!arr || !callback) {
        return;
    }

    for (xSize i = 0; i < arr->arrSize; i++) {
        callback((void *)((char *)arr->data + (i * arr->elemSize)));
    }
}

xArray *xArray_copy(const xArray *arr)
{
    // validate arguments
    if (!xArray_isValid(arr)) {
        return NULL;
    }

    // create new array object and return if source array is empty
    xArray *copy = xArray_new(arr->elemSize);
    if (!arr->data || arr->arrSize == 0) {
        return copy;
    }

    // allocate memory for new array data
    xArray_resize(copy, arr->arrSize);
    if (copy->arrCapacity < arr->arrSize) {
        // failed to allocate memory, free array and return NULL
        xArray_free(copy);
        return NULL;
    }

    // copy over data from source array
    xMemCopy(copy->data, arr->data, arr->arrSize * arr->elemSize);
    copy->arrSize = arr->arrSize;

    return copy;
}

void xArray_append(xArray *arr, const xArray *other)
{
    // validate arguments
    if (!xArray_isValid(arr) || !xArray_isValid(other) || !other->data || other->arrSize == 0) {
        return;
    }

    // ensure there is enough space in target array
    xArray_resize(arr, arr->arrSize + other->arrSize);
    if (arr->arrCapacity < arr->arrSize + other->arrSize) {
        // failed to allocate memory, do nothing and return
        return;
    }

    // copy over data from source array
    xMemCopy((void *)((char *)arr->data + (arr->arrSize * arr->elemSize)), other->data, other->arrSize * other->elemSize);
    arr->arrSize += other->arrSize;
}

xArray *xArray_filter(const xArray *arr, xBool (*predicate)(const void *))
{
    // validate arguments
    if (!xArray_isValid(arr) || !predicate) {
        return NULL;
    } else if (!arr->data || arr->arrSize == 0) {
        // return empty array if the source array is empty
        return xArray_new(arr->elemSize);
    }

    // create new array object
    xArray *filtered = xArray_new(arr->elemSize);
    if (!xArray_isValid(filtered)) {
        return NULL;
    }

    // iterate over source array and filter elements
    for (xSize i = 0; i < arr->arrSize; i++) {
        void *elem = (void *)((char *)arr->data + (i * arr->elemSize));
        if (predicate(elem)) {
            xArray_push(filtered, elem);
        }
    }

    return filtered;
}

xArray *xArray_map(const xArray *arr, void *(*mapper)(const void *))
{
    // validate arguments
    if (!xArray_isValid(arr) || !mapper) {
        return NULL;
    } else if (!arr->data || arr->arrSize == 0) {
        // return empty array if the source one is empty
        return xArray_new(arr->elemSize);
    }

    // create new array object
    xArray *mapped = xArray_new(arr->elemSize);
    if (!xArray_isValid(mapped)) {
        return NULL;
    }

    // iterate over source array and map elements
    for (xSize i = 0; i < arr->arrSize; i++) {
        void *elem = (void *)((char *)arr->data + (i * arr->elemSize));
        xArray_push(mapped, mapper(elem));
    }

    return mapped;
}
