#!/usr/bin/env bash

if ! [ -x "$(command -v make)" ]; then
  echo 'Error: make is not installed. Needed to build the executable.' >&2
  exit 1
fi

if ! [ -x "$(command -v cmake)" ]; then
  echo 'Error: cmake is not installed. Needed to build the executable.' >&2
  exit 1
fi

instances_path=../instances
instances="${instances_path}/xsttrp/xsttrp1 ${instances_path}/xsttrp/xsttrp2 ${instances_path}/xsttrp/xsttrp3 ${instances_path}/xsttrp/xsttrp4 ${instances_path}/xsttrp/xsttrp5 ${instances_path}/xsttrp/xsttrp6 ${instances_path}/xsttrp/xsttrp7 ${instances_path}/xsttrp/xsttrp8 ${instances_path}/xsttrp/xsttrp9 ${instances_path}/xsttrp/xsttrp10 ${instances_path}/xsttrp/xsttrp11 ${instances_path}/xsttrp/xsttrp12 ${instances_path}/xsttrp/xsttrp13 ${instances_path}/xsttrp/xsttrp14 ${instances_path}/xsttrp/xsttrp15 ${instances_path}/xsttrp/xsttrp16 ${instances_path}/xsttrp/xsttrp17 ${instances_path}/xsttrp/xsttrp18 ${instances_path}/xsttrp/xsttrp19 ${instances_path}/xsttrp/xsttrp20 ${instances_path}/xsttrp/xsttrp21 ${instances_path}/xsttrp/xsttrp22 ${instances_path}/xsttrp/xsttrp23 ${instances_path}/xsttrp/xsttrp24 ${instances_path}/xsttrp/xsttrp25 ${instances_path}/xsttrp/xsttrp26 ${instances_path}/xsttrp/xsttrp27 ${instances_path}/xsttrp/xsttrp28 ${instances_path}/xsttrp/xsttrp29 ${instances_path}/xsttrp/xsttrp30 ${instances_path}/xsttrp/xsttrp31 ${instances_path}/xsttrp/xsttrp32 ${instances_path}/xsttrp/xsttrp33 ${instances_path}/xsttrp/xsttrp34 ${instances_path}/xsttrp/xsttrp35 ${instances_path}/xsttrp/xsttrp36 ${instances_path}/xsttrp/xsttrp37 ${instances_path}/xsttrp/xsttrp38 ${instances_path}/xsttrp/xsttrp39 ${instances_path}/xsttrp/xsttrp40 ${instances_path}/xsttrp/xsttrp41 ${instances_path}/xsttrp/xsttrp42 ${instances_path}/xsttrp/xsttrp43 ${instances_path}/xsttrp/xsttrp44 ${instances_path}/xsttrp/xsttrp45 ${instances_path}/xsttrp/xsttrp46 ${instances_path}/xsttrp/xsttrp47 ${instances_path}/xsttrp/xsttrp48 ${instances_path}/xsttrp/xsttrp49 ${instances_path}/xsttrp/xsttrp50 ${instances_path}/xsttrp/xsttrp51 ${instances_path}/xsttrp/xsttrp52 ${instances_path}/xsttrp/xsttrp53 ${instances_path}/xsttrp/xsttrp54 ${instances_path}/xsttrp/xsttrp55 ${instances_path}/xsttrp/xsttrp56 ${instances_path}/xsttrp/xsttrp57 ${instances_path}/xsttrp/xsttrp58 ${instances_path}/xsttrp/xsttrp59 ${instances_path}/xsttrp/xsttrp60 ${instances_path}/xsttrp/xsttrp61 ${instances_path}/xsttrp/xsttrp62 ${instances_path}/xsttrp/xsttrp63 ${instances_path}/xsttrp/xsttrp64 ${instances_path}/xsttrp/xsttrp65 ${instances_path}/xsttrp/xsttrp66 ${instances_path}/xsttrp/xsttrp67 ${instances_path}/xsttrp/xsttrp68 ${instances_path}/xsttrp/xsttrp69 ${instances_path}/xsttrp/xsttrp70 ${instances_path}/xsttrp/xsttrp71 ${instances_path}/xsttrp/xsttrp72 ${instances_path}/xsttrp/xsttrp73 ${instances_path}/xsttrp/xsttrp74 ${instances_path}/xsttrp/xsttrp75 ${instances_path}/xsttrp/xsttrp76 ${instances_path}/xsttrp/xsttrp77"

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_RELOCATE=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_relocate.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_SWAP=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_swap.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_TWOPT=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_twopt.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_SEGSWAP=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_segswap.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_ROOTREF=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_rootref.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_OPERATOR_RELOCATE=OFF -DENABLE_OPERATOR_SWAP=OFF -DENABLE_OPERATOR_TWOPT=OFF -DENABLE_OPERATOR_SEGSWAP=OFF -DENABLE_OPERATOR_ROOTREF=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_only_shaking.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_POLISHING_PHASE=OFF . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_polishing.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_VND=ON -DENABLE_BEST_RNEI_ORDER=ON. )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_vnd_best_rnei_order.txt --seed 0 --runs 10 --files $instances 
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON -DENABLE_LONGER_IMPROVEMENT=ON . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_longer_improvement.txt --seed 0 --runs 10 --files $instances 
rm avxs


