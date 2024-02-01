
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "SparkFun_WM8960_Arduino_Library.h"

int main(int argc, char* argv[])
{
  int i;
  float vol;
  bool vol_defined = false;

  WM8960 codec;

  for (i = 0 ; i < argc ; i++)
  {
    if (strcmp(argv[i], "-i") == 0)
    {
      vol = std::stof(argv[i+1]);
      vol_defined = true;
    }
  }

  if (vol_defined == false)
  {
    printf("volume undefined, use \"-i\" to set volume in dB\n");
  }

  codec.setLINVOLDB(vol);
  codec.setRINVOLDB(vol);

  return 0;
}
