/**
 * @file xTypes.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Custom types used in xcFramework modules.
 * @version 0.1
 * @date 05.08.2024.
 *
 * Module defines custom types used in xcFramework modules.
 */

#ifndef XBASE_TYPES_H
#define XBASE_TYPES_H

#include "xBase/xLimits.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief Boolean type.
 */
typedef enum {
    false = 0, /**< False value. */
    true       /**< True value. */
} xBool;

/**
 * @brief 8-bit signed integer.
 */
typedef signed char xInt8;

/**
 * @brief 8-bit unsigned integer.
 */
typedef unsigned char xUInt8;

/**
 * @brief 16-bit signed integer.
 */
typedef signed short xInt16;

/**
 * @brief 16-bit unsigned integer.
 */
typedef unsigned short xUInt16;

/**
 * @brief 32-bit signed integer.
 */
typedef signed int xInt32;

/**
 * @brief 32-bit unsigned integer.
 */
typedef unsigned int xUInt32;

/**
 * @brief 64-bit signed integer.
 */
typedef signed long long xInt64;

/**
 * @brief 64-bit unsigned integer.
 */
typedef unsigned long long xUInt64;

/**
 * @brief 32-bit floating point number.
 */
typedef float xFloat32;

/**
 * @brief 64-bit floating point number.
 */
typedef double xFloat64;

/**
 * @brief 8-bit character.
 */
typedef char xChar;

/**
 * @brief 16-bit character.
 */
typedef short xChar16;

/**
 * @brief Size type.
 */
#if defined(_WIN64) || defined(__LP64__) || defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__)
// 64-bit platform
typedef xUInt64 xSize;
#define XSIZE_MAX XUINT64_MAX
#define XSIZE_MIN XUINT64_MIN
#else
// fallback to 32-bit platform
typedef xUInt32 xSize;
#define XSIZE_MAX XUINT32_MAX
#define XSIZE_MIN XUINT32_MIN
#endif

/**
 * @brief Offset type.
 */
#if defined(_WIN64) || defined(__LP64__) || defined(__x86_64__) || defined(__aarch64__) || defined(__ppc64__)
// 64-bit platform
typedef xInt64 xOffset;
#define XOFFSET_MAX XINT64_MAX
#define XOFFSET_MIN XINT64_MIN
#else
// fallback to 32-bit platform
typedef xInt32 xOffset;
#define XOFFSET_MAX XINT32_MAX
#define XOFFSET_MIN XINT32_MIN
#endif

/**
 * @brief Null pointer.
 */
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // XBASE_TYPES_H
