#include <stdio.h>
#include "matrix.h"

Matrix *create_matrix(int r, int c) {
    Matrix* m = (Matrix*) malloc(sizeof(Matrix));
    m->num_rows = r;
    m->num_cols = c;

    // Allocating memory to a 2D array 'data' to represent
    // the elements of the matrix in row-major order.
    
    m->data = (long long int**) calloc(r, sizeof(long long int*));
    for (int i = 0; i < r; i++) {
        m->data[i] = (long long int*) calloc(c, sizeof(long long int));
    }

    return m;
}

void destroy_matrix(Matrix *A) {
    if (!A) return;
    
    for (int i = 0; i < A->num_rows; i++) {
        free(A->data[i]);
    }

    free(A->data);
    free(A);
}

Matrix *add_matrix(Matrix *A, Matrix *B) {
    if (A->num_rows != B->num_rows || A->num_cols != B->num_cols) {
        // Invalid input
        return NULL;
    }

    int num_rows = A->num_cols, num_cols = A->num_cols;
    Matrix *Y = create_matrix(num_rows, num_cols);
    for (int r = 0; r < A->num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            Y->data[r][c] = A->data[r][c] + B->data[r][c];
        }
    }

    return Y;
}

Matrix *mult_matrix(Matrix *A, Matrix *B) {
    if (A->num_cols != B->num_rows) {
        // Invalid input
        return NULL;
    }

    int num_rows = A->num_rows, num_cols = B->num_cols;
    Matrix *Y = create_matrix(num_rows, num_cols);

    int product_length = A->num_cols;
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            // Calculating the 'dot product' of r-th row of A and c-th column of
            // B. The 'dot product' implies, for two arrays of the same length,
            // the sum of the products of corresponding elements at equal indices.

            long long int dot_product = 0;
            for (int n = 0; n < product_length; n++) {
                dot_product += A->data[r][n] * B->data[n][c];
            }

            Y->data[r][c] = dot_product;
        }
    }

    return Y;
}

Matrix *scalar_mult_matrix(long long int s, Matrix *A) {
    int num_rows = A->num_rows, num_cols = A->num_cols;
    Matrix *Y = create_matrix(num_rows, num_cols);
    
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            Y->data[r][c] = s * A->data[r][c];
        }
    }

    return Y;
}

Matrix *transpose_matrix(Matrix *A) {
    int num_rows = A->num_cols, num_cols = A->num_rows;
    Matrix *Y = create_matrix(num_rows, num_cols);

    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            Y->data[r][c] = A->data[c][r];
        }
    }

    return Y;
}

Matrix *_exclude_rc_matrix(Matrix *A, int exc_row, int exc_col) {
    // Excludes exc_row-th row and exc_col-th column (0-indexed) from matrix A.
    // This function requires exc_row < A->num_rows and exc_col < A->num_cols.
    // Otherwise returns NULL.

    if (exc_row >= A->num_rows || exc_col >= A->num_cols) return NULL;

    int new_rows = A->num_rows - 1;
    int new_cols = A->num_cols - 1;
    Matrix *Y = create_matrix(new_rows, new_cols);

    // Copying over normally upto row exc_row-1
    for (int r = 0; r < exc_row; r++) {
        // Copying over normally upto column exc_col-1
        for (int c = 0; c < exc_col; c++) {
            Y->data[r][c] = A->data[r][c];
        }

        // Each column of Y now takes its value from the next column in A
        for (int c = exc_col; c < new_cols; c++) {
            Y->data[r][c] = A->data[r][c+1];
        }
    }

    // Each row of Y now takes its value from the next row in A
    for (int r = exc_row; r < new_rows; r++) {
        // Copying over normally upto column exc_col-1
        for (int c = 0; c < exc_col; c++) {
            Y->data[r][c] = A->data[r+1][c];
        }

        // Each column of Y now takes its value from the next column in A
        for (int c = exc_col; c < new_cols; c++) {
            Y->data[r][c] = A->data[r+1][c+1];
        }
    }

    return Y;
}

long long int determinant(Matrix *A) {
    // Determinant can be calculated by calculating the 'dot product' of arrays
    // M and N, where M is the i-th row of the matrix A, and N is the i-th column
    // of the cofactor matrix of the matrix B (1 <= i <= dimension(A)).
    // 'Dot product' here is defined the same as in mult_matrix function.
    
    // Assumes that input matrix is always a square matrix.

    int order = A->num_rows;

    if (order == 1) return A->data[0][0];

    long long int det = 0;
    int sign = 1;
    for (int c = 0; c < order; c++, sign *= 1) {
        Matrix *c_excluded_matrix = _exclude_rc_matrix(A, 0, c);
        det += sign * A->data[0][c] * determinant(c_excluded_matrix);
        destroy_matrix(c_excluded_matrix);
    }

    return det;
}

void print_matrix(Matrix *m) {
    // Eliminating this function as it is implemented using file pointers in main.c
}