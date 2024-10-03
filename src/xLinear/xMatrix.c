#include "xLinear/xMatrix.h"
#include <stdlib.h>  // standard library (for malloc, calloc, free)
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

struct xMatrix_s {
    float *data;  // contiguous array of all matrix elements
    xSize rows;   // number of rows
    xSize cols;   // number of columns
};

xMatrix *xMatrix_new(xSize rows, xSize cols)
{
    // validate arguments
    if (!rows || !cols) {
        return NULL;
    }

    // create matrix object and allocate memory for data
    xMatrix *mat = (xMatrix *)malloc(sizeof(xMatrix) + rows * cols * sizeof(float));
    if (!mat) {
        return NULL;
    }

    mat->data = (float *)(mat + 1);
    mat->rows = rows;
    mat->cols = cols;

    // zero-initialize matrix data
    for (xSize i = 0; i < rows * cols; i++) {
        mat->data[i] = 0.0f;
    }

    return mat;
}

void xMatrix_free(xMatrix *matrix)
{
    // validate arguments
    if (!matrix) {
        return;
    }

    // set matrix attributes to zero (invalidate matrix)
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;

    free(matrix);
}

inline xSize xMatrix_getRows(const xMatrix *matrix) { return (matrix) ? matrix->rows : 0; }

inline xSize xMatrix_getCols(const xMatrix *matrix) { return (matrix) ? matrix->cols : 0; }

inline xBool xMatrix_isValid(const xMatrix *matrix) { return (matrix) ? matrix->data && matrix->rows && matrix->cols : 0; }

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
        return 0.0f / 0.0f;  // NaN
    }

    // get matrix element
    return matrix->data[row * matrix->cols + col];
}

xMatrix *xMatrix_duplicate(const xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create matrix object
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // copy matrix data
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(mat, i, j, xMatrix_get(matrix, i, j));
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

xMatrix *xMatrix_identity(xSize dimension)
{
    // validate arguments
    if (!dimension) {
        return NULL;
    }

    // create identity matrix
    xMatrix *mat = xMatrix_new(dimension, dimension);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // assign values to diagonal elements
    for (xSize i = 0; i < dimension; i++) {
        xMatrix_set(mat, i, i, 1.0f);
    }

    return mat;
}

xMatrix *xMatrix_transpose(const xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create transposed matrix
    xMatrix *mat = xMatrix_new(matrix->cols, matrix->rows);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // transpose matrix
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(mat, j, i, xMatrix_get(matrix, i, j));
        }
    }

    return mat;
}

xMatrix *xMatrix_transpose_inplace(xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // transpose matrix in-place
    xSize rows = matrix->rows;
    xSize cols = matrix->cols;

    // allocate a temporary array to store the transposed data
    float *temp = (float *)malloc(rows * cols * sizeof(float));
    if (!temp) {
        return NULL;
    }

    // transpose the matrix into the temporary array
    for (xSize i = 0; i < rows; i++) {
        for (xSize j = 0; j < cols; j++) {
            temp[j * rows + i] = matrix->data[i * cols + j];
        }
    }

    // copy the transposed data back to the original matrix
    for (xSize i = 0; i < rows * cols; i++) {
        matrix->data[i] = temp[i];
    }

    // free the temporary array
    free(temp);

    // swap the rows and columns
    matrix->rows = cols;
    matrix->cols = rows;

    return matrix;
}

xMatrix *xMatrix_add(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation
    xMatrix *mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_add_inplace(mat, lhs, rhs)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_add_inplace(xMatrix *res, const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || res->cols != lhs->cols ||
        lhs->cols != rhs->cols || res->rows != lhs->rows || lhs->rows != rhs->rows) {
        // perform no action and return NULL instead of passing back `res`
        return NULL;
    }

    // add matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(lhs, i, j) + xMatrix_get(rhs, i, j));
        }
    }

    return res;
}

xMatrix *xMatrix_sub(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation
    xMatrix *mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_sub_inplace(mat, lhs, rhs)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_sub_inplace(xMatrix *res, const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || res->cols != lhs->cols ||
        lhs->cols != rhs->cols || res->rows != lhs->rows || lhs->rows != rhs->rows) {
        // perform no action and return NULL instead of passing back `res`
        return NULL;
    }

    // subtract matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(lhs, i, j) - xMatrix_get(rhs, i, j));
        }
    }

    return res;
}

xMatrix *xMatrix_mul(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->cols != rhs->rows) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation
    xMatrix *mat = xMatrix_new(lhs->rows, rhs->cols);
    if (!xMatrix_mul_inplace(mat, lhs, rhs)) {
        // opeation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_mul_inplace(xMatrix *res, const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || res->rows != lhs->rows ||
        res->cols != rhs->cols || lhs->cols != rhs->rows) {
        return NULL;
    }

    // multiply matrices
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < rhs->cols; j++) {
            float sum = 0.0f;
            for (xSize k = 0; k < lhs->cols; k++) {
                sum += xMatrix_get(lhs, i, k) * xMatrix_get(rhs, k, j);
            }
            xMatrix_set(res, i, j, sum);
        }
    }

    return res;
}

xMatrix *xMatrix_dotmul(const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation
    xMatrix *mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_dotmul_inplace(mat, lhs, rhs)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_dotmul_inplace(xMatrix *res, const xMatrix *lhs, const xMatrix *rhs)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || res->rows != lhs->rows ||
        res->cols != lhs->cols || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return NULL;
    }

    // element-wise multiplication
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(lhs, i, j) * xMatrix_get(rhs, i, j));
        }
    }

    return res;
}

xMatrix *xMatrix_scalarAdd(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_scalarAdd_inplace(mat, matrix, scalar)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_scalarAdd_inplace(xMatrix *res, const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(matrix) || res->rows != matrix->rows || res->cols != matrix->cols) {
        return NULL;
    }

    // add scalar to all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(matrix, i, j) + scalar);
        }
    }

    return res;
}

xMatrix *xMatrix_scalarSub(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation on it
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_scalarSub_inplace(mat, matrix, scalar)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_scalarSub_inplace(xMatrix *res, const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(matrix) || res->rows != matrix->rows || res->cols != matrix->cols) {
        return NULL;
    }

    // subtract scalar from all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(matrix, i, j) - scalar);
        }
    }

    return res;
}

xMatrix *xMatrix_scalarMul(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation on it
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_scalarMul_inplace(mat, matrix, scalar)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_scalarMul_inplace(xMatrix *res, const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(matrix) || res->rows != matrix->rows || res->cols != matrix->cols) {
        return NULL;
    }

    // multiply matrix by scalar
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(matrix, i, j) * scalar);
        }
    }

    return res;
}

xMatrix *xMatrix_scalarDiv(const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create matrix to store result and perform in-place operation on it
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_scalarDiv_inplace(mat, matrix, scalar)) {
        // operation failed, free memory and return
        xMatrix_free(mat);
        return NULL;
    }

    return mat;
}

xMatrix *xMatrix_scalarDiv_inplace(xMatrix *res, const xMatrix *matrix, float scalar)
{
    // validate arguments
    if (!xMatrix_isValid(res) || !xMatrix_isValid(matrix) || res->rows != matrix->rows || res->cols != matrix->cols) {
        return NULL;
    }

    // divide matrix by scalar
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(res, i, j, xMatrix_get(matrix, i, j) / scalar);
        }
    }

    return res;
}

xMatrix *xMatrix_submatrix(const xMatrix *matrix, xSize row1, xSize col1, xSize row2, xSize col2)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row1 >= matrix->rows || col1 >= matrix->cols || row2 <= row1 || col2 <= col1) {
        return NULL;
    }

    // create matrix to store result
    xMatrix *mat = xMatrix_new(row2 - row1, col2 - col1);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // copy values from original matrix
    for (xSize i = row1; i < row2; i++) {
        for (xSize j = col1; j < col2; j++) {
            xMatrix_set(mat, i - row1, j - col1, xMatrix_get(matrix, i, j));
        }
    }

    return mat;
}

xMatrix *xMatrix_minor(const xMatrix *matrix, xSize row, xSize col)
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || row >= matrix->rows || col >= matrix->cols) {
        return NULL;
    }

    // create matrix to store result
    xMatrix *mat = xMatrix_new(matrix->rows - 1, matrix->cols - 1);
    if (!xMatrix_isValid(mat)) {
        return NULL;
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
            xMatrix_set(mat, k, l, xMatrix_get(matrix, i, j));
            l++;
        }
        k++;
    }

    return mat;
}

xMatrix *xMatrix_map(const xMatrix *matrix, float (*func)(float))
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || !func) {
        return NULL;
    }

    // create matrix to store result
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(mat, i, j, func(xMatrix_get(matrix, i, j)));
        }
    }

    return mat;
}

xMatrix *xMatrix_map2(const xMatrix *lhs, const xMatrix *rhs, float (*func)(float, float))
{
    // validate arguments
    if (!xMatrix_isValid(lhs) || !xMatrix_isValid(rhs) || !func || lhs->rows != rhs->rows || lhs->cols != rhs->cols) {
        return NULL;
    }

    // create matrix to store result
    xMatrix *mat = xMatrix_new(lhs->rows, lhs->cols);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < lhs->rows; i++) {
        for (xSize j = 0; j < lhs->cols; j++) {
            xMatrix_set(mat, i, j, func(xMatrix_get(lhs, i, j), xMatrix_get(rhs, i, j)));
        }
    }

    return mat;
}

xMatrix *xMatrix_mapScalar(const xMatrix *matrix, float scalar, float (*func)(float, float))
{
    // validate arguments
    if (!xMatrix_isValid(matrix) || !func) {
        return NULL;
    }

    // create matrix to store result
    xMatrix *mat = xMatrix_new(matrix->rows, matrix->cols);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // apply function to all matrix elements
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            xMatrix_set(mat, i, j, func(xMatrix_get(matrix, i, j), scalar));
        }
    }

    return mat;
}

float *xMatrix_flatten(const xMatrix *matrix)
{
    // validate arguments
    if (!xMatrix_isValid(matrix)) {
        return NULL;
    }

    // create array to store flattened matrix
    float *arr = (float *)malloc(matrix->rows * matrix->cols * sizeof(float));
    if (!arr) {
        return NULL;
    }

    // copy data
    for (xSize i = 0; i < matrix->rows; i++) {
        for (xSize j = 0; j < matrix->cols; j++) {
            arr[i * matrix->cols + j] = xMatrix_get(matrix, i, j);
        }
    }

    return arr;
}

xMatrix *xMatrix_unflatten(const float *arr, xSize rows, xSize cols)
{
    // validate arguments
    if (!arr || !rows || !cols) {
        return NULL;
    }

    // create matrix to store unflattened array
    xMatrix *mat = xMatrix_new(rows, cols);
    if (!xMatrix_isValid(mat)) {
        return NULL;
    }

    // copy data
    for (xSize i = 0; i < rows; i++) {
        for (xSize j = 0; j < cols; j++) {
            xMatrix_set(mat, i, j, arr[i * cols + j]);
        }
    }

    return mat;
}
