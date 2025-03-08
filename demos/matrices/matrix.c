#include "matrix.h"

#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

// Mátrix szorzás
void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            c[i][j] = 0.0f;
            for (int k = 0; k < 3; ++k)
                c[i][j] += a[i][k] * b[k][j];
        }
}

// Egységmátrix
void init_identity_matrix(float matrix[3][3])
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
}

// Mátrix skalárszorzás
void multiply_matrix_by_scalar(float matrix[3][3], float scalar)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] *= scalar;
}

// Skálázás
void scale(float matrix[3][3], float sx, float sy)
{
    matrix[0][0] *= sx;
    matrix[1][1] *= sy;
}

// Eltolás
void shift(float matrix[3][3], float dx, float dy)
{
    matrix[0][2] += dx;
    matrix[1][2] += dy;
}

// Mátrix elforgatás
void rotate(float matrix[3][3], float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    float temp[3][3];
    init_identity_matrix(temp);
    temp[0][0] = cosA; temp[0][1] = -sinA;
    temp[1][0] = sinA; temp[1][1] = cosA;
    multiply_matrices(temp, matrix, matrix);
}