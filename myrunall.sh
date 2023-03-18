#!/bin/sh
cd "$( dirname "$0" )"
cmake ./SDK/c++
make -C ./SDK/c++
./Robot -f ./SDK/c++/main -m maps/1.txt -l ERR
./Robot -f ./SDK/c++/main -m maps/2.txt -l ERR
./Robot -f ./SDK/c++/main -m maps/3.txt -l ERR
./Robot -f ./SDK/c++/main -m maps/4.txt -l ERR