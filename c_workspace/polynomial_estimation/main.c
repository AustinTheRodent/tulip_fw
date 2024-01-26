#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <string.h>
#include <math.h>
#include "invert_matrix_malloc.h"

uint32_t get_flines(char* fname);

int main(int argc, char *argv[])
{

  int i, j;
  uint8_t arg_count = 0;
  char* input_fname;
  char* output_fname;
  FILE* fin_ptr;
  FILE* fout_ptr;
  char * line = NULL;
  size_t len = 0;

  uint32_t transfer_function_sz;
  float* transfer_function;
  float** A;
  float** A_inv;
  float* B;

  for(i = 0 ; i < argc ; i++)
  {
    if(strcmp(argv[i], "-i") == 0)
    {
      input_fname = argv[i+1];
      arg_count++;
    }
    else if(strcmp(argv[i], "-o") == 0)
    {
      output_fname = argv[i+1];
      arg_count++;
    }
  }

  printf("arg_count: %i\n", arg_count);

  if(arg_count < 2)
  {
    printf("Error: Not enough args\n");
    return -1;
  }

  transfer_function_sz = get_flines(input_fname);
  printf("transfer_function_sz: %i\n", transfer_function_sz);

  transfer_function = (float*)malloc(transfer_function_sz*sizeof(float));

  fin_ptr = fopen(input_fname, "r");
  for(i=0;i<transfer_function_sz;i++)
  {
    getline(&line, &len, fin_ptr);
    transfer_function[i] = atof(line);
  }
  fclose(fin_ptr);

  for(i=0;i<transfer_function_sz;i++)
  {
    printf("transfer_function[%i]: %f\n", i, transfer_function[i]);
  }

  B = (float*)malloc(transfer_function_sz*sizeof(float));

  A = (float**)malloc(transfer_function_sz*sizeof(float*));
  A_inv = (float**)malloc(transfer_function_sz*sizeof(float*));
  for(i=0;i<transfer_function_sz;i++)
  {
    A[i] = (float*)malloc(transfer_function_sz*sizeof(float));
    A_inv[i] = (float*)malloc(transfer_function_sz*sizeof(float));
  }

  for(i=0;i<transfer_function_sz;i++)
  {
    for(j=0;j<transfer_function_sz;j++)
    {
      A[i][j] = pow(i, j);
    }
  }

  printf("\n");

  if(invert_matrix(A, transfer_function_sz, A_inv) == -1)
  {
    printf("Error: inverse matrix issue\n");
    return -1;
  }

  for(i=0;i<transfer_function_sz;i++)
  {
    B[i] = 0;
  }
  for(i=0;i<transfer_function_sz;i++)
  {
    for(j=0;j<transfer_function_sz;j++)
    {
      B[i] = B[i] + transfer_function[j]*A_inv[i][j];
    }
  }

  printf("B:\n");
  for(i=0;i<transfer_function_sz;i++)
  {
    printf("B[%i]: %f\n", i, B[i]);
  }

  fout_ptr = fopen(output_fname, "w");
  for(i=0;i<transfer_function_sz;i++)
  {
    fprintf(fout_ptr, "%f\n", B[i]);
  }
  fclose(fout_ptr);

  // free malloc variables:
  free(transfer_function);
  free(B);

  for(i=0;i<transfer_function_sz;i++)
  {
    free(A[i]);
    free(A_inv[i]);
  }
  free(A);
  free(A_inv);

}

uint32_t get_flines(char* fname)
{
  FILE* fin_ptr = fopen(fname, "r");
  char * line = NULL;
  size_t len = 0;
  uint32_t count = 0;
  while(getline(&line, &len, fin_ptr) != -1)
  {
    count++;
  }
  return count;
}





















//int main()
//{
//  float k;
//  float** a;
//  float** inverse;
//  int i, j;
//  int8_t ret_val;
//
//  printf("Enter the order of the Matrix : ");
//  scanf("%f", &k);
//
//  a = (float**) malloc(k * sizeof(float*));
//  inverse = (float**) malloc(k * sizeof(float*));
//  for(i=0;i<k;i++)
//  {
//    a[i]=(float*)malloc(k*sizeof(float));
//    inverse[i]=(float*)malloc(k*sizeof(float));
//  }
//
//  printf("Enter the elements of %.0fX%.0f Matrix : \n", k, k);
//  for (i = 0;i < k; i++)
//  {
//   for (j = 0;j < k; j++)
//    {
//      scanf("%f", &a[i][j]);
//    }
//  }
//
//  ret_val = invert_matrix(a, k, inverse);
//  if(ret_val == -1)
//  {
//    printf("cannot invert given matrix\n");
//    return 0;
//  }
//
//  printf("\n\n\nThe inverse of matrix is : \n");
//  for (i=0;i<k;i++)
//  {
//    for (j=0;j<k;j++)
//    {
//      printf("\t%f", inverse[i][j]);
//    }
//    printf("\n");
//  }
//
//  for(i=0;i<k;i++)
//  {
//    free(a[i]);
//    free(inverse[i]);
//  }
//  free(a);
//  free(inverse);
//
//}
