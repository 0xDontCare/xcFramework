#include "xString/xString.h"
#include <stdlib.h>           // malloc, realloc, free
#include "xBase/xMemtools.h"  // copy, set, compare and hash function
#include "xBase/xTypes.h"     // xSize, xChar, XSIZE_MAX

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xString_s {
    xChar *data;         // pointer to beginning of string
    xSize length;        // length of string from beginning address
    xSize capacity;      // allocated memory size
    xSize *refCount;     // reference counter (always should be allocated)
    xChar *baseAddress;  // base address of data (for freeing memory if string is only a part of a larger memory block)
};

xSize cstrlen(const xChar *str)
{
    // check validity of passed pointer
    if (!str) {
        return 0;
    }

    // calculate length of the string
    xSize len = 0;
    while (str[len]) {
        len++;
    }

    return len;
}

xString *xString_new(void)
{
    // allocate memory for the string struct
    xString *ret = NULL;
    if (!(ret = (xString *)malloc(sizeof(xString)))) {
        return NULL;
    }

    // zero-initialize the struct
    ret->data = NULL;
    ret->length = 0;
    ret->capacity = 0;
    ret->refCount = NULL;
    ret->baseAddress = NULL;

    // allocate reference counter
    if (!(ret->refCount = (xSize *)malloc(sizeof(xSize)))) {
        free(ret);
        return NULL;
    }
    *ret->refCount = 1;

    return ret;
}

void xString_free(xString *str)
{
    // check validity of passed arguments
    if (!xString_isValid(str)) {
        return;
    }

    // free memory if reference counter hits zero
    if (--(*str->refCount) == 0) {
        free(str->refCount);
        str->refCount = NULL;
        if (str->baseAddress) {
            // string data is substring of larger block
            free(str->baseAddress);
            str->baseAddress = NULL;
        } else if (str->data) {
            // string data is standalone
            free(str->data);
        }
        str->data = NULL;
    }

    // free the string struct
    free(str);

    return;
}

inline xSize xString_getLength(const xString *str) { return xString_isValid(str) ? str->length : 0; }

inline xSize xString_getCapacity(const xString *str) { return xString_isValid(str) ? str->capacity : 0; }

inline const xChar *xString_getData(const xString *str) { return xString_isValid(str) ? str->data : NULL; }

inline xBool xString_isValid(const xString *str) { return (str && str->refCount) ? true : false; }

void xString_optimize(xString *str)
{
    // check validity of passed pointer
    if (!xString_isValid(str) || !str->data) {
        return;
    }

    // reallocate memory to fit the string
    if (*str->refCount > 1) {
        // string is shared, create standalone copy
        xChar *newData = (xChar *)malloc(str->length);
        if (!newData) {
            return;
        }
        xSize *newRefCount = (xSize *)malloc(sizeof(xSize));
        if (!newRefCount) {
            free(newData);
            return;
        }

        for (xSize i = 0; i < str->length; i++) {
            newData[i] = str->data[i];
        }
        *newRefCount = 1;

        (*str->refCount)--;
        str->data = newData;
        str->baseAddress = NULL;
        str->refCount = newRefCount;
        str->capacity = str->length;
    } else {
        // string is standalone, reallocate memory
        xChar *newData = (xChar *)realloc(str->data, str->length);
        if (!newData) {
            return;
        }
        str->data = newData;
        str->capacity = str->length;
    }
}

void xString_preallocate(xString *str, xSize size)
{
    // check validity of passed pointer and requested size
    if (!xString_isValid(str) || size == 0) {
        return;
    }

    // add requested size to current capacity
    if (*str->refCount > 1) {
        // string is shared, create standalone copy
        xChar *newData = (xChar *)malloc(str->length + size);
        if (!newData) {
            return;
        }
        xSize *newRefCount = (xSize *)malloc(sizeof(xSize));
        if (!newRefCount) {
            free(newData);
            return;
        }

        for (xSize i = 0; i < str->length + size; i++) {
            newData[i] = (i < str->length) ? str->data[i] : 0;
        }
        *newRefCount = 1;

        (*str->refCount)--;
        str->data = newData;
        str->baseAddress = NULL;
        str->refCount = newRefCount;
        str->capacity += size;
    } else {
        // string is standalone, reallocate memory
        xChar *newData = (xChar *)realloc(str->data, str->length + size);
        if (!newData) {
            return;
        }

        for (xSize i = str->length; i < str->length + size; i++) {
            newData[i] = 0;
        }

        str->data = newData;
        str->capacity += size;
    }
}

void xString_clear(xString *str)
{
    // check validity of passed pointer
    if (!xString_isValid(str) || !str->data) {
        return;
    }

    // check if string is shared
    if (*str->refCount > 1) {
        // string is shared, create new blank string
        xSize *newRefCount = (xSize *)malloc(sizeof(xSize));
        if (!newRefCount) {
            return;
        }
        *newRefCount = 1;

        // decrement previous reference counter
        (*str->refCount)--;

        // clear string attributes
        str->data = NULL;
        str->baseAddress = NULL;
        str->length = 0;
        str->capacity = 0;
        str->refCount = newRefCount;
    } else {
        // string is standalone, just reset length
        str->length = 0;
    }
}

xString *xString_copy(const xString *str)
{
    // check validity of passed pointer
    if (!xString_isValid(str) || !str->data) {
        // create and return blank string if input is invalid
        return xString_new();
    }

    // allocate struct for copy string
    xString *ret = NULL;
    if (!(ret = (xString *)malloc(sizeof(xString)))) {
        // NULL because we already failed to allocate memory
        return NULL;
    }

    // copy the string attributes
    *ret = *str;

    // increment reference counter
    (*ret->refCount)++;

    return ret;
}

xString *xString_copyDetached(const xString *str)
{
    // check validity of passed pointer
    if (!xString_isValid(str) || !str->data) {
        return xString_new();
    }

    // allocate struct for string copy
    xString *ret = NULL;
    if (!(ret = (xString *)malloc(sizeof(xString)))) {
        // NULL because memory allocation already failed once
        return NULL;
    }

    // allocate heap elements of string object (data and reference counter)
    if (!(ret->data = (xChar *)malloc(str->length * sizeof(xChar)))) {
        free(ret);
        return NULL;
    } else if (!(ret->refCount = (xSize *)malloc(sizeof(xSize)))) {
        free(ret->data);
        free(ret);
        return NULL;
    }

    // copy string data and initialize string attributes
    xMemCopy(ret->data, str->data, str->length);
    ret->length = str->length;
    ret->capacity = str->length;
    *ret->refCount = 1;
    ret->baseAddress = NULL;

    return ret;
}

xString *xString_append(const xString *str, const xChar *data, xSize len)
{
    // check validity of passed arguments
    if (!xString_isValid(str)) {
        // string is invalid, create new one only from data to be appended
        return xString_fromCStringS(data, len);
    } else if (!data || len == 0) {
        // data is invalid, return copy of original string
        return xString_copy(str);
    }

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        // failed to create new string so nothing else can be done
        return NULL;
    }

    // expand string memory to fit new data
    xString_preallocate(ret, len);
    if (ret->capacity < ret->length + len) {
        // failed to expand stirng
        return NULL;
    }

    // append data to the string
    xMemCopy(ret->data + ret->length, data, len);
    ret->length += len;

    return ret;
}

xString *xString_substring(const xString *str, xSize start, xSize end)
{
    // check validity of passed arguments
    if (!xString_isValid(str) || !str->data || start >= end || start >= str->length) {
        // if string or index interval is invalid, create and return blank string
        return xString_new();
    }

    // clamp end index to string length
    end = (end > str->length) ? str->length : end;

    // create copy of string
    xString *ret = xString_copy(str);
    if (!ret) {
        // failed to allocate memory once so return NULL
        return NULL;
    }

    // update attribute values
    ret->baseAddress = ret->data;  // base address is now used for freeing
    ret->data += start;            // shift string start pointer
    ret->length = end - start;     // calculate string length
    ret->capacity -= start;        // reduce capacity by amount the data pointer is shifted

    return ret;
}

/**
 * @brief
 * Assisting function for xString pattern matching functions.
 *
 * @param pattern Pointer to the pattern string.
 * @param len Length of the pattern string.
 * @return int* Pointer to the LPS array.
 *
 * @note
 * The returned array should be freed after use.
 *
 * @note
 * Length of returned array is equal to the length of the pattern string.
 */
static xSize *xString_LPS(const xChar *pattern, xSize len)
{
    if (!pattern || !len) {
        return NULL;
    }

    xSize *lps = (xSize *)malloc(len * sizeof(xSize));
    if (!lps) {
        return NULL;
    }

    lps[0] = 0;
    xSize j = 0;
    for (xSize i = 1; i < len; i++) {
        if (pattern[i] == pattern[j]) {
            lps[i] = ++j;
        } else {
            if (j) {
                j = lps[j - 1];
                i--;
            } else {
                lps[i] = 0;
            }
        }
    }

    return lps;
}

/**
 * @brief
 * Assisting function for xString pattern matching functions. Finds first occurrence of the pattern in the string starting from the
 * specified position.
 *
 * @param str Pointer to the string.
 * @param data Pointer to the pattern.
 * @param len Length of the pattern.
 * @param start Position in the string to start searching from.
 * @return xSize Position of the first occurrence of the pattern in the string, or XSIZE_MAX if the pattern is not found.
 */
static xSize xString_findNext(const xString *str, const xChar *data, xSize len, xSize start)
{
    // check validity of passed arguments
    if (!xString_isValid(str) || !data || len > str->length - start || start >= str->length) {
        return XSIZE_MAX;
    }

    // special case (short needles)
    if (len == 0) {
        // zero-length needle is always found at given index
        return start;
    } else if (len == 1) {
        // single character search
        for (xSize i = start; i < str->length; i++) {
            if (str->data[i] == data[0]) {
                return i;
            }
        }
        return XSIZE_MAX;
    }
    // TODO: add more short cases (2-char, 4-char)

    // create LPS array for the pattern
    xSize *lps = xString_LPS(data, len);
    if (!lps) {
        return XSIZE_MAX;
    }

    // search for the pattern in the string
    xSize i = start, j = 0;
    while (i < str->length) {
        if (str->data[i] == data[j]) {
            i++;
            j++;
        }

        if (j == len) {
            free(lps);
            return i - j;
        }

        if (i < str->length && str->data[i] != data[j]) {
            if (j) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return XSIZE_MAX;
}

static xSize xString_findPrev(const xString *str, const xChar *data, xSize len, xSize start)
{
    // validate passed arguments
    if (!xString_isValid(str) || !data || len > str->length) {
        // invalid arguments, return XSIZE_MAX
        return XSIZE_MAX;
    }

    // clamp start index to string length
    start = (start + len >= str->length) ? str->length - 1 : start + len;

    // special case (short needles)
    if (len == 0) {
        // zero-length needle is always found at given index
        return start;
    } else if (len == 1) {
        // single character search
        for (xSize i = start; i <= start; i--) {
            if (str->data[i] == data[0]) {
                return i;
            }
        }

        return XSIZE_MAX;
    }
    // TODO: add more short cases (2-char, 4-char)

    // create LPS array for the pattern
    xSize *lps = xString_LPS(data, len);
    if (!lps) {
        return XSIZE_MAX;
    }

    // search for the pattern in the string
    xSize i = 0, j = 0, ret = XSIZE_MAX;
    while ((str->length - i) >= (len - j)) {
        if (str->data[i] == data[j]) {
            i++;
            j++;
        }
        if (j == len) {
            ret = i - j;
            j = lps[j - 1];
        } else if ((i < str->length) && (str->data[i] != data[j])) {
            if (j) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    free(lps);
    return ret;
}

xSize xString_find(const xString *str, const xChar *data, xSize len) { return xString_findNext(str, data, len, 0); }

xSize xString_findLast(const xString *str, const xChar *data, xSize len)
{
    return xString_findPrev(str, data, len, str->length - 1);
}

xSize xString_count(const xString *haystack, const xChar *needle, xSize len)
{
    // check validity of passed arguments
    if (!haystack || !haystack->refCount || !needle || !len || len > haystack->length) {
        return 0;
    }

    if (len == 1) {
        // single character search
        xSize count = 0;
        for (xSize i = 0; i < haystack->length; i++) {
            if (haystack->data[i] == needle[0]) {
                count++;
            }
        }
        return count;
    } else {
        // multiple character pattern search
        xSize count = 0;
        xSize *lps = xString_LPS(needle, len);
        if (!lps) {
            return 0;
        }

        xSize i = 0, j = 0, k = XSIZE_MAX;
        while (i < haystack->length) {
            if (haystack->data[i] == needle[j]) {
                i++;
                j++;
            }
            if (j == len) {
                if (k == XSIZE_MAX || i - j > k) {
                    count++;
                    k = i - j;
                }
                j = lps[j - 1];
            } else if (i < haystack->length && haystack->data[i] != needle[j]) {
                if (j) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }

        free(lps);
        return count;
    }
}

xSize xString_count_overlapping(const xString *haystack, const xChar *needle, xSize len)
{
    // check validity of passed arguments
    if (!haystack || !haystack->refCount || !needle || !len || len > haystack->length) {
        return 0;
    }

    if (len == 1) {
        // single character search
        xSize count = 0;
        for (xSize i = 0; i < haystack->length; i++) {
            if (haystack->data[i] == needle[0]) {
                count++;
            }
        }
        return count;
    } else {
        // multiple character pattern search
        xSize count = 0;
        xSize *lps = xString_LPS(needle, len);
        if (!lps) {
            return 0;
        }

        xSize i = 0, j = 0;
        while (i < haystack->length) {
            if (haystack->data[i] == needle[j]) {
                i++;
                j++;
            }
            if (j == len) {
                count++;
                j = lps[j - 1];
            } else if (i < haystack->length && haystack->data[i] != needle[j]) {
                if (j) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }

        free(lps);
        return count;
    }
}

xString *xString_replaceFirst(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                              xSize replacementLen)
{
    // check validity of passed arguments
    if (!xString_isValid(str)) {
        return xString_new();
    } else if (!needle || !needleLen || needleLen > str->length) {
        return xString_copy(str);
    } else if (!replacement || !replacementLen) {
        xSize pos = xString_find(str, needle, needleLen);
        if (pos == XSIZE_MAX) {
            return xString_copy(str);
        } else {
            return xString_remove(str, pos, pos + needleLen);
        }
    }

    // get position of the needle in the string
    xSize pos = xString_find(str, needle, needleLen);
    if (pos == XSIZE_MAX) {
        return xString_copy(str);
    }

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        // allocation failed once, return NULL
        return NULL;
    }

    // replace the needle with the replacement
    if (needleLen < replacementLen) {
        // new memory is needed to fit the replacement
        xString_preallocate(ret, replacementLen - needleLen);

        // check if memory reallocation was successful
        if (ret->capacity < ret->length + replacementLen - needleLen) {
            xString_free(ret);
            return NULL;
        }

        // shift the rest of the string to the right
        for (xSize i = ret->length - 1; i >= pos + needleLen; i--) {
            ret->data[i + replacementLen - needleLen] = ret->data[i];
        }
    } else if (needleLen > replacementLen) {
        // replacement is smaller than the needle, no need for expanding memory

        // shift the rest of the string to the left
        for (xSize i = pos + needleLen; i < ret->length; i++) {
            ret->data[i + replacementLen - needleLen] = ret->data[i];
        }
    }

    // insert the replacement into the string
    xMemCopy(ret->data + pos, replacement, replacementLen);
    /*
    for (xSize i = 0; i < replacementLen; i++) {
        ret->data[pos + i] = replacement[i];
    }
    */

    ret->length += replacementLen - needleLen;

    return ret;
}

xString *xString_replaceLast(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                             xSize replacementLen)
{
    // check  validity of passed arguments
    if (!xString_isValid(str)) {
        return xString_new();
    } else if (!needle || !needleLen || needleLen > str->length) {
        return xString_copy(str);
    } else if (!replacement || !replacementLen) {
        xSize pos = xString_findLast(str, needle, needleLen);
        if (pos == XSIZE_MAX) {
            return xString_copy(str);
        } else {
            return xString_remove(str, pos, pos + needleLen);
        }
    }

    // get position of the needle in the string
    xSize pos = xString_findLast(str, needle, needleLen);
    if (pos == XSIZE_MAX) {
        return xString_copy(str);
    }

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        return NULL;
    }

    // replace the needle with the replacement
    if (needleLen < replacementLen) {
        // new memory is needed to fit the replacement
        xString_preallocate(ret, replacementLen - needleLen);

        // check if memory reallocation was successful
        if (ret->capacity < ret->length + replacementLen - needleLen) {
            xString_free(ret);
            return xString_new();
        }

        // shift the rest of the string to the right
        for (xSize i = ret->length - 1; i >= pos + needleLen; i--) {
            ret->data[i + replacementLen - needleLen] = ret->data[i];
        }
    } else if (needleLen > replacementLen) {
        // replacement is smaller than the needle, no need for expanding memory

        // shift the rest of the string to the left
        for (xSize i = pos + needleLen; i < ret->length; i++) {
            ret->data[i + replacementLen - needleLen] = ret->data[i];
        }
    }

    // insert the replacement into the string
    xMemCopy(ret->data + pos, replacement, replacementLen);
    /*
    for (xSize i = 0; i < replacementLen; i++) {
        ret->data[pos + i] = replacement[i];
    }
    */

    ret->length += replacementLen - needleLen;

    return ret;
}

xString *xString_replaceAll(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                            xSize replacementLen)
{
    // check  validity of passed arguments
    if (!xString_isValid(str)) {
        return xString_new();
    } else if (!needle || !needleLen || needleLen > str->length) {
        return xString_copy(str);
    }

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        return NULL;
    }

    // get count of replacements to perform
    xSize count = xString_count(str, needle, needleLen);
    if (!count) {
        return ret;
    }

    // preallocate memory for the replacements (if larger than the needle)
    if (needleLen < replacementLen) {
        xString_preallocate(ret, count * (replacementLen - needleLen));

        // check if memory reallocation was successful
        if (ret->capacity < ret->length + count * (replacementLen - needleLen)) {
            xString_free(ret);
            return xString_copy(str);
        }
    }

    // replace all occurrences of the needle with the replacement
    xSize pos = 0;
    for (xSize i = 0; i < count; i++) {
        pos = xString_findNext(ret, needle, needleLen, pos);
        if (pos == XSIZE_MAX) {
            break;
        }

        // replace the needle with the replacement
        if (needleLen < replacementLen) {
            // shift the rest of the string to the right
            for (xSize j = ret->length - 1; j >= pos + needleLen; j--) {
                ret->data[j + replacementLen - needleLen] = ret->data[j];
            }
        } else if (needleLen > replacementLen) {
            // shift the rest of the string to the left
            for (xSize j = pos + needleLen; j < ret->length; j++) {
                ret->data[j + replacementLen - needleLen] = ret->data[j];
            }
        }

        // insert the replacement into the string
        xMemCopy(ret->data + pos, replacement, replacementLen);
        /*
        for (xSize j = 0; j < replacementLen; j++) {
            ret->data[pos + j] = replacement[j];
        }
        */

        ret->length += replacementLen - needleLen;
        pos += replacementLen;
    }

    return ret;
}

xString *xString_remove(const xString *str, xSize start, xSize end)
{
    // check validity of passed arguments
    if (!str || !str->data) {
        return xString_new();
    } else if (start >= end || start >= str->length) {
        return xString_copy(str);
    }
    end = (end > str->length) ? str->length : end;  // clamp end index to string length

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        // allocation already failed onc, return NULL
        return NULL;
    }

    // remove target interval from string
    for (xSize i = end; i < ret->length; i++) {
        ret->data[start + i - end] = ret->data[i];
    }
    ret->length -= end - start;

    return ret;
}

xString *xString_insert(const xString *str, const xChar *data, xSize len, xSize index)
{
    // check validity of passed arguments
    if (!xString_isValid(str) || !str->data) {
        return xString_new();
    } else if (!data || len == 0 || index > str->length) {
        return xString_copy(str);
    } else if (index == str->length) {
        return xString_append(str, data, len);
    }

    // create standalone copy of the string
    xString *ret = xString_copyDetached(str);
    if (!ret) {
        return NULL;
    }

    // expand string to fit additional data
    xString_preallocate(ret, len);
    if (ret->capacity < ret->length + len) {
        xString_free(ret);
        return NULL;
    }

    // shift data after target index to the right to make place
    for (xSize i = ret->length - 1; i >= index; i--) {
        ret->data[i + len] = ret->data[i];

        // edge case when data is inserted at the beginning of the string
        if (index == 0 && i == 0) {
            break;
        }
    }

    // insert new data to string
    xMemCopy(ret->data + index, data, len);
    ret->length += len;

    return ret;
}

// xString *xString_split(const xString *str, const xChar *delimiter, xSize delimiterLen, xSize *count) {}

int xString_compare(const xString *str1, const xString *str2)
{
    // check validity of passed arguments
    if (!xString_isValid(str1) || !xString_isValid(str2)) {
        return 0;
    } else if (!str1->data) {
        // first string empty, return -1 if second string is not empty
        return (str2->data) ? -1 : 0;
    } else if (!str2->data) {
        // second string empty, return 1 since first string is not empty
        return 1;
    }

    // compare the strings
    xSize i = 0;
    while (i < str1->length && i < str2->length) {
        if (str1->data[i] != str2->data[i]) {
            return str1->data[i] - str2->data[i];
        }
        i++;
    }

    return str1->length - str2->length;
}

int xString_compareIgnoreCase(const xString *str1, const xString *str2)
{
    // check validity of passed arguments
    if (!xString_isValid(str1) || !xString_isValid(str2)) {
        return 0;
    } else if (!str1->data) {
        // first string empty, return -1 if second string is not empty
        return (str2->data) ? -1 : 0;
    } else if (!str2->data) {
        // second string empty, return 1 since first string is not empty
        return 1;
    }

    // compare the strings
    xSize i = 0;
    while (i < str1->length && i < str2->length) {
        // TODO: improve test condiition (possible overflow of xChar value)
        if (str1->data[i] != str2->data[i] && str1->data[i] != str2->data[i] + 32 && str1->data[i] != str2->data[i] - 32) {
            return str1->data[i] - str2->data[i];
        }
        i++;
    }

    return str1->length - str2->length;
}

xChar *xString_toCString(const xString *str)
{
    // check validity of passed arguments
    if (!xString_isValid(str) || !str->data) {
        return NULL;
    }

    // create a copy of the string
    xChar *ret = (xChar *)malloc(str->length + 1);
    if (!ret) {
        return NULL;
    }

    // copy the string data
    for (xSize i = 0; i < str->length; i++) {
        ret[i] = str->data[i];
    }
    ret[str->length] = '\0';

    return ret;
}

int xString_toInt(const xString *str) { return (int)xString_toLong(str); }

long long xString_toLong(const xString *str)
{
    // check validity of passed arguments
    if (!xString_isValid(str) || !str->data) {
        return 0;
    }

    // convert the string to long
    long long ret = 0;
    int sign = 1;
    xSize i = 0;

    // skip leading whitespaces
    while (i < str->length && str->data[i] == ' ') {
        i++;
    }

    // check for sign character on the beginning
    if (str->data[0] == '-' || str->data[0] == '+') {
        sign = (str->data[0] == '-') ? -1 : 1;
        i++;
    }

    // convert the string to long
    for (; i < str->length; i++) {
        if ((str->data[i] < '0' || str->data[i] > '9') && str->data[i] != ' ') {
            // if any non-digit character is found, string is not convertible
            return 0;
        } else if (str->data[i] == ' ') {
            // skip trailing whitespaces
            break;
        }
        ret = ret * 10 + str->data[i] - '0';
    }

    return ret * sign;
}

/**
 * @brief
 * Test single precision floating point number (float) for NaN value.
 *
 * @param value Single precision floating point number.
 * @return xBool true if the value is NaN, false otherwise.
 */
inline static xBool isnan_f(float value)
{
    union {
        float f;
        xUInt32 u;
    } val = {value};

    return ((val.u & 0x7f800000) == 0x7f800000) && (val.u & 0x007fffff) ? true : false;
}

/**
 * @brief
 * Test double precision floating point number (double) for NaN value.
 *
 * @param value Double precision floating point number.
 * @return xBool true if the value is NaN, false otherwise.
 */
inline static xBool isnan_d(double value)
{
    union {
        double d;
        xUInt64 u;
    } val = {value};

    return (val.u & 0x7ff0000000000000) == 0x7ff0000000000000 && (val.u & 0x000fffffffffffff) ? true : false;
}

/**
 * @brief
 * Test single precision floating point number (float) for infinity.
 *
 * @param value Single precision floating point number.
 * @return int 1 if value is positive infinity, -1 if value is negative infinity, 0 otherwise.
 */
inline static int isinf_f(float value)
{
    union {
        float f;
        xUInt32 u;
    } val = {value};

    return (((val.u & 0x7f800000) == 0x7f800000) && ((val.u & 0x007fffff) == 0)) * (val.u & 0x80000000 ? -1 : 1);
}

/**
 * @brief
 * Test double precision floating point number (double) for infinity.
 *
 * @param value Double precision floating point number.
 * @return int 1 if value is positive infinity, -1 if value is negative infinity, 0 otherwise.
 */
inline static int isinf_d(double value)
{
    union {
        double d;
        xUInt64 u;
    } val = {value};

    return ((val.u & 0x7ff0000000000000) == 0x7ff0000000000000 && (val.u & 0x000fffffffffffff) == 0) *
           (val.u & 0x8000000000000000 ? -1 : 1);
}

float xString_toFloat(const xString *str)
{
    double ret = xString_toDouble(str);
    if (isnan_d(ret)) {
        return 0.0f / 0.0f;
    } else if (isinf_d(ret) > 0) {
        return 1.0f / 0.0f;
    } else if (isinf_d(ret) < 0) {
        return -1.0f / 0.0f;
    }
    return (float)ret;
}

double xString_toDouble(const xString *str)
{
    // validate passed string
    if (!xString_isValid(str) || !str->data) {
        return 0.0 / 0.0;
    }

    // initialize local variables
    double result = 0.0;
    double sign = 1.0;
    double frac = 0.0;
    double div = 1.0;
    xBool dec = false;
    xSize i = 0;

    // skip leading whitespaces
    while (i < str->length && str->data[i] == ' ') {
        i++;
    }

    // handle sign character
    if (str->data[i] == '-' || str->data[i] == '+') {
        sign = (str->data[i] == '-') ? -1.0 : 1.0;
        i++;
    }

    // check special cases (NaN, Inf, -Inf)
    if (str->length - i >= 3) {
        if (xMemCmp((const void *)(str->data + i), (const void *)"NaN", 3)) {
            return 0.0 / 0.0;
        } else if (xMemCmp((const void *)(str->data + i), (const void *)"Inf", 3)) {
            return (sign * 1.0) / 0.0;
        }
    }

    // number conversion
    while (i < str->length) {
        char c = str->data[i];

        if (c >= '0' && c <= '9') {
            if (dec) {
                frac = frac * 10.0 + (c - '0');
                div *= 10.0;
            } else {
                result = result * 10.0 + (c - '0');
            }
        } else if (c == '.') {
            if (dec) {
                return 0.0 / 0.0;
            }
            dec = true;
        } else if (c == ' ') {
            break;
        } else {
            return 0.0 / 0.0;
        }

        i++;
    }

    result = result + frac / div;
    result *= sign;

    // skip trailing whitespaces
    while (i < str->length && str->data[i] == ' ') {
        i++;
    }

    // if all characters are consumed, return the result, else NaN
    return (i == str->length) ? result : 0.0 / 0.0;

    /*
    // check validity of passed arguments
    if (!xString_isValid(str) || !str->data) {
        return (double)0x7ff8000000000001;
    }

    // convert the string to double
    double ret = 0.0;
    int sign = 1;
    xSize i = 0;

    // skip leading whitespaces
    while (i < str->length && str->data[i] == ' ') {
        i++;
    }

    // handle sign character
    if (str->data[i] == '-' || str->data[i] == '+') {
        sign = (str->data[i] == '-') ? -1 : 1;
        i++;
    }

    // check for NaN and infinity values
    if (str->length - i >= 3) {
        if (str->data[i] == 'N' && str->data[i + 1] == 'a' && str->data[i + 2] == 'N') {
            return (double)0x7ff8000000000001;
        } else if (str->data[i] == 'I' && str->data[i + 1] == 'n' && str->data[i + 2] == 'f') {
            return (double)(sign > 0 ? 0x7ff0000000000000 : 0xfff0000000000000);
        }
    }

    // convert the integer part of the number
    for (; i < str->length; i++) {
        if (str->data[i] == '.') {
            break;
        }
        if ((str->data[i] < '0' || str->data[i] > '9') && str->data[i] != ' ') {
            // invalid character found, stop conversion and return NaN
            return (double)0x7ff8000000000001;
        } else if (str->data[i] == ' ') {
            // skip trailing whitespaces
            break;
        }

        ret = ret * 10 + str->data[i] - '0';
    }

    double dec = 0.0;
    double div = 1.0;
    for (i++; i < str->length; i++) {
        if ((str->data[i] < '0' || str->data[i] > '9') && str->data[i] != ' ') {
            // invalid character, return NaN
            return (double)0x7ff8000000000001;
        } else if (str->data[i] == ' ') {
            // skip trailing whitespaces
            break;
        }
        dec = dec * 10 + str->data[i] - '0';
        div *= 10;
    }

    return (ret + dec / div) * sign;
    */
}

xString *xString_fromCString(const xChar *cstr)
{
    // check validity of passed arguments
    if (!cstr || !cstr[0]) {
        // invalid address or empty string case
        return xString_new();
    }

    // create a new string
    xString *ret = xString_new();
    if (!xString_isValid(ret)) {
        // memory allocation failed once, return NULL
        return NULL;
    }

    // allocate memory for the string data
    ret->data = (xChar *)malloc(cstrlen(cstr) * sizeof(xChar));
    if (!ret->data) {
        // not enough memory can be allocated. return NULL
        xString_free(ret);
        return NULL;
    }

    // copy the data from the C string
    ret->length = ret->capacity = cstrlen(cstr);
    xMemCopy(ret->data, cstr, ret->length);

    return ret;
}

xString *xString_fromCStringS(const xChar *cstr, xSize len)
{
    // check validity of passed arguments
    if (!cstr || !len) {
        // if invalid address or copy length is zero, return NULL
        return xString_new();
    }

    // create a new string
    xString *ret = xString_new();
    if (!xString_isValid(ret)) {
        // failed to allocate memory, return NULL
        return NULL;
    }

    // allocate memory for the string data
    ret->data = (xChar *)malloc(len);
    if (!ret->data) {
        // data allocation failed, clear object and return NULL
        xString_free(ret);
        return NULL;
    }

    // copy the data from the C string
    ret->length = ret->capacity = len;
    xMemCopy(ret->data, cstr, len);

    return ret;
}

xString *xString_fromInt(int value) { return xString_fromLong((long long)value); }

xString *xString_fromLong(long long value)
{
    // special case for zero value
    if (value == 0) {
        return xString_fromCString("0");
    }

    // create a new string
    xString *ret = xString_new();
    if (!xString_isValid(ret)) {
        return ret;
    }

    // determine the sign of the value and convert to absolute value
    int sign = (value < 0) ? 1 : 0;
    long long temp = value = value < 0 ? -value : value;
    xSize len = 0;

    // calculate the number of digits needed for the value
    while (temp) {
        temp /= 10;
        len++;
    }

    // allocate memory for the string data
    xString_preallocate(ret, len + sign);
    if (ret->capacity < len + sign) {
        // failed to allocate data block, return NULL
        xString_free(ret);
        return NULL;
    }

    // add sign to the string
    if (sign) {
        ret->data[0] = '-';
        ret->length++;
        len++;  // offset because of added sign character
    }

    // convert the value to string
    for (; value; ret->length++) {
        ret->data[len - ret->length + sign - 1] = value % 10 + '0';
        value /= 10;
    }

    return ret;
}

xString *xString_fromFloat(float value)
{
    // check if the value is NaN or infinity
    if (isnan_f(value)) {
        return xString_fromCString("NaN");
    } else if (isinf_f(value)) {
        return xString_fromCString((value < 0) ? "-Inf" : "Inf");
    }

    // create a new string
    xString *ret = xString_new();
    if (!ret->refCount) {
        return ret;
    }

    // determine sign of the value
    int sign = (value < 0) ? -1 : 1;
    value = (value < 0) ? -value : value;

    // convert integer part of the value to string
    xString *intPart = xString_fromLong((long)value);
    if (!xString_isValid(intPart)) {
        xString_free(ret);
        return NULL;
    }

    // convert fractional part of the value to string
    xString *fracPart = xString_new();
    if (!xString_isValid(fracPart)) {
        xString_free(intPart);
        xString_free(ret);
        return NULL;
    }

    // calculate the length of the fractional part
    float frac = value - (long)value;
    xSize len = 0;
    while (frac && len < 6) {
        frac *= 10;
        fracPart = xString_append(fracPart, (xChar[]){(xChar)frac + '0'}, 1);
        frac -= (int)frac;
        len++;
    }

    // concatenate the integer and fractional parts
    ret = xString_append(intPart, ".", 1);
    ret = xString_append(ret, fracPart->data, fracPart->length);

    // add sign to the string
    if (sign < 0) {
        ret = xString_insert(ret, "-", 1, 0);
    }

    xString_free(intPart);
    xString_free(fracPart);

    return ret;
}

xString *xString_fromDouble(double value)
{
    // check if the value is NaN or infinity
    if (isnan_d(value)) {
        return xString_fromCString("NaN");
    } else if (isinf_d(value)) {
        return xString_fromCString((value < 0) ? "-Inf" : "Inf");
    }

    // create a new string
    xString *ret = xString_new();
    if (!xString_isValid(ret)) {
        return NULL;
    }

    // determine sign of the value
    int sign = (value < 0) ? -1 : 1;
    value = (value < 0) ? -value : value;

    // convert integer part of the value to string
    xString *intPart = xString_fromLong((long)value);
    if (!xString_isValid(intPart)) {
        xString_free(intPart);
        return NULL;
    }

    // convert fractional part of the value to string
    xString *fracPart = xString_new();
    if (!xString_isValid(fracPart)) {
        xString_free(intPart);
        xString_free(ret);
        return NULL;
    }

    // calculate the length of the fractional part
    double frac = value - (long)value;
    xSize len = 0;
    while (frac && len < 15) {
        frac *= 10;
        fracPart = xString_append(fracPart, (xChar[]){(xChar)frac + '0'}, 1);
        frac -= (int)frac;
        len++;
    }

    // concatenate the integer and fractional parts
    ret = xString_append(intPart, ".", 1);
    ret = xString_append(ret, fracPart->data, fracPart->length);

    // add sign to the string
    if (sign < 0) {
        ret = xString_insert(ret, "-", 1, 0);
    }

    xString_free(intPart);
    xString_free(fracPart);

    return ret;
}

xUInt64 xString_hash(const xString *str)
{
    // delegate call to xMemHash
    return xString_isValid(str) ? xMemHash((const void *)str->data, str->length) : 0;
}
