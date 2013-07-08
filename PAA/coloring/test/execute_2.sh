#!/bin/bash

#
# ./execute.sh 10 10 2 10
#

do_test() 
{
    GENGRAPH="../gengraph/bin"
    VERTEX=$2
    MIN=$3
    MAX=$4
    SAMPLES=$1
    OUTPUT=$6
    EXECUTE=$5

    TEMP="../out/test_distrib$SAMPLES"
    echo "#####>>>>> Generating (pre) $TEMP"
    if $GENGRAPH/distrib $VERTEX 2.5 $MIN $MAX > $TEMP
    then
        for ((i = 0; i < $SAMPLES; i++))
        do
            FILE="../in/test$i"
            echo "#####>>>>> Generating $FILE"
            if $GENGRAPH/graph $TEMP > $FILE
            then
                if [ -e "$FILE" ]; then
                    if $EXECUTE; then
                        if $OUTPUT; then
                            ../bin/tp-final "$FILE" "../out/test$1"
                        else
                           ../bin/tp-final "$FILE"
                        fi
                    fi
                fi
            fi
        done
    fi
}

#do_test 100 10 2 10 true true
#do_test 1000 100 10 100 true false
do_test 100 1000 25 100 true false
