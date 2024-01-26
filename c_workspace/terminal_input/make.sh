#!/bin/bash

for i in $@; do

    #echo $i
    case $i in
            "clean")
                cd products_make
                make clean
                exit
                ;;
    esac

done

cd products_make
make

#if [ -d ./objects/ ]; then # if directory exists
#    echo if
#else
#    echo else
#fi
