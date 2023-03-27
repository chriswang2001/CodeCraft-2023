#!/bin/sh
cd "$( dirname "$0" )"

cmake ./SDK/c++
make

./Robot -f ./main -m maps/1.txt -l ERR
./Robot -f ./main -m maps/2.txt -l ERR
./Robot -f ./main -m maps/3.txt -l ERR
./Robot -f ./main -m maps/4.txt -l ERR