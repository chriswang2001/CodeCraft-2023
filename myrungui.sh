cd "$( dirname "$0" )"
cmake ./SDK/c++
make -C ./SDK/c++
./Robot_gui -d ./SDK/c++/main -m maps/1.txt