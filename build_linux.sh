#!/bin/sh

mkdir -p build &&
cd build &&
cmake -DUSE_LUA=ON -DUSE_PHYSFS=ON ../ &&
make -j2
