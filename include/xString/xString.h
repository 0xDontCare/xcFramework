/**
 * @file xString.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Safe string library module for xcFramework (and effectively successor to xStringWorks module).
 * @version 0.20
 * @date 13.07.2024.
 *
 * Library implements safer string type and respective functions for C. Version 0.20 introduces significant changes and improvements
 * from previous versions like copy-on-write, better memory management, and more. All functions have prefix `xString_`.
 */

#ifndef XSTRING_BASE_H
#define XSTRING_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xBase/xTypes.h"

/**
 * @brief
 * Calculate length of null-terminated C string.
 *
 * @param cstr Pointer to C string.
 * @return Length of C string.
 */
xSize cstrlen(const xChar *cstr);

/**
 * @brief
 * String type introduced by xcFramework.
 *
 * @warning
 * Even if structure is stack-allocated, it is recommended to free it with xString_free function due to internal pointers.
 */
typedef struct xString_s xString;

/**
 * @brief
 * Creates new blank xString object.
 *
 * @note
 * Initially string capacity is 0, length is 0, and data is NULL. Only reference counter is allocated and initialized.
 *
 * @note
 * If reference counter is not allocated successfully, its pointer will also be set to NULL.
 *
 * @return
 * xString object with no data.
 */
xString *xString_new(void);

/**
 * @brief
 * Frees xString heap elements of xString object.
 *
 * @note
 * Internal dangling pointers are set to NULL.
 *
 * @note
 * Depending on reference count, function may not free memory if string data is shared.
 *
 * @note
 * If xString object contains no data or is already freed, function will return without any action.
 *
 * @param str Pointer to xString object to be freed.
 */
void xString_free(xString *str);

/**
 * @brief
 * Get length of xString object data.
 *
 * @param str Pointer to xString object.
 * @return xSize value of xString object data length.
 */
extern xSize xString_getLength(const xString *str);

/**
 * @brief
 * Get allocated size of xString object data.
 *
 * @param str Pointer to xString object.
 * @return xSize value of xString object data capacity.
 */
extern xSize xString_getCapacity(const xString *str);

/**
 * @brief
 * Get pointer to text data contained in xString object.
 *
 * @param str Pointer to xString object.
 * @return const xChar* pointer to xString object data.
 *
 * @note
 * If xString object is empty or not allocated, function will return NULL.
 *
 * @note
 * Returned pointer is const, so it is not recommended to modify data directly, especially if string contains data mutual to other
 * xString objects.
 */
extern const xChar *xString_getData(const xString *str);

/**
 * @brief
 * Check if xString object is valid (usable for other functions from framework).
 *
 * @param str Pointer to xString object to check.
 * @return xBool Non-zero if object is valid, zero otherwise.
 */
extern xBool xString_isValid(const xString *str);

/**
 * @brief
 * Optimizes xString object by removing unused memory.
 *
 * @param str Pointer to xString object to be optimized.
 *
 * @note
 * If function fails (due to memory allocation fail), string will not be
 * optimized and will be left in larger block of memory.
 */
void xString_optimize(xString *str);

/**
 * @brief
 * Expand target string capacity at least by given size.
 *
 * @param str Pointer to xString object to be preallocated.
 * @param size Size to be added to xString object capacity.
 *
 * @note
 * If new buffer allocation fails, old buffer will be left untouched.
 *
 * @note
 * If xString object is not allocated, new memory block will be allocated with given size.
 */
void xString_preallocate(xString *str, xSize size);

/**
 * @brief
 * Clear content of xString object.
 *
 * @param str Pointer to xString object to be cleared.
 *
 * @note
 * If pointer to xString object is invalid or string is empty, funciton will perform no action.
 */
void xString_clear(xString *str);

/**
 * @brief
 * Create copy of string with shared reference to data
 *
 * @param str Pointer to target string object
 * @return xString object containing copy of original
 */
xString *xString_copy(const xString *str);

/**
 * @brief
 * Create copy of xString object with detached reference to data (allocates additional memory)
 *
 * @param str Pointer to target string object
 * @return xString object containing detached copy of original
 */
xString *xString_copyDetached(const xString *str);

/**
 * @brief
 * Append data block of some size to xString object. Currently the main way of putting data into xString object.
 *
 * @param str Modified xString object.
 * @param data Appended data block location.
 * @param len Block size.
 *
 * @note
 * If string data is unallocated or not large enough, new memory block will be automatically allocated.
 *
 * @note
 * Due to way memory is extended to new size (by doubling old capacity), allocated memory block may be larger than actual string
 * data length. In this case, it would be suggested to optimize string if needed.
 *
 * @note
 * If appended data pointer is NULL or length value is invalid (0 or negative), xString object will be left untouched.
 */
xString *xString_append(const xString *str, const xChar *data, xSize len);

/**
 * @brief
 * Create substring of xString object.
 *
 * @param str Input xString object.
 * @param start Starting index.
 * @param end Ending index.
 * @return Pointer to xString object containing selected substring of input xString object.
 *
 * @note
 * Selected interval includes characters on starting index and excludes one on the ending index.
 */
xString *xString_substring(const xString *str, xSize start, xSize end);

/**
 * @brief
 * Finds first match of data block in xString object.
 *
 * @param str Input xString object.
 * @param data Pointer to data block.
 * @param len Data block size.
 * @return Index of first occurence of data block in xString data, XSIZE_MAX if no
 * matches found or invalid arguments given.
 *
 * @note
 * If function fails anywhere internally, it will return XSIZE_MAX.
 */
xSize xString_find(const xString *str, const xChar *data, xSize len);

/**
 * @brief
 * Find last occurence of matching data block in xString object.
 *
 * @param str Pointer to xString object.
 * @param data Pointer to data block.
 * @param len Data block size.
 * @return Index of last occurence of data block in xString data, XSIZE_MAX if no matches found or invalid arguments given.
 */
xSize xString_findLast(const xString *str, const xChar *data, xSize len);

/**
 * @brief
 * Count number of occurences of pattern in xString object.
 *
 * @param haystack Pointer to xString object to serach in.
 * @param needle Pattern to search for.
 * @param len Length of pattern.
 * @return Number of pattern matches in xString object.
 */
xSize xString_count(const xString *haystack, const xChar *needle, xSize len);

/**
 * @brief
 * Count number of overlapping occurences of pattern in xString object.
 *
 * @param haystack Pointer to xString object to serach in.
 * @param needle Pattern to search for.
 * @param len Length of pattern.
 * @return Number of overlapping pattern matches in xString object.
 */
xSize xString_count_overlapping(const xString *haystack, const xChar *needle, xSize len);

/**
 * @brief
 * Replace first occurence of pattern in xString object.
 *
 * @param str Pointer to xString object to search in.
 * @param needle Pattern to search for.
 * @param needleLen Length of pattern.
 * @param replacement Replacement pattern.
 * @param replacementLen Length of replacement pattern.
 * @return xString object with replaced pattern.
 */
xString *xString_replaceFirst(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                              xSize replacementLen);

/**
 * @brief
 * Replace last occurence of pattern in xString object.
 *
 * @param str Pointer to xString object to search in.
 * @param needle Pattern to search for.
 * @param needleLen Length of pattern.
 * @param replacement Replacement pattern.
 * @param replacementLen Length of replacement pattern.
 * @return xString object with replaced pattern.
 */
xString *xString_replaceLast(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                             xSize replacementLen);

/**
 * @brief
 * Replace all non-overlapping pattern matches in xString object.
 *
 * @param str Pointer to xString object to search in.
 * @param needle Pattern to search for.
 * @param needleLen Length of pattern.
 * @param replacement Replacement pattern.
 * @param replacementLen Length of replacement pattern.
 * @return xString object with replaced pattern.
 */
xString *xString_replaceAll(const xString *str, const xChar *needle, xSize needleLen, const xChar *replacement,
                            xSize replacementLen);

/**
 * @brief
 * Remove content of xString object in given index interval.
 *
 * @param str Pointer to xString object to modify.
 * @param start Starting index.
 * @param end Ending index.
 * @return xString object with removed content.
 */
xString *xString_remove(const xString *str, xSize start, xSize end);

/**
 * @brief
 * Insert data block into xString object at given index.
 *
 * @param str Pointer to xString object to modify.
 * @param data Data block to insert.
 * @param len Length of data block.
 * @param index Index to insert data block at.
 * @return xString object with inserted data block.
 */
xString *xString_insert(const xString *str, const xChar *data, xSize len, xSize index);

/**
 * @brief
 * Split xString object into array of xString objects by delimiter.
 *
 * @param str Pointer to xString object to split.
 * @param delimiter Delimiter string.
 * @param delimiterLen Length of delimiter string.
 * @param count Pointer to variable to store number of split parts.
 * @return Array of xString objects.
 */
xString *xString_split(const xString *str, const xChar *delimiter, xSize delimiterLen, xSize *count);

/**
 * @brief
 * Lexicographically compare two xString objects. (case-sensitive)
 *
 * @param str1 First xString object.
 * @param str2 Second xString object.
 * @return 0 if strings are equal, negative if first string precedes second, positive if first string follows second.
 */
int xString_compare(const xString *str1, const xString *str2);

/**
 * @brief
 * Lexicographically compare two xString objects. (case-insensitive)
 *
 * @param str1 First xString object.
 * @param str2 Second xString object.
 * @return 0 if strings are equal, negative if first string precedes second, positive if first string follows second.
 */
int xString_compareIgnoreCase(const xString *str1, const xString *str2);

/**
 * @brief
 * Convert xString object to null-terminated C string.
 *
 * @param str Pointer to xString object.
 * @return Pointer to null-terminated C string.
 */
xChar *xString_toCString(const xString *str);

/**
 * @brief
 * Convert xString object to integer.
 *
 * @param str Pointer to xString object.
 * @return Integer value of xString object.
 */
int xString_toInt(const xString *str);

/**
 * @brief
 * Convert xString object to long integer.
 *
 * @param str Pointer to xString object.
 * @return Long integer value of xString object.
 */
long long xString_toLong(const xString *str);

/**
 * @brief
 * Convert xString object to float.
 *
 * @param str Pointer to xString object.
 * @return Float value of xString object.
 */
float xString_toFloat(const xString *str);

/**
 * @brief
 * Convert xString object to double.
 *
 * @param str Pointer to xString object.
 * @return Double value of xString object.
 */
double xString_toDouble(const xString *str);

/**
 * @brief
 * Create xString object from C string.
 *
 * @param cstr C string.
 * @return xString object containing C string.
 */
xString *xString_fromCString(const xChar *cstr);

/**
 * @brief
 * Create xString object from C string with given length.
 *
 * @param cstr C string.
 * @param len Length of C string.
 * @return xString object containing C string.
 */
xString *xString_fromCStringS(const xChar *cstr, xSize len);

/**
 * @brief
 * Create xString object from integer.
 *
 * @param value Integer value.
 * @return xString object containing integer value.
 */
xString *xString_fromInt(int value);

/**
 * @brief
 * Create xString object from long integer.
 *
 * @param value Long integer value.
 * @return xString object containing long integer value.
 */
xString *xString_fromLong(long long value);

/**
 * @brief
 * Create xString object from float.
 *
 * @param value Float value.
 * @return xString object containing float value.
 */
xString *xString_fromFloat(float value);

/**
 * @brief
 * Create xString object from double.
 *
 * @param value Double value.
 * @return xString object containing double value.
 */
xString *xString_fromDouble(double value);

/**
 * @brief
 * Calculate hash value of xString object.
 *
 * @param str Pointer to xString object.
 * @return Hash value of xString object.
 */
xUInt64 xString_hash(const xString *str);

#ifdef __cplusplus
}
#endif

#endif  // XSTRING_BASE_H
