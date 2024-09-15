/**
 * @file xMatrix_test.c
 * @author 0xDontCare (https://www.github.com/0xDontCare)
 * @brief CUnit test for all functions within xMatrix module.
 * @version 0.1
 * @date 14.09.2024.
 */

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <math.h>
#include "xBase/xTypes.h"
#include "xLinear/xMatrix.h"

void test_xMatrix_new(void)
{
    // Test case 1: Creating matrix with valid dimensions
    xMatrix *mat1 = xMatrix_new(3, 3);
    CU_ASSERT_PTR_NOT_NULL(mat1);
    CU_ASSERT_EQUAL(xMatrix_getRows(mat1), 3);
    CU_ASSERT_EQUAL(xMatrix_getCols(mat1), 3);
    CU_ASSERT_TRUE(xMatrix_isValid(mat1));
    xMatrix_free(mat1);

    // Test case 2: Creating matrix with zero rows
    xMatrix *mat2 = xMatrix_new(0, 3);
    CU_ASSERT_PTR_NULL(mat2);

    // Test case 3: Creating matrix with zero columns
    xMatrix *mat3 = xMatrix_new(3, 0);
    CU_ASSERT_PTR_NULL(mat3);

    // Test case 4: Creating matrix with zero dimensions
    xMatrix *mat4 = xMatrix_new(0, 0);
    CU_ASSERT_PTR_NULL(mat4);
}

void test_xMatrix_free(void)
{
    // Test case 1: Freeing NULL matrix
    xMatrix_free(NULL);  // should not crash

    // Test case 2: Freeing valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix_free(mat);
}

void test_xMatrix_getRows(void)
{
    // Test case 1: Getting rows from NULL matrix
    CU_ASSERT_EQUAL(xMatrix_getRows(NULL), 0);

    // Test case 2: Getting rows from valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    CU_ASSERT_EQUAL(xMatrix_getRows(mat), 3);
    xMatrix_free(mat);
}

void test_xMatrix_getCols(void)
{
    // Test case 1: Getting columns from NULL matrix
    CU_ASSERT_EQUAL(xMatrix_getCols(NULL), 0);

    // Test case 2: Getting columns from valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    CU_ASSERT_EQUAL(xMatrix_getCols(mat), 3);
    xMatrix_free(mat);
}

void test_xMatrix_isValid(void)
{
    // Test case 1: Checking NULL matrix
    CU_ASSERT_FALSE(xMatrix_isValid(NULL));

    // Test case 2: Checking valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    CU_ASSERT_TRUE(xMatrix_isValid(mat));
    xMatrix_free(mat);
}

void test_xMatrix_getset(void)
{
    // Test case 1: Getting and setting elements in NULL matrix
    xMatrix *mat = NULL;
    CU_ASSERT_TRUE(isnanf(xMatrix_get(mat, 0, 0)));
    xMatrix_set(mat, 0, 0, 1.0f);  // should not crash

    // Test case 2: Getting and setting elements in valid matrix
    mat = xMatrix_new(3, 3);
    CU_ASSERT_EQUAL(xMatrix_get(mat, 0, 0), 0.0f);
    xMatrix_set(mat, 0, 0, 1.0f);
    CU_ASSERT_EQUAL(xMatrix_get(mat, 0, 0), 1.0f);
    xMatrix_free(mat);

    // Test case 3: Getting and setting elements out of bounds
    mat = xMatrix_new(3, 3);
    CU_ASSERT_TRUE(isnanf(xMatrix_get(mat, 3, 3)));
    xMatrix_set(mat, 3, 3, 1.0f);  // should not crash
    xMatrix_free(mat);
}

void test_xMatrix_duplicate(void)
{
    // Test case 1: Duplicating NULL matrix
    CU_ASSERT_PTR_NULL(xMatrix_duplicate(NULL));

    // Test case 2: Duplicating valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix *dup = xMatrix_duplicate(mat);
    CU_ASSERT_PTR_NOT_NULL(dup);
    CU_ASSERT_TRUE(xMatrix_isValid(dup));
    xMatrix_free(mat);
    xMatrix_free(dup);

    // Test case 3: Duplicating matrix with data
    mat = xMatrix_new(3, 3);
    xMatrix_set(mat, 0, 0, 1.0f);
    xMatrix_set(mat, 1, 1, 2.0f);
    xMatrix_set(mat, 2, 2, 3.0f);
    dup = xMatrix_duplicate(mat);
    CU_ASSERT_EQUAL(xMatrix_get(dup, 0, 0), 1.0f);
    CU_ASSERT_EQUAL(xMatrix_get(dup, 1, 1), 2.0f);
    CU_ASSERT_EQUAL(xMatrix_get(dup, 2, 2), 3.0f);
    xMatrix_free(mat);
    xMatrix_free(dup);
}

void test_xMatrix_fill(void)
{
    // Test case 1: Filling NULL matrix
    xMatrix_fill(NULL, 1.0f);  // should not crash

    // Test case 2: Filling valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(mat, i, j), 1.0f);
        }
    }
    xMatrix_free(mat);

    // Test case 3: Filling matrix with NaN
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 0.0f / 0.0f);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_TRUE(isnanf(xMatrix_get(mat, i, j)));
        }
    }
    xMatrix_free(mat);
}

void test_xMatrix_identity(void)
{
    // Test case 1: Creating identity matrix with zero dimension
    CU_ASSERT_PTR_NULL(xMatrix_identity(0));

    // Test case 2: Creating identity matrix with valid dimension
    xMatrix *mat = xMatrix_identity(3);
    CU_ASSERT_PTR_NOT_NULL(mat);
    CU_ASSERT_TRUE(xMatrix_isValid(mat));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            if (i == j) {
                CU_ASSERT_EQUAL(xMatrix_get(mat, i, j), 1.0f);
            } else {
                CU_ASSERT_EQUAL(xMatrix_get(mat, i, j), 0.0f);
            }
        }
    }
    xMatrix_free(mat);
}

void test_xMatrix_transpose(void)
{
    // Test case 1: Transposing NULL matrix
    CU_ASSERT_PTR_NULL(xMatrix_transpose(NULL));

    // Test case 2: Transposing valid matrix
    xMatrix *mat = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(mat, i, j, i * 3 + j);
        }
    }
    xMatrix *transposed = xMatrix_transpose(mat);
    CU_ASSERT_PTR_NOT_NULL(transposed);
    CU_ASSERT_TRUE(xMatrix_isValid(transposed));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(transposed, i, j), j * 3 + i);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(transposed);

    // Test case 3: Transposing 1x1 matrix
    mat = xMatrix_new(1, 1);
    xMatrix_set(mat, 0, 0, 1.0f);
    transposed = xMatrix_transpose(mat);
    CU_ASSERT_PTR_NOT_NULL(transposed);
    CU_ASSERT_TRUE(xMatrix_isValid(transposed));
    CU_ASSERT_EQUAL(xMatrix_get(transposed, 0, 0), 1.0f);
    xMatrix_free(mat);
    xMatrix_free(transposed);

    // Test case 4: Transposing non-square matrix
    mat = xMatrix_new(1, 3);
    xMatrix_set(mat, 0, 0, 1.0f);
    xMatrix_set(mat, 0, 1, 2.0f);
    xMatrix_set(mat, 0, 2, 3.0f);
    transposed = xMatrix_transpose(mat);
    CU_ASSERT_PTR_NOT_NULL(transposed);
    CU_ASSERT_TRUE(xMatrix_isValid(transposed));
    CU_ASSERT_EQUAL(xMatrix_get(transposed, 0, 0), 1.0f);
    CU_ASSERT_EQUAL(xMatrix_get(transposed, 1, 0), 2.0f);
    CU_ASSERT_EQUAL(xMatrix_get(transposed, 2, 0), 3.0f);
    xMatrix_free(mat);
    xMatrix_free(transposed);
}

void test_xMatrix_add(void)
{
    // Test case 1: Adding NULL matrices
    CU_ASSERT_PTR_NULL(xMatrix_add(NULL, NULL));

    // Test case 2: Adding matrix with NULL matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix *sum = xMatrix_add(mat, NULL);
    CU_ASSERT_PTR_NULL(sum);
    xMatrix_free(mat);

    // Test case 3: Adding matrices with different dimensions
    xMatrix *lhs = xMatrix_new(3, 3);
    xMatrix *rhs = xMatrix_new(2, 2);
    sum = xMatrix_add(lhs, rhs);
    CU_ASSERT_PTR_NULL(sum);

    // Test case 4: Adding valid matrices
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
            xMatrix_set(rhs, i, j, j * 3 + i);
        }
    }
    sum = xMatrix_add(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(sum);
    CU_ASSERT_TRUE(xMatrix_isValid(sum));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(sum, i, j), i * 4 + j * 4);
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(sum);
}

void test_xMatrix_sub(void)
{
    // Test case 1: Subtracting NULL matrices
    CU_ASSERT_PTR_NULL(xMatrix_sub(NULL, NULL));

    // Test case 2: Subtracting matrix with NULL matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix *diff = xMatrix_sub(mat, NULL);
    CU_ASSERT_PTR_NULL(diff);
    xMatrix_free(mat);

    // Test case 3: Subtracting matrices with different dimensions
    xMatrix *lhs = xMatrix_new(3, 3);
    xMatrix *rhs = xMatrix_new(2, 2);
    diff = xMatrix_sub(lhs, rhs);
    CU_ASSERT_PTR_NULL(diff);
    xMatrix_free(lhs);
    xMatrix_free(rhs);

    // Test case 4: Subtracting valid matrices
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
            xMatrix_set(rhs, i, j, j * 3 + i);
        }
    }
    diff = xMatrix_sub(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(diff);
    CU_ASSERT_TRUE(xMatrix_isValid(diff));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(diff, i, j), (float)(i * 2) - (float)(j * 2));
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(diff);

    // Test case 5: Subtracting matrix from itself
    lhs = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
        }
    }
    diff = xMatrix_sub(lhs, lhs);
    CU_ASSERT_PTR_NOT_NULL(diff);
    CU_ASSERT_TRUE(xMatrix_isValid(diff));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(diff, i, j), 0.0f);
        }
    }
    xMatrix_free(lhs);
}

void test_xMatrix_mul(void)
{
    // Test case 1: Multiplying NULL matrices
    CU_ASSERT_PTR_NULL(xMatrix_mul(NULL, NULL));

    // Test case 2: Multiplying matrix with NULL matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix *prod = xMatrix_mul(mat, NULL);
    CU_ASSERT_PTR_NULL(prod);
    xMatrix_free(mat);

    // Test case 3: Multiplying matrices with incompatible dimensions
    xMatrix *lhs = xMatrix_new(3, 3);
    xMatrix *rhs = xMatrix_new(2, 2);
    prod = xMatrix_mul(lhs, rhs);
    CU_ASSERT_PTR_NULL(prod);
    xMatrix_free(lhs);
    xMatrix_free(rhs);

    // Test case 4: Multiplying valid matrices
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
            xMatrix_set(rhs, i, j, j * 3 + i);
        }
    }
    prod = xMatrix_mul(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            float sum = 0.0f;
            for (xSize k = 0; k < 3; k++) {
                sum += xMatrix_get(lhs, i, k) * xMatrix_get(rhs, k, j);
            }
            CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), sum);
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(prod);

    // Test case 5: Multiplying matrix by identity matrix
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_identity(3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
        }
    }
    prod = xMatrix_mul(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), xMatrix_get(lhs, i, j));
        }
    }
    xMatrix_free(lhs);

    // Test case 6: Multiplying non-square matrices
    lhs = xMatrix_new(3, 2);
    rhs = xMatrix_new(2, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 2; j++) {
            xMatrix_set(lhs, i, j, i * 2 + j);
            xMatrix_set(rhs, j, i, i * 2 + j);
        }
    }
    prod = xMatrix_mul(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            float sum = 0.0f;
            for (xSize k = 0; k < 2; k++) {
                sum += xMatrix_get(lhs, i, k) * xMatrix_get(rhs, k, j);
            }
            CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), sum);
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(prod);
}

void test_xMatrix_dotmul(void)
{
    // Test case 1: Multiplying NULL matrices
    CU_ASSERT_PTR_NULL(xMatrix_dotmul(NULL, NULL));

    // Test case 2: Multiplying matrix with NULL matrix
    xMatrix *mat = xMatrix_new(3, 3);
    xMatrix *prod = xMatrix_dotmul(mat, NULL);
    CU_ASSERT_PTR_NULL(prod);
    xMatrix_free(mat);

    // Test case 3: Multiplying matrices with different dimensions
    xMatrix *lhs = xMatrix_new(3, 3);
    xMatrix *rhs = xMatrix_new(2, 2);
    prod = xMatrix_dotmul(lhs, rhs);
    CU_ASSERT_PTR_NULL(prod);
    xMatrix_free(lhs);
    xMatrix_free(rhs);

    // Test case 4: Multiplying valid matrices
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
            xMatrix_set(rhs, i, j, j * 3 + i);
        }
    }
    prod = xMatrix_dotmul(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), (3 * i + j) * (3 * j + i));
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(prod);

    // Test case 5: Multiplying matrix by identity matrix
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_identity(3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(lhs, i, j, i * 3 + j);
        }
    }
    prod = xMatrix_dotmul(lhs, rhs);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            if (i == j) {
                CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), i * 3 + j);
            } else {
                CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), 0.0f);
            }
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(prod);
}

void test_xMatrix_scalarAdd(void)
{
    // Test case 1: Adding scalar to NULL matrix
    xMatrix *mat = NULL;
    xMatrix *sum = xMatrix_scalarAdd(mat, 1.0f);
    CU_ASSERT_PTR_NULL(sum);

    // Test case 2: Adding scalar to valid matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    sum = xMatrix_scalarAdd(mat, 1.0f);
    CU_ASSERT_PTR_NOT_NULL(sum);
    CU_ASSERT_TRUE(xMatrix_isValid(sum));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(sum, i, j), 2.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(sum);

    // Test case 3: Adding NaN to matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 5.0f);
    sum = xMatrix_scalarAdd(mat, 0.0f / 0.0f);
    CU_ASSERT_PTR_NOT_NULL(sum);
    CU_ASSERT_TRUE(xMatrix_isValid(sum));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_TRUE(isnanf(xMatrix_get(sum, i, j)));
        }
    }
    xMatrix_free(mat);
    xMatrix_free(sum);
}

void test_xMatrix_scalarSub(void)
{
    // Test case 1: Subtracting scalar from NULL matrix
    xMatrix *mat = NULL;
    xMatrix *diff = xMatrix_scalarSub(mat, 1.0f);
    CU_ASSERT_PTR_NULL(diff);

    // Test case 2: Subtracting scalar from valid matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    diff = xMatrix_scalarSub(mat, 1.0f);
    CU_ASSERT_PTR_NOT_NULL(diff);
    CU_ASSERT_TRUE(xMatrix_isValid(diff));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(diff, i, j), 0.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(diff);

    // Test case 3: Subtracting NaN from matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 5.0f);
    diff = xMatrix_scalarSub(mat, 0.0f / 0.0f);
    CU_ASSERT_PTR_NOT_NULL(diff);
    CU_ASSERT_TRUE(xMatrix_isValid(diff));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_TRUE(isnanf(xMatrix_get(diff, i, j)));
        }
    }
    xMatrix_free(mat);
    xMatrix_free(diff);
}

void test_xMatrix_scalarMul(void)
{
    // Test case 1: Multiplying NULL matrix by scalar
    xMatrix *mat = NULL;
    xMatrix *prod = xMatrix_scalarMul(mat, 1.0f);
    CU_ASSERT_PTR_NULL(prod);

    // Test case 2: Multiplying valid matrix by scalar
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    prod = xMatrix_scalarMul(mat, 2.0f);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_EQUAL(xMatrix_get(prod, i, j), 2.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(prod);

    // Test case 3: Multiplying matrix by NaN
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 5.0f);
    prod = xMatrix_scalarMul(mat, 0.0f / 0.0f);
    CU_ASSERT_PTR_NOT_NULL(prod);
    CU_ASSERT_TRUE(xMatrix_isValid(prod));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_TRUE(isnanf(xMatrix_get(prod, i, j)));
        }
    }
    xMatrix_free(mat);
    xMatrix_free(prod);
}

void test_xMatrix_scalarDiv(void)
{
    // Test case 1: Dividing NULL matrix by scalar
    xMatrix *mat = NULL;
    xMatrix *quot = xMatrix_scalarDiv(mat, 1.0f);
    CU_ASSERT_PTR_NULL(quot);

    // Test case 2: Dividing valid matrix by scalar
    mat = xMatrix_new(3, 3);
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            xMatrix_set(mat, i, j, (float)(3 * i + j + 1));
        }
    }
    quot = xMatrix_scalarDiv(mat, 2.0f);
    CU_ASSERT_PTR_NOT_NULL(quot);
    CU_ASSERT_TRUE(xMatrix_isValid(quot));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_DOUBLE_EQUAL(xMatrix_get(quot, i, j), (3 * i + j + 1) / 2.0f, 0.0001);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(quot);

    // Test case 3: Dividing matrix by NaN
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 5.0f);
    quot = xMatrix_scalarDiv(mat, 0.0f / 0.0f);
    CU_ASSERT_PTR_NOT_NULL(quot);
    CU_ASSERT_TRUE(xMatrix_isValid(quot));
    for (xSize i = 0; i < 3; i++) {
        for (xSize j = 0; j < 3; j++) {
            CU_ASSERT_TRUE(isnanf(xMatrix_get(quot, i, j)));
        }
    }
    xMatrix_free(mat);
    xMatrix_free(quot);
}

void test_xMatrix_submatrix(void)
{
    // Test case 1: Getting submatrix from NULL matrix
    xMatrix *mat = NULL;
    xMatrix *submat = xMatrix_submatrix(mat, 0, 0, 1, 1);
    CU_ASSERT_PTR_NULL(submat);

    // Test case 2: Getting submatrix with invalid bounds
    mat = xMatrix_new(3, 3);
    submat = xMatrix_submatrix(mat, 1, 1, 0, 0);
    CU_ASSERT_PTR_NULL(submat);
    xMatrix_free(mat);

    // Test case 3: Getting submatrix with valid bounds
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    submat = xMatrix_submatrix(mat, 0, 0, 2, 2);
    CU_ASSERT_PTR_NOT_NULL(submat);
    CU_ASSERT_TRUE(xMatrix_isValid(submat));
    CU_ASSERT_EQUAL(xMatrix_getRows(submat), 2);
    CU_ASSERT_EQUAL(xMatrix_getCols(submat), 2);
    for (xSize i = 0; i < xMatrix_getRows(submat); i++) {
        for (xSize j = 0; j < xMatrix_getCols(submat); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(submat, i, j), 1.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(submat);

    // Test case 4: Getting non-square submatrix from square matrix
    mat = xMatrix_new(5, 5);
    for (xSize i = 0; i < 5; i++) {
        for (xSize j = 0; j < 5; j++) {
            xMatrix_set(mat, i, j, (float)(5 * i + j + 1));
        }
    }
    submat = xMatrix_submatrix(mat, 1, 1, 3, 4);
    CU_ASSERT_PTR_NOT_NULL(submat);
    CU_ASSERT_TRUE(xMatrix_isValid(submat));
    CU_ASSERT_EQUAL(xMatrix_getRows(submat), 2);
    CU_ASSERT_EQUAL(xMatrix_getCols(submat), 3);
    for (xSize i = 0; i < xMatrix_getRows(submat); i++) {
        for (xSize j = 0; j < xMatrix_getCols(submat); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(submat, i, j), (float)(5 * (i + 1) + j + 2));
        }
    }
    xMatrix_free(mat);
    xMatrix_free(submat);
}

void test_xMatrix_minor(void)
{
    // Test case 1: Getting minor from NULL matrix
    xMatrix *mat = NULL;
    xMatrix *minor = xMatrix_minor(mat, 0, 0);
    CU_ASSERT_PTR_NULL(minor);

    // Test case 2: Getting minor with invalid bounds
    mat = xMatrix_new(3, 3);
    minor = xMatrix_minor(mat, 3, 3);
    CU_ASSERT_PTR_NULL(minor);
    xMatrix_free(mat);

    // Test case 3: Getting minor with valid bounds
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    minor = xMatrix_minor(mat, 1, 1);
    CU_ASSERT_PTR_NOT_NULL(minor);
    CU_ASSERT_TRUE(xMatrix_isValid(minor));
    CU_ASSERT_EQUAL(xMatrix_getRows(minor), 2);
    CU_ASSERT_EQUAL(xMatrix_getCols(minor), 2);
    for (xSize i = 0; i < xMatrix_getRows(minor); i++) {
        for (xSize j = 0; j < xMatrix_getCols(minor); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(minor, i, j), 1.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(minor);

    // Test case 4: Getting minor from non-square matrix
    mat = xMatrix_new(3, 2);
    xMatrix_fill(mat, 1.0f);
    minor = xMatrix_minor(mat, 1, 1);
    CU_ASSERT_PTR_NOT_NULL(minor);
    CU_ASSERT_TRUE(xMatrix_isValid(minor));
    CU_ASSERT_EQUAL(xMatrix_getRows(minor), 2);
    CU_ASSERT_EQUAL(xMatrix_getCols(minor), 1);
    for (xSize i = 0; i < xMatrix_getRows(minor); i++) {
        for (xSize j = 0; j < xMatrix_getCols(minor); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(minor, i, j), 1.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(minor);

    // Test case 5: Getting minor from 1x1 matrix
    mat = xMatrix_new(1, 1);
    xMatrix_fill(mat, 1.0f);
    minor = xMatrix_minor(mat, 0, 0);
    CU_ASSERT_PTR_NULL(minor);
    xMatrix_free(mat);
}

xSize processedCtr = 0;

static float dummyMap1(float val)
{
    processedCtr++;
    return val;
}

static float dummyMap2(float val1, float val2)
{
    processedCtr++;
    return val1 + val2;
}

void test_xMatrix_map(void)
{
    // Test case 1: Mapping NULL matrix
    xMatrix *mat = NULL;
    processedCtr = 0;
    xMatrix *mapped = xMatrix_map(mat, dummyMap1);
    CU_ASSERT_PTR_NULL(mapped);
    CU_ASSERT_EQUAL(processedCtr, 0);

    // Test case 2: Mapping valid matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    processedCtr = 0;
    mapped = xMatrix_map(mat, dummyMap1);
    CU_ASSERT_PTR_NOT_NULL(mapped);
    CU_ASSERT_TRUE(xMatrix_isValid(mapped));
    CU_ASSERT_EQUAL(processedCtr, 9);
    for (xSize i = 0; i < xMatrix_getRows(mapped); i++) {
        for (xSize j = 0; j < xMatrix_getCols(mapped); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(mapped, i, j), 1.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(mapped);
}

void test_xMatrix_map2(void)
{
    // Test case 1: Mapping NULL matrices
    xMatrix *lhs = NULL;
    xMatrix *rhs = NULL;
    processedCtr = 0;
    xMatrix *mapped = xMatrix_map2(lhs, rhs, dummyMap2);
    CU_ASSERT_PTR_NULL(mapped);
    CU_ASSERT_EQUAL(processedCtr, 0);

    // Test case 2: Mapping matrices with different dimensions
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(2, 2);
    processedCtr = 0;
    mapped = xMatrix_map2(lhs, rhs, dummyMap2);
    CU_ASSERT_PTR_NULL(mapped);
    CU_ASSERT_EQUAL(processedCtr, 0);
    xMatrix_free(lhs);
    xMatrix_free(rhs);

    // Test case 3: Mapping valid matrices
    lhs = xMatrix_new(3, 3);
    rhs = xMatrix_new(3, 3);
    xMatrix_fill(lhs, 1.0f);
    xMatrix_fill(rhs, 2.0f);
    processedCtr = 0;
    mapped = xMatrix_map2(lhs, rhs, dummyMap2);
    CU_ASSERT_PTR_NOT_NULL(mapped);
    CU_ASSERT_TRUE(xMatrix_isValid(mapped));
    CU_ASSERT_EQUAL(processedCtr, 9);
    for (xSize i = 0; i < xMatrix_getRows(mapped); i++) {
        for (xSize j = 0; j < xMatrix_getCols(mapped); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(mapped, i, j), 3.0f);
        }
    }
    xMatrix_free(lhs);
    xMatrix_free(rhs);
    xMatrix_free(mapped);
}

void test_xMatrix_mapScalar(void)
{
    // Test case 1: Mapping NULL matrix
    xMatrix *mat = NULL;
    processedCtr = 0;
    xMatrix *mapped = xMatrix_mapScalar(mat, 1.0f, dummyMap2);
    CU_ASSERT_PTR_NULL(mapped);
    CU_ASSERT_EQUAL(processedCtr, 0);

    // Test case 2: Mapping valid matrix
    mat = xMatrix_new(3, 3);
    xMatrix_fill(mat, 1.0f);
    processedCtr = 0;
    mapped = xMatrix_mapScalar(mat, 2.0f, dummyMap2);
    CU_ASSERT_PTR_NOT_NULL(mapped);
    CU_ASSERT_TRUE(xMatrix_isValid(mapped));
    CU_ASSERT_EQUAL(processedCtr, 9);
    for (xSize i = 0; i < xMatrix_getRows(mapped); i++) {
        for (xSize j = 0; j < xMatrix_getCols(mapped); j++) {
            CU_ASSERT_EQUAL(xMatrix_get(mapped, i, j), 3.0f);
        }
    }
    xMatrix_free(mat);
    xMatrix_free(mapped);
}

int main(void)
{
    CU_pSuite suite = NULL;

    // Initialize CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    // Add a suite to the registry
    suite = CU_add_suite("xMatrix_test", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the tests to the suite
    if ((CU_add_test(suite, "xMatrix_new", test_xMatrix_new) == NULL) ||
        (CU_add_test(suite, "xMatrix_getRows", test_xMatrix_getRows) == NULL) ||
        (CU_add_test(suite, "xMatrix_getCols", test_xMatrix_getCols) == NULL) ||
        (CU_add_test(suite, "xMatrix_isValid", test_xMatrix_isValid) == NULL) ||
        (CU_add_test(suite, "xMatrix_getset", test_xMatrix_getset) == NULL) ||
        (CU_add_test(suite, "xMatrix_duplicate", test_xMatrix_duplicate) == NULL) ||
        (CU_add_test(suite, "xMatrix_fill", test_xMatrix_fill) == NULL) ||
        (CU_add_test(suite, "xMatrix_identity", test_xMatrix_identity) == NULL) ||
        (CU_add_test(suite, "xMatrix_transpose", test_xMatrix_transpose) == NULL) ||
        (CU_add_test(suite, "xMatrix_add", test_xMatrix_add) == NULL) ||
        (CU_add_test(suite, "xMatrix_sub", test_xMatrix_sub) == NULL) ||
        (CU_add_test(suite, "xMatrix_mul", test_xMatrix_mul) == NULL) ||
        (CU_add_test(suite, "xMatrix_dotmul", test_xMatrix_dotmul) == NULL) ||
        (CU_add_test(suite, "xMatrix_scalarAdd", test_xMatrix_scalarAdd) == NULL) ||
        (CU_add_test(suite, "xMatrix_scalarSub", test_xMatrix_scalarSub) == NULL) ||
        (CU_add_test(suite, "xMatrix_scalarMul", test_xMatrix_scalarMul) == NULL) ||
        (CU_add_test(suite, "xMatrix_scalarDiv", test_xMatrix_scalarDiv) == NULL) ||
        (CU_add_test(suite, "xMatrix_submatrix", test_xMatrix_submatrix) == NULL) ||
        (CU_add_test(suite, "xMatrix_minor", test_xMatrix_minor) == NULL) ||
        (CU_add_test(suite, "xMatrix_map", test_xMatrix_map) == NULL) ||
        (CU_add_test(suite, "xMatrix_map2", test_xMatrix_map2) == NULL) ||
        (CU_add_test(suite, "xMatrix_mapScalar", test_xMatrix_mapScalar) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
