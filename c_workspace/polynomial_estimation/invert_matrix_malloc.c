#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "invert_matrix_malloc.h"

//int8_t invert_matrix(float** input, float order, float** inverse);
//float determinant(float** a, float k);
//void cofactor(float** num, float f, float** inverse);
//void transpose(float** num, float** fac, float r, float** inverse);

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

int8_t invert_matrix(float** input, float order, float** inverse)
{
  float d;
  d = determinant(input, order);
  if (d == 0)
  {
    return -1;
  }
  else
  {
    cofactor(input, order, inverse);
    return 0;
  }
}

/*For calculating Determinant of the Matrix */
float determinant(float** a, float k)
{
  float s = 1, det = 0;
  float** b;
  int i, j, m, n, c;

  b = (float**) malloc(k * sizeof(float*));
  for(i=0;i<k;i++)
  {
    b[i]=(float*)malloc(k*sizeof(float));
  }

  if (k == 1)
  {
    return (a[0][0]);
  }
  else
  {
    det = 0;
    for (c = 0; c < k; c++)
    {
      m = 0;
      n = 0;
      for (i = 0;i < k; i++)
      {
        for (j = 0 ;j < k; j++)
        {
          b[i][j] = 0;
          if (i != 0 && j != c)
          {
            b[m][n] = a[i][j];
            if (n < (k - 2))
            {
              n++;
            }
            else
            {
              n = 0;
              m++;
            }
          }
        }
      }
        det = det + s * (a[0][c] * determinant(b, k - 1));
        s = -1 * s;
     }
  }

  for(i=0;i<k;i++)
  {
   free(b[i]);
  }
  free(b);

  return (det);
}

void cofactor(float** num, float f, float** inverse)
{
  float** fac;
  float** b;
  int p, q, m, n, i, j;

  fac = (float**) malloc(f * sizeof(float*));
  b = (float**) malloc(f * sizeof(float*));
  for(i=0;i<f;i++)
  {
    b[i]=(float*)malloc(f*sizeof(float));
    fac[i]=(float*)malloc(f*sizeof(float));
  }

  for (q = 0;q < f; q++)
  {
    for (p = 0;p < f; p++)
    {
      m = 0;
      n = 0;
      for (i = 0;i < f; i++)
      {
        for (j = 0;j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = num[i][j];
            if (n < (f - 2))
            {
              n++;
            }
            else
            {
              n = 0;
              m++;
            }
          }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  transpose(num, fac, f, inverse);

  for(i=0;i<f;i++)
  {
   free(b[i]);
   free(fac[i]);
  }
  free(b);
  free(fac);

}

/*Finding transpose of matrix*/
void transpose(float** num, float** fac, float r, float** inverse)
{
  int i, j;
  float** b;
  float d;

  b = (float**) malloc(r * sizeof(float*));
  for(i=0;i<r;i++)
  {
    b[i]=(float*)malloc(r*sizeof(float));
  }

  for (i = 0;i < r; i++)
  {
    for (j = 0;j < r; j++)
    {
      b[i][j] = fac[j][i];
    }
  }
  d = determinant(num, r);
  for (i = 0;i < r; i++)
  {
    for (j = 0;j < r; j++)
    {
      inverse[i][j] = b[i][j] / d;
    }
  }

  for(i=0;i<r;i++)
  {
    free(b[i]);
  }
  free(b);

}
