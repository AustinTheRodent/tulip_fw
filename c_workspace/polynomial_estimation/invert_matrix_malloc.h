#ifndef INVERT_MATRIX_MALLOC_H
#define INVERT_MATRIX_MALLOC_H

int8_t invert_matrix(float** input, float order, float** inverse);
float determinant(float** a, float k);
void cofactor(float** num, float f, float** inverse);
void transpose(float** num, float** fac, float r, float** inverse);

#endif
