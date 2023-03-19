#!/bin/sh
cd "$( dirname "$0" )"
cmake ./SDK/c++
make -C ./SDK/c++
./Robot -f ./SDK/c++/main -m maps/$1.txt