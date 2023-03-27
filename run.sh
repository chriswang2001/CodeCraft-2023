#!/bin/sh
cd "$( dirname "$0" )"

cmake ./SDK/c++
make

./Robot -f ./main -m maps/$1.txt