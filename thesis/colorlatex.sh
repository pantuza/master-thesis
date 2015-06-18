#!/bin/bash

#
# Script to install colorlatex
#

PROJECT_URL="git@github.com:vivienm/colorlatex.git";

GIT=$(which git);

[ "$?" == "1" ] && {
    echo -e "Missing git tool";
    exit 1;
}

$GIT clone $PROJECT_URL;

cd colorlatex;

sudo make prefix=/usr install

cd -;

rm -rf colorlatex;

echo -e "colorlatex installed";
