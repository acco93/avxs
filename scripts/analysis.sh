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

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)

./avxs --log rank1.txt --rank 1 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log rank2.txt --rank 2 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log rank3.txt --rank 3 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log rank4.txt --rank 4 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log rank5.txt --rank 5 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log near1.txt --near 1 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log near2.txt --near 2 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log near5.txt --near 5 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log near10.txt --near 10 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
./avxs --log near25.txt --near 25 --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77

rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_VND=ON . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_vnd.txt --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_STATS=ON . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_rnei_shake.txt --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
rm avxs

########################################################################################################################

(cd ../source/ && cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_NGRANULAR=ON . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)
./avxs --log xsttrp_no_granular.txt --seed 0 --runs 10 --files ../instances/xsttrp/xsttrp1 ../instances/xsttrp/xsttrp2 ../instances/xsttrp/xsttrp3 ../instances/xsttrp/xsttrp4 ../instances/xsttrp/xsttrp5 ../instances/xsttrp/xsttrp6 ../instances/xsttrp/xsttrp7 ../instances/xsttrp/xsttrp8 ../instances/xsttrp/xsttrp9 ../instances/xsttrp/xsttrp10 ../instances/xsttrp/xsttrp11 ../instances/xsttrp/xsttrp12 ../instances/xsttrp/xsttrp13 ../instances/xsttrp/xsttrp14 ../instances/xsttrp/xsttrp15 ../instances/xsttrp/xsttrp16 ../instances/xsttrp/xsttrp17 ../instances/xsttrp/xsttrp18 ../instances/xsttrp/xsttrp19 ../instances/xsttrp/xsttrp20 ../instances/xsttrp/xsttrp21 ../instances/xsttrp/xsttrp22 ../instances/xsttrp/xsttrp23 ../instances/xsttrp/xsttrp24 ../instances/xsttrp/xsttrp25 ../instances/xsttrp/xsttrp26 ../instances/xsttrp/xsttrp27 ../instances/xsttrp/xsttrp28 ../instances/xsttrp/xsttrp29 ../instances/xsttrp/xsttrp30 ../instances/xsttrp/xsttrp31 ../instances/xsttrp/xsttrp32 ../instances/xsttrp/xsttrp33 ../instances/xsttrp/xsttrp34 ../instances/xsttrp/xsttrp35 ../instances/xsttrp/xsttrp36 ../instances/xsttrp/xsttrp37 ../instances/xsttrp/xsttrp38 ../instances/xsttrp/xsttrp39 ../instances/xsttrp/xsttrp40 ../instances/xsttrp/xsttrp41 ../instances/xsttrp/xsttrp42 ../instances/xsttrp/xsttrp43 ../instances/xsttrp/xsttrp44 ../instances/xsttrp/xsttrp45 ../instances/xsttrp/xsttrp46 ../instances/xsttrp/xsttrp47 ../instances/xsttrp/xsttrp48 ../instances/xsttrp/xsttrp49 ../instances/xsttrp/xsttrp50 ../instances/xsttrp/xsttrp51 ../instances/xsttrp/xsttrp52 ../instances/xsttrp/xsttrp53 ../instances/xsttrp/xsttrp54 ../instances/xsttrp/xsttrp55 ../instances/xsttrp/xsttrp56 ../instances/xsttrp/xsttrp57 ../instances/xsttrp/xsttrp58 ../instances/xsttrp/xsttrp59 ../instances/xsttrp/xsttrp60 ../instances/xsttrp/xsttrp61 ../instances/xsttrp/xsttrp62 ../instances/xsttrp/xsttrp63 ../instances/xsttrp/xsttrp64 ../instances/xsttrp/xsttrp65 ../instances/xsttrp/xsttrp66 ../instances/xsttrp/xsttrp67 ../instances/xsttrp/xsttrp68 ../instances/xsttrp/xsttrp69 ../instances/xsttrp/xsttrp70 ../instances/xsttrp/xsttrp71 ../instances/xsttrp/xsttrp72 ../instances/xsttrp/xsttrp73 ../instances/xsttrp/xsttrp74 ../instances/xsttrp/xsttrp75 ../instances/xsttrp/xsttrp76 ../instances/xsttrp/xsttrp77
rm avxs
