/**
 * @file xLimits.h
 * @author 0xDontCare (you@domain.com)
 * @brief Limit definitions for xcFramework types.
 * @version 0.1
 * @date 05.08.2024.
 *
 * Header is used along with xTypes.h to define limits for custom types defined there.
 */

#ifndef XBASE_LIMITS_H
#define XBASE_LIMITS_H

#define XINT8_MIN 0x80
#define XINT8_MAX 0x7F

#define XUINT8_MIN 0x00
#define XUINT8_MAX 0xFF

#define XINT16_MIN 0x8000
#define XINT16_MAX 0x7FFF

#define XUINT16_MIN 0x0000
#define XUINT16_MAX 0xFFFF

#define XINT32_MIN 0x80000000
#define XINT32_MAX 0x7FFFFFFF

#define XUINT32_MIN 0x00000000U
#define XUINT32_MAX 0xFFFFFFFFU

#define XINT64_MIN 0x8000000000000000LL
#define XINT64_MAX 0x7FFFFFFFFFFFFFFFLL

#define XUINT64_MIN 0x0000000000000000ULL
#define XUINT64_MAX 0xFFFFFFFFFFFFFFFFULL

#define XFLOAT_MIN ((float)0x00800000)
#define XFLOAT_MAX ((float)0x7F7FFFFF)

#define XDOUBLE_MIN ((double)0x0010000000000000)
#define XDOUBLE_MAX ((double)0x7FEFFFFFFFFFFFFF)

#endif  // XBASE_LIMITS_H
