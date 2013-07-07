#!/bin/bash

#
# Test script that run the program to each input file placed at
# "in" directory and places its output inside "out" directory
#
# author: Gustavo Pantuza
# since: 27.06.2013
#

EXEC=bin/tp3

for file in $(ls in | sort);
do
    echo -ne "running ${EXEC} for in/$file with \
output out/$(echo $file | cut -d. -f1).out";
   
    ./${EXEC} in/$file out/$(echo $file | cut -d. -f1).out;
    echo "";
done;
