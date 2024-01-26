#!/bin/bash

make clean
make
./main.exe -i transfer_function.txt -o poly_taps.txt
