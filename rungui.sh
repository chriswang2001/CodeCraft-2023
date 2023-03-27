#!/bin/sh
cd "$( dirname "$0" )"

cmake ./SDK/c++
make

./Robot_gui -d ./main -m maps/$1.txt
