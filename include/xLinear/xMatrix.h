/**
 * @file xMatrix.h
 * @author 0xDontCare (https://github.com/0xDontCare)
 * @brief Matrix structure and functions.
 * @version 0.10
 * @date 26.07.2024.
 *
 * Module declares matrix structure and mathematical operations applicable to them. All functions have prefix `xMatrix_`.
 */

#ifndef XLINEAR_MATRIX_H
#define XLINEAR_MATRIX_H

#include "xBase/xTypes.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @brief
 * Matrix structure introduced by xcFramework.
 *
 * @note
 * Do not access structure members directly. Use provided functions for managing xMatrix object.
 *
 * @warning
 * Even if structure itself is on stack, it should be properly freed using xMatrix_free() function to free internal data.
 */
typedef struct xMatrix_s xMatrix;

/**
 * @brief
 * Creates empty xMatrix object of given size.
 *
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return xMatrix object filled with zeros.
 */
xMatrix *xMatrix_new(xSize rows, xSize cols);

/**
 * @brief
 * Free xMatrix object and its data from memory.
 *
 * @param matrix Pointer to xMatrix object to free.
 *
 * @note
 * Once xMatrix object is freed, it is invalidated and no longer usable with other functions.
 */
void xMatrix_free(xMatrix *matrix);

/**
 * @brief
 * Get number of rows in the matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @return xSize Number of rows.
 */
extern xSize xMatrix_getRows(const xMatrix *matrix);

/**
 * @brief
 * Get number of columns in the matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @return xSize Number of columns.
 */
extern xSize xMatrix_getCols(const xMatrix *matrix);

/**
 * @brief
 * Check if matrix is valid.
 *
 * @param matrix Pointer to xMatrix object.
 * @return int Non-zero if matrix is valid, zero otherwise.
 */
extern xBool xMatrix_isValid(const xMatrix *matrix);

/**
 * @brief
 * Set value at given row and column.
 *
 * @param matrix Pointer to xMatrix object.
 * @param row Index of the row.
 * @param col Index of the column.
 * @param value Value to set.
 */
extern void xMatrix_set(xMatrix *matrix, xSize row, xSize col, float value);

/**
 * @brief
 * Get value at given row and column.
 *
 * @param matrix Pointer to xMatrix object.
 * @param row Index of the row.
 * @param col Index of the column.
 * @return float Value at given row and column.
 *
 * @note
 * If matrix is invalid or indices are out of range, NaN is returned.
 */
extern float xMatrix_get(const xMatrix *matrix, xSize row, xSize col);

/**
 * @brief
 * Create copy of matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @return xMatrix Copy of matrix.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 *
 * @note
 * Copy of matrix is deep copy, meaning that new memory is allocated for data.
 */
xMatrix *xMatrix_duplicate(const xMatrix *matrix);

/**
 * @brief
 * Fill matrix with given value.
 *
 * @param matrix Pointer to xMatrix object.
 * @param value Value to fill matrix with.
 */
void xMatrix_fill(xMatrix *matrix, float value);

/**
 * @brief
 * Create identity matrix of given dimension.
 *
 * @param dimension Dimension of the identity matrix.
 * @return xMatrix Identity matrix.
 *
 * @note
 * If dimension is zero or allocation fails, invalid matrix is returned.
 */
xMatrix *xMatrix_identity(xSize dimension);

/**
 * @brief
 * Transpose matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @return xMatrix Transposed matrix.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 */
xMatrix *xMatrix_transpose(const xMatrix *matrix);

/**
 * @brief
 * Add two matrices.
 *
 * @param lhs Left-hand side matrix.
 * @param rhs Right-hand side matrix.
 * @return xMatrix Sum of two matrices.
 *
 * @note
 * If matrices are invalid or have different dimensions, invalid matrix is returned.
 */
xMatrix *xMatrix_add(const xMatrix *lhs, const xMatrix *rhs);

/**
 * @brief
 * Subtract two matrices.
 *
 * @param lhs Left-hand side matrix.
 * @param rhs Right-hand side matrix.
 * @return xMatrix Difference of two matrices.
 *
 * @note
 * If matrices are invalid or have different dimensions, invalid matrix is returned.
 */
xMatrix *xMatrix_sub(const xMatrix *lhs, const xMatrix *rhs);

/**
 * @brief
 * Multiply two matrices.
 *
 * @param lhs Left-hand side matrix.
 * @param rhs Right-hand side matrix.
 * @return xMatrix Product of two matrices.
 *
 * @note
 * If matrices are invalid or have incompatible dimensions, invalid matrix is returned.
 */
xMatrix *xMatrix_mul(const xMatrix *lhs, const xMatrix *rhs);

/**
 * @brief
 * Calculate Hadamard product of two matrices.
 *
 * @param lhs Pointer to left-hand side matrix.
 * @param rhs Pointer to right-hand side matrix.
 * @return xMatrix Hadamard product of two matrices.
 *
 * @note
 * If matrices are invalid or have different dimensions, invalid matrix is returned.
 *
 * @note
 * Hadamard product is element-wise multiplication of two matrices.
 */
xMatrix *xMatrix_dotmul(const xMatrix *lhs, const xMatrix *rhs);

/**
 * @brief
 * Add scalar to matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @param scalar Scalar value.
 * @return xMatrix Sum of matrix and scalar.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 */
xMatrix *xMatrix_scalarAdd(const xMatrix *matrix, float scalar);

/**
 * @brief
 * Subtract scalar from matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @param scalar Scalar value.
 * @return xMatrix Difference of matrix and scalar.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 */
xMatrix *xMatrix_scalarSub(const xMatrix *matrix, float scalar);

/**
 * @brief
 * Multiply matrix by scalar.
 *
 * @param matrix Pointer to xMatrix object.
 * @param scalar Scalar value.
 * @return xMatrix Product of matrix and scalar.
 */
xMatrix *xMatrix_scalarMul(const xMatrix *matrix, float scalar);

/**
 * @brief
 * Divide matrix by scalar.
 *
 * @param matrix Pointer to xMatrix object.
 * @param scalar Scalar value.
 * @return xMatrix Quotient of matrix and scalar.
 */
xMatrix *xMatrix_scalarDiv(const xMatrix *matrix, float scalar);

/**
 * @brief
 * Raise matrix to given power.
 *
 * @param matrix Pointer to xMatrix object.
 * @param power Power to raise matrix to.
 * @return xMatrix Matrix raised to given power.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 */
// xMatrix xMatrix_pow(const xMatrix *matrix, float power);

/**
 * @brief
 * Get submatrix of matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @param row1 Lower bound of row.
 * @param col1 Lower bound of column.
 * @param row2 Upper bound of row.
 * @param col2 Upper bound of column.
 * @return xMatrix Submatrix of matrix.
 *
 * @note
 * If matrix is invalid or bounds are out of range, invalid matrix is returned.
 *
 * @note
 * Lower bound is inclusive, upper bound is exclusive.
 */
xMatrix *xMatrix_submatrix(const xMatrix *matrix, xSize row1, xSize col1, xSize row2, xSize col2);

/**
 * @brief
 * Get minor of matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @param row Row to exclude from matrix.
 * @param col Column to exclude from matrix.
 * @return xMatrix Minor of matrix.
 *
 * @note
 * If matrix is invalid or indices are out of range, invalid matrix is returned.
 */
xMatrix *xMatrix_minor(const xMatrix *matrix, xSize row, xSize col);

/**
 * @brief
 * Apply function to each element of matrix.
 *
 * @param matrix Pointer to xMatrix object.
 * @param func Function to apply to each element.
 * @return xMatrix Matrix with function applied to each element.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 *
 * @note
 * Function should take float as argument and return float.
 */
xMatrix *xMatrix_map(const xMatrix *matrix, float (*func)(float));

/**
 * @brief
 * Apply function to each element of two matrices.
 *
 * @param lhs Left-hand side matrix.
 * @param rhs Right-hand side matrix.
 * @param func Function to apply to each element.
 * @return xMatrix Matrix with function applied to each element.
 *
 * @note
 * If matrices are invalid or have different dimensions, invalid matrix is returned.
 *
 * @note
 * Function should take two floats as arguments and return float.
 */
xMatrix *xMatrix_map2(const xMatrix *lhs, const xMatrix *rhs, float (*func)(float, float));

/**
 * @brief
 * Apply function to each element of matrix and scalar.
 *
 * @param matrix Pointer to xMatrix object.
 * @param scalar Scalar value.
 * @param func Function to apply to each element.
 * @return xMatrix Matrix with function applied to each element.
 *
 * @note
 * If matrix is invalid, invalid matrix is returned.
 *
 * @note
 * Function should take two floats as arguments and return float.
 */
xMatrix *xMatrix_mapScalar(const xMatrix *matrix, float scalar, float (*func)(float, float));

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // XLINEAR_MATRIX_H
