#include "xStructures/xDictionary.h"
#include <stdlib.h>  // standard library (for malloc, free, realloc)
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

typedef struct xKeyValuePair_s {
    xUInt64 hash;  // hash of the key
    void *key;     // pointer to the key
    void *value;   // pointer to the value
} xKeyValuePair;

struct xDictionary_s {
    xKeyValuePair *entries;                    // array of key-value pairs (ascending order by key)
    xSize dictSize;                            // number of entries
    xSize dictCapacity;                        // number of entries that can be stored
    xSize keySize;                             // size of the key in bytes
    xSize valueSize;                           // size of the value in bytes
    xUInt64 (*hashFunction)(const void *key);  // hash function for the key
};

xDictionary xDictionary_new(xSize keySize, xSize valueSize, xUInt64 (*hashFunction)(const void *key))
{
    if (!keySize || !valueSize || !hashFunction) {
        return (xDictionary){0};
    }

    xDictionary dict = {0};
    dict.keySize = keySize;
    dict.valueSize = valueSize;
    dict.hashFunction = hashFunction;
    return dict;
}

void xDictionary_free(xDictionary *dict)
{
    if (!dict || !dict->entries) {
        return;
    }

    free(dict->entries);
    dict->entries = NULL;
    dict->dictSize = 0;
    dict->dictCapacity = 0;
}

inline xSize xDictionary_getSize(const xDictionary *dict) { return (dict) ? dict->dictSize : 0; }

inline xSize xDictionary_getCapacity(const xDictionary *dict) { return (dict) ? dict->dictCapacity : 0; }

inline xSize xDictionary_getKeySize(const xDictionary *dict) { return (dict) ? dict->keySize : 0; }

inline xSize xDictionary_getValueSize(const xDictionary *dict) { return (dict) ? dict->valueSize : 0; }

inline xBool xDictionary_isValid(const xDictionary *dict) { return (dict && dict->entries != NULL) ? true : false; }

void xDictionary_insert(xDictionary *dict, const void *key, const void *value)
{
    // validate arguments
    if (!dict || !key || !value) {
        return;
    }

    // allocate memory for new entry
    if (dict->dictSize == dict->dictCapacity) {
        dict->dictCapacity = dict->dictCapacity ? dict->dictCapacity * 2 : 1;
        xKeyValuePair *newEntries = realloc(dict->entries, dict->dictCapacity * sizeof(xKeyValuePair));
        if (!newEntries) {
            dict->dictCapacity /= 2;  // revert capacity
            return;
        }
        dict->entries = newEntries;
    }

    // calculate hash and binary search for insertion
    xUInt64 hash = dict->hashFunction(key);
    xSize left = 0;
    xSize right = dict->dictSize;
    while (left < right) {
        xSize mid = left + (right - left) / 2;
        if (dict->entries[mid].hash < hash) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // shift elements to the right
    for (xSize i = dict->dictSize; i > left; --i) {
        dict->entries[i] = dict->entries[i - 1];
    }

    // insert new entry
    dict->entries[left].hash = hash;
    dict->entries[left].key = (void *)key;
    dict->entries[left].value = (void *)value;
    dict->dictSize++;
}

void *xDictionary_get(const xDictionary *dict, const void *key)
{
    // validate arguments
    if (!dict || !key || !dict->entries) {
        return NULL;
    }

    // calculate hash and binary search for key
    xUInt64 hash = dict->hashFunction(key);
    xSize left = 0;
    xSize right = dict->dictSize;
    while (left < right) {
        xSize mid = left + (right - left) / 2;
        if (dict->entries[mid].hash < hash) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // return value if key found
    return (left < dict->dictSize && dict->entries[left].hash == hash) ? dict->entries[left].value : NULL;
}

void *xDictionary_remove(xDictionary *dict, const void *key)
{
    // validate arguments
    if (!dict || !key || !dict->entries) {
        return NULL;
    }

    // calculate hash and binary search for key
    xUInt64 hash = dict->hashFunction(key);
    xSize left = 0;
    xSize right = dict->dictSize;
    while (left < right) {
        xSize mid = left + (right - left) / 2;
        if (dict->entries[mid].hash < hash) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // return value if key found
    if (left < dict->dictSize && dict->entries[left].hash == hash) {
        void *value = dict->entries[left].value;
        for (xSize i = left; i < dict->dictSize - 1; ++i) {
            dict->entries[i] = dict->entries[i + 1];
        }
        dict->dictSize--;
        return value;
    }

    return NULL;
}

xBool xDictionary_contains(const xDictionary *dict, const void *key)
{
    // validate arguments
    if (!dict || !key || !dict->entries) {
        return false;
    }

    // calculate hash and binary search for key
    xUInt64 hash = dict->hashFunction(key);
    xSize left = 0;
    xSize right = dict->dictSize;
    while (left < right) {
        xSize mid = left + (right - left) / 2;
        if (dict->entries[mid].hash < hash) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // return non-zero if key found
    return (left < dict->dictSize && dict->entries[left].hash == hash) ? true : false;
}
