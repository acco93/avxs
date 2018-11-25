#!/usr/bin/env bash
(cd ../source/ && cmake . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake &&
 rm ../source/Makefile &&  rm -r ../build && mkdir -p ../build && mv ../source/avxs ../build)
echo "\n\n\n\n\n"
echo "============================================="
echo "Go to ../build/ to find the avxs executable"


