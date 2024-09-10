/**
 * @file xDictionary.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Dictionary implementation in xStructures module.
 * @version 0.10
 * @date 25.07.2024.
 *
 * Module declares dictionary structure and functions for managing it. All functions have prefix `xDictionary_`.
 * Key hashing function should be implemented manually depending on the key type.
 */

#ifndef XSTRUCTURES_DICTIONARY_H
#define XSTRUCTURES_DICTIONARY_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Dictionary structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xDictionary object.
 *
 * @warning
 * Even if structure itself is on stack, it should be properly freed using xDictionary_free() function to free internal data.
 */
typedef struct xDictionary_s xDictionary;

/**
 * @brief
 * Creates empty xDictionary object.
 *
 * @param keySize Size of the key in bytes.
 * @param valueSize Size of the value in bytes.
 * @param hashFunction Hash function for the key.
 * @return xDictionary object with no data.
 */
xDictionary xDictionary_new(xSize keySize, xSize valueSize, xUInt64 (*hashFunction)(const void *key));

/**
 * @brief
 * Free xDictionary object and its data from memory.
 *
 * @param dict Pointer to xDictionary object to free.
 *
 * @note
 * Once xDictionary object is freed, it is invalidated and no longer usable with other functions.
 *
 * @warning
 * User is responsible for freeing individual keys and values if they are dynamically allocated.
 */
void xDictionary_free(xDictionary *dict);

/**
 * @brief
 * Get number of elements in dictionary.
 *
 * @param dict Pointer to xDictionary object.
 * @return xSize Number of elements in dictionary.
 */
inline xSize xDictionary_getSize(const xDictionary *dict);

/**
 * @brief
 * Get allocated capacity of dictionary.
 *
 * @param dict Pointer to xDictionary object.
 * @return xSize Capacity of dictionary in number of elements.
 */
inline xSize xDictionary_getCapacity(const xDictionary *dict);

/**
 * @brief
 * Get size of the key in bytes.
 *
 * @param dict Pointer to xDictionary object.
 * @return xSize Size of the key in bytes.
 */
inline xSize xDictionary_getKeySize(const xDictionary *dict);

/**
 * @brief
 * Get size of the value in bytes.
 *
 * @param dict Pointer to xDictionary object.
 * @return xSize Size of the value in bytes.
 */
inline xSize xDictionary_getValueSize(const xDictionary *dict);

/**
 * @brief
 * Check if dictionary is valid.
 *
 * @param dict Pointer to xDictionary object.
 * @return xBool Non-zero if dictionary is valid, zero otherwise.
 */
inline xBool xDictionary_isValid(const xDictionary *dict);

/**
 * @brief
 * Insert key-value pair into dictionary.
 *
 * @param dict Pointer to xDictionary object where key-value pair will be inserted.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 *
 * @note
 * If key already exists in dictionary, value will be replaced.
 *
 * @warning
 * It is important to respect key-value sizes defined when creating dictionary to avoid memory corruption.
 */
void xDictionary_insert(xDictionary *dict, const void *key, const void *value);

/**
 * @brief
 * Get value from dictionary by key.
 *
 * @param dict Pointer to xDictionary object.
 * @param key Pointer to the key.
 * @return void* Pointer to the value or NULL if key does not exist.
 *
 * @warning
 * It is important to respect key type defined when creating dictionary to calculate hash correctly.
 */
void *xDictionary_get(const xDictionary *dict, const void *key);

/**
 * @brief
 * Remove key-value pair from dictionary by key.
 *
 * @param dict Pointer to xDictionary object.
 * @param key Pointer to the key.
 * @return void* Pointer to the value or NULL if key does not exist.
 *
 * @warning
 * It is important to respect key type defined when creating dictionary to calculate hash correctly.
 *
 * @note
 * It is important to free key and value if they are dynamically allocated.
 */
void *xDictionary_remove(xDictionary *dict, const void *key);

/**
 * @brief
 * Check if key exists in dictionary.
 *
 * @param dict Pointer to xDictionary object.
 * @param key Pointer to the key.
 * @return int Non-zero if key exists, zero otherwise.
 *
 * @warning
 * It is important to respect key type defined when creating dictionary to calculate hash correctly.
 */
xBool xDictionary_contains(const xDictionary *dict, const void *key);

#ifdef __cplusplus
}
#endif

#endif  // XSTRUCTURES_DICTIONARY_H
