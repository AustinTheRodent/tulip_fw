#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

union ufloat
{
  float f;
  uint32_t bytes;
};


int main (int argc, char *argv[])
{
  union ufloat u1;
  u1.f = 4500000;


  printf("u1.bytes: %f\n", u1.f);
  printf("u1.bytes: %X\n", u1.bytes);

  return 1;

}
