#include "matrix.h"
#include "stdio.h"
#include "math.h"

int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];
    float identity[3][3];
    float multiplied[3][3];
    float transform[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    print_matrix(a);
    print_matrix(b);
    
    add_matrices(a, b, c);

    print_matrix(c);

    // egységmátrix
    printf("\nEgységmátrix:\n");
    init_identity_matrix(identity);
    print_matrix(identity);

    // skalárral való szorzás
    printf("\nSkalárral való szorzás:\n");
    multiply_matrix_by_scalar(c, 3);
    print_matrix(c);

    // Mátrix szorzás
    printf("\nMátrix szorzás (a*b):\n");
    multiply_matrices(a, b, multiplied);
    print_matrix(multiplied);

    // Skálázás
    init_identity_matrix(transform);
    scale(transform, 2.0f, 3.0f);
    print_matrix(transform);

    // Eltolás
    init_identity_matrix(transform);
    shift(transform, 1.0f, 2.0f);
    print_matrix(transform);

    // Forgatás
    init_identity_matrix(transform);
    rotate(transform, M_PI / 4);
    print_matrix(transform);

	return 0;
}

