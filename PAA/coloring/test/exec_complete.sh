#!/bin/bash

#
# ./execute.sh 10 10 2 10
#

do_test() 
{
    SAMPLES=$1

    for ((i = 0; i < $SAMPLES; i++))
    do
        FILE="$i"
        ../bin/tp-final "$i" "../out/c$i"
    done
}

do_test 200
