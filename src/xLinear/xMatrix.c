#include "xLinear/xMatrix.h"
#include <stdlib.h>  // standard library (for malloc, calloc, free)
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xMatrix_s {
    float *data;  // contiguous array of all matrix elements
    xSize rows;   // number of rows
    xSize cols;   // number of columns
};

xMatrix xMatrix_new(xSize rows, xSize cols)
{
    // validate arguments
    if (!rows || !cols) {
        return (xMatrix){0};
    }

    // create matrix object
    xMatrix mat = {0};
    mat.data = (float *)calloc(rows * cols, sizeof(float));
    if (!mat.data) {
        return mat;
    }
    mat.rows = rows;
    mat.cols = cols;

    return mat;
}

void xMatrix_free(xMatrix *matrix)
{
    if (!matrix || !matrix->data) {
        return;
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
}

inline xSize xMatrix_getRows(const xMatrix *matrix) { return (matrix) ? matrix->rows : 0; }

inline xSize xMatrix_getCols(const xMatrix *matrix) { return (matrix) ? matrix->cols : 0; }

inline int xMatrix_isValid(const xMatrix *matrix) { return (matrix) ? matrix->data && matrix->rows && matrix->cols : 0; }

inline void xMatrix_set(xMatrix *matrix, xSize row, xSize col, float value)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row >= matrix->rows || col >= matrix->cols) {
        return;
    }

    // set matrix element
    matrix->data[row * matrix->cols + col] = value;
}

inline float xMatrix_get(const xMatrix *matrix, xSize row, xSize col)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row >= matrix->rows || col >= matrix->cols) {
        return (float)0x7FC00000;  // quiet NaN
    }

    // get matrix element
    return matrix->data[row * matrix->cols + col];
}

xMatrix xMatrix_duplicate(const xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create matrix object
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // copy matrix data
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(matrix, i, j));
        }
    }

    return mat;
}

void xMatrix_fill(xMatrix *matrix, float value)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return;
    }

    // fill matrix with value
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(matrix, i, j, value);
        }
    }
}

xMatrix xMatrix_identity(xSize dimension)
{
    // validate arguments
    if (!dimension) {
        return (xMatrix){0};
    }

    // create identity matrix
    xMatrix mat = xMatrix_new(dimension, dimension);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // assign values to diagonal elements
    for (xSize i = 0; i < dimension; i++) {
        xMatrix_set(&mat, i, i, 1.0f);
    }

    return mat;
}

xMatrix xMatrix_transpose(const xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create transposed matrix
    xMatrix mat = xMatrix_new(matrix->cols, matrix->rows);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // transpose matrix
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, j, i, xMatrix_get(matrix, i, j));
        }
    }

    return mat;
}

xMatrix xMatrix_add(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // add matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(lhs, i, j) + xMatrix_get(rhs, i, j));
        }
    }

    return mat;
}

xMatrix xMatrix_sub(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // subtract matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(lhs, i, j) - xMatrix_get(rhs, i, j));
        }
    }

    return mat;
}

xMatrix xMatrix_mul(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->cols != rhs->rows) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(lhs->rows, rhs->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // multiply matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < rhs->cols; j++) {
            float sum = 0.0f;
            for (xSize k = 0; k < lhs->cols; k++) {
                sum += xMatrix_get(lhs, i, k) * xMatrix_get(rhs, k, j);
            }
            xMatrix_set(&mat, i, j, sum);
        }
    }

    return mat;
}

xMatrix xMatrix_dotmul(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // element-wise multiplication
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(lhs, i, j) * xMatrix_get(rhs, i, j));
        }
    }

    return mat;
}

xMatrix xMatrix_scalarAdd(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // add scalar all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(matrix, i, j) + scalar);
        }
    }

    return mat;
}

xMatrix xMatrix_scalarSub(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // subtract scalar from all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(matrix, i, j) - scalar);
        }
    }

    return mat;
}

xMatrix xMatrix_scalarMul(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // multiply matrix by scalar
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(matrix, i, j) * scalar);
        }
    }

    return mat;
}

xMatrix xMatrix_scalarDiv(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // divide matrix by scalar
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, xMatrix_get(matrix, i, j) / scalar);
        }
    }

    return mat;
}

xMatrix xMatrix_submatrix(const xMatrix *matrix, xSize row1, xSize col1, xSize row2, xSize col2)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row1 >= matrix->rows || col1 >= matrix->cols || row2 <= row1 || col2 <= col1) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(row2 - row1, col2 - col1);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // copy values from original matrix
    for (xSize i = row1; i < row2; i++) {
        for (xSize j = col1; j < col2; j++) {
            xMatrix_set(&mat, i - row1, j - col1, xMatrix_get(matrix, i, j));
        }
    }

    return mat;
}

xMatrix xMatrix_minor(const xMatrix *matrix, xSize row, xSize col)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row >= matrix->rows || col >= matrix->cols) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows - 1, matrix->cols - 1);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // create minor matrix
    for (xSize i = 0, k = 0; i < matrix->rows; i++) {
        if (i == row) {
            continue;
        }
        for (xSize j = 0, l = 0; j < matrix->cols; j++) {
            if (j == col) {
                continue;
            }
            xMatrix_set(&mat, k, l, xMatrix_get(matrix, i, j));
            l++;
        }
        k++;
    }

    return mat;
}

xMatrix xMatrix_map(const xMatrix *matrix, float (*func)(float))
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || !func) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, func(xMatrix_get(matrix, i, j)));
        }
    }

    return mat;
}

xMatrix xMatrix_map2(const xMatrix *lhs, const xMatrix *rhs, float (*func)(float, float))
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || !func || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(&mat, i, j, func(xMatrix_get(lhs, i, j), xMatrix_get(rhs, i, j)));
        }
    }

    return mat;
}

xMatrix xMatrix_mapScalar(const xMatrix *matrix, float scalar, float (*func)(float, float))
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || !func) {
        return (xMatrix){0};
    }

    // create matrix to store result
    xMatrix mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(&mat)) {
        return mat;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(&mat, i, j, func(xMatrix_get(matrix, i, j), scalar));
        }
    }

    return mat;
}
