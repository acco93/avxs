#!/usr/bin/env bash

if ! [ -x "$(command -v make)" ]; then
  echo 'Error: make is not installed. Needed to build the executable.' >&2
  exit 1
fi

if ! [ -x "$(command -v cmake)" ]; then
  echo 'Error: cmake is not installed. Needed to build the executable.' >&2
  exit 1
fi

if ! [ -x "$(command -v pdftk)" ]; then
  echo 'Error: pdftk is not installed. Needed to create pdf files.' >&2
  exit 1
fi

if ! [ -x "$(command -v pdflatex)" ]; then
  echo 'Error: pdflatex is not installed. Needed to compile tex files.' >&2
  exit 1
fi



instances_path=../instances

(cd ../source/ && cmake . )
(cd ../source/ && cmake --build . --target all -- -j 4 )
(rm -r ../source/CMakeFiles && rm ../source/CMakeCache.txt && rm ../source/cmake_install.cmake && rm ../source/Makefile && mv ../source/avxs .)

########################################################################################################################

./avxs --log mdvrp.txt --seed 0 --runs 10 --tex-picture --files ${instances_path}/mdvrp/p01 ${instances_path}/mdvrp/p02 ${instances_path}/mdvrp/p03 ${instances_path}/mdvrp/p04 ${instances_path}/mdvrp/p05 ${instances_path}/mdvrp/p06 ${instances_path}/mdvrp/p07 ${instances_path}/mdvrp/p12 ${instances_path}/mdvrp/p15 ${instances_path}/mdvrp/p18 ${instances_path}/mdvrp/p21
./avxs --log lrp.txt --seed 0 --runs 10 --tex-picture --files ${instances_path}/lrp_tuzun/coordP111112.dat ${instances_path}/lrp_tuzun/coordP111122.dat ${instances_path}/lrp_tuzun/coordP111212.dat ${instances_path}/lrp_tuzun/coordP111222.dat ${instances_path}/lrp_tuzun/coordP112112.dat ${instances_path}/lrp_tuzun/coordP112122.dat ${instances_path}/lrp_tuzun/coordP112212.dat ${instances_path}/lrp_tuzun/coordP112222.dat ${instances_path}/lrp_tuzun/coordP113112.dat ${instances_path}/lrp_tuzun/coordP113122.dat ${instances_path}/lrp_tuzun/coordP113212.dat ${instances_path}/lrp_tuzun/coordP113222.dat ${instances_path}/lrp_tuzun/coordP131112.dat ${instances_path}/lrp_tuzun/coordP131122.dat ${instances_path}/lrp_tuzun/coordP131212.dat ${instances_path}/lrp_tuzun/coordP131222.dat ${instances_path}/lrp_tuzun/coordP132112.dat ${instances_path}/lrp_tuzun/coordP132122.dat ${instances_path}/lrp_tuzun/coordP132212.dat ${instances_path}/lrp_tuzun/coordP132222.dat ${instances_path}/lrp_tuzun/coordP133112.dat ${instances_path}/lrp_tuzun/coordP133122.dat ${instances_path}/lrp_tuzun/coordP133212.dat ${instances_path}/lrp_tuzun/coordP133222.dat ${instances_path}/lrp_tuzun/coordP121112.dat ${instances_path}/lrp_tuzun/coordP121122.dat ${instances_path}/lrp_tuzun/coordP121212.dat ${instances_path}/lrp_tuzun/coordP121222.dat ${instances_path}/lrp_tuzun/coordP122112.dat ${instances_path}/lrp_tuzun/coordP122122.dat ${instances_path}/lrp_tuzun/coordP122212.dat ${instances_path}/lrp_tuzun/coordP122222.dat ${instances_path}/lrp_tuzun/coordP123112.dat ${instances_path}/lrp_tuzun/coordP123122.dat ${instances_path}/lrp_tuzun/coordP123212.dat ${instances_path}/lrp_tuzun/coordP123222.dat
./avxs --log sttrpsd.txt --seed 0 --runs 10 --tex-picture --files ${instances_path}/STTRPSD/STTRP-25-5-1-c.dat ${instances_path}/STTRPSD/STTRP-25-5-2-c.dat ${instances_path}/STTRPSD/STTRP-25-5-1-rd.dat ${instances_path}/STTRPSD/STTRP-25-5-2-rd.dat ${instances_path}/STTRPSD/STTRP-25-10-1-c.dat ${instances_path}/STTRPSD/STTRP-25-10-2-c.dat ${instances_path}/STTRPSD/STTRP-25-10-1-rd.dat ${instances_path}/STTRPSD/STTRP-25-10-2-rd.dat ${instances_path}/STTRPSD/STTRP-50-5-1-c.dat ${instances_path}/STTRPSD/STTRP-50-5-2-c.dat ${instances_path}/STTRPSD/STTRP-50-5-1-rd.dat ${instances_path}/STTRPSD/STTRP-50-5-2-rd.dat ${instances_path}/STTRPSD/STTRP-50-10-1-c.dat ${instances_path}/STTRPSD/STTRP-50-10-2-c.dat ${instances_path}/STTRPSD/STTRP-50-10-1-rd.dat ${instances_path}/STTRPSD/STTRP-50-10-2-rd.dat ${instances_path}/STTRPSD/STTRP-100-10-1-c.dat ${instances_path}/STTRPSD/STTRP-100-10-2-c.dat ${instances_path}/STTRPSD/STTRP-100-10-1-rd.dat ${instances_path}/STTRPSD/STTRP-100-10-2-rd.dat ${instances_path}/STTRPSD/STTRP-100-20-1-c.dat ${instances_path}/STTRPSD/STTRP-100-20-2-c.dat ${instances_path}/STTRPSD/STTRP-100-20-1-rd.dat ${instances_path}/STTRPSD/STTRP-100-20-2-rd.dat ${instances_path}/STTRPSD/STTRP-200-10-1-c.dat ${instances_path}/STTRPSD/STTRP-200-10-2-c.dat ${instances_path}/STTRPSD/STTRP-200-10-1-rd.dat ${instances_path}/STTRPSD/STTRP-200-10-2-rd.dat ${instances_path}/STTRPSD/STTRP-200-20-1-c.dat ${instances_path}/STTRPSD/STTRP-200-20-2-c.dat ${instances_path}/STTRPSD/STTRP-200-20-1-rd.dat ${instances_path}/STTRPSD/STTRP-200-20-2-rd.dat
./avxs --log bartolini.txt --seed 0 --tex-picture --runs 10 --precision 5 --round-results 1 --files ${instances_path}/bartolini/chao_1.30.dat ${instances_path}/bartolini/chao_2.30.dat ${instances_path}/bartolini/chao_3.30.dat ${instances_path}/bartolini/chao_4.30.dat ${instances_path}/bartolini/chao_5.30.dat ${instances_path}/bartolini/chao_6.30.dat ${instances_path}/bartolini/chao_7.30.dat ${instances_path}/bartolini/chao_8.30.dat ${instances_path}/bartolini/chao_9.30.dat ${instances_path}/bartolini/chao_10.30.dat ${instances_path}/bartolini/chao_11.30.dat ${instances_path}/bartolini/chao_12.30.dat ${instances_path}/bartolini/tai_1.30.dat ${instances_path}/bartolini/tai_2.30.dat ${instances_path}/bartolini/tai_3.30.dat ${instances_path}/bartolini/tai_4.30.dat ${instances_path}/bartolini/tai_5.30.dat ${instances_path}/bartolini/tai_6.30.dat ${instances_path}/bartolini/tai_7.30.dat ${instances_path}/bartolini/tai_8.30.dat ${instances_path}/bartolini/tai_9.30.dat ${instances_path}/bartolini/tai_10.30.dat ${instances_path}/bartolini/tai_11.30.dat ${instances_path}/bartolini/tai_12.30.dat ${instances_path}/bartolini/chao_1.40.dat ${instances_path}/bartolini/chao_2.40.dat ${instances_path}/bartolini/chao_3.40.dat ${instances_path}/bartolini/chao_4.40.dat ${instances_path}/bartolini/chao_5.40.dat ${instances_path}/bartolini/chao_6.40.dat ${instances_path}/bartolini/chao_7.40.dat ${instances_path}/bartolini/chao_8.40.dat ${instances_path}/bartolini/chao_9.40.dat ${instances_path}/bartolini/chao_10.40.dat ${instances_path}/bartolini/chao_11.40.dat ${instances_path}/bartolini/chao_12.40.dat
./avxs --log sttrp.txt --seed 0 --tex-picture --runs 10 --files ${instances_path}/chao/TTRP_01.txt.sv ${instances_path}/chao/TTRP_02.txt.sv ${instances_path}/chao/TTRP_03.txt.sv ${instances_path}/chao/TTRP_04.txt.sv ${instances_path}/chao/TTRP_05.txt.sv ${instances_path}/chao/TTRP_06.txt.sv ${instances_path}/chao/TTRP_07.txt.sv ${instances_path}/chao/TTRP_08.txt.sv ${instances_path}/chao/TTRP_09.txt.sv ${instances_path}/chao/TTRP_10.txt.sv ${instances_path}/chao/TTRP_11.txt.sv ${instances_path}/chao/TTRP_12.txt.sv ${instances_path}/chao/TTRP_13.txt.sv ${instances_path}/chao/TTRP_14.txt.sv ${instances_path}/chao/TTRP_15.txt.sv ${instances_path}/chao/TTRP_16.txt.sv ${instances_path}/chao/TTRP_17.txt.sv ${instances_path}/chao/TTRP_18.txt.sv ${instances_path}/chao/TTRP_19.txt.sv ${instances_path}/chao/TTRP_20.txt.sv ${instances_path}/chao/TTRP_21.txt.sv
./avxs --log xsttrp.txt --seed 0 --tex-picture --runs 10 --files ${instances_path}/xsttrp/xsttrp1 ${instances_path}/xsttrp/xsttrp2 ${instances_path}/xsttrp/xsttrp3 ${instances_path}/xsttrp/xsttrp4 ${instances_path}/xsttrp/xsttrp5 ${instances_path}/xsttrp/xsttrp6 ${instances_path}/xsttrp/xsttrp7 ${instances_path}/xsttrp/xsttrp8 ${instances_path}/xsttrp/xsttrp9 ${instances_path}/xsttrp/xsttrp10 ${instances_path}/xsttrp/xsttrp11 ${instances_path}/xsttrp/xsttrp12 ${instances_path}/xsttrp/xsttrp13 ${instances_path}/xsttrp/xsttrp14 ${instances_path}/xsttrp/xsttrp15 ${instances_path}/xsttrp/xsttrp16 ${instances_path}/xsttrp/xsttrp17 ${instances_path}/xsttrp/xsttrp18 ${instances_path}/xsttrp/xsttrp19 ${instances_path}/xsttrp/xsttrp20 ${instances_path}/xsttrp/xsttrp21 ${instances_path}/xsttrp/xsttrp22 ${instances_path}/xsttrp/xsttrp23 ${instances_path}/xsttrp/xsttrp24 ${instances_path}/xsttrp/xsttrp25 ${instances_path}/xsttrp/xsttrp26 ${instances_path}/xsttrp/xsttrp27 ${instances_path}/xsttrp/xsttrp28 ${instances_path}/xsttrp/xsttrp29 ${instances_path}/xsttrp/xsttrp30 ${instances_path}/xsttrp/xsttrp31 ${instances_path}/xsttrp/xsttrp32 ${instances_path}/xsttrp/xsttrp33 ${instances_path}/xsttrp/xsttrp34 ${instances_path}/xsttrp/xsttrp35 ${instances_path}/xsttrp/xsttrp36 ${instances_path}/xsttrp/xsttrp37 ${instances_path}/xsttrp/xsttrp38 ${instances_path}/xsttrp/xsttrp39 ${instances_path}/xsttrp/xsttrp40 ${instances_path}/xsttrp/xsttrp41 ${instances_path}/xsttrp/xsttrp42 ${instances_path}/xsttrp/xsttrp43 ${instances_path}/xsttrp/xsttrp44 ${instances_path}/xsttrp/xsttrp45 ${instances_path}/xsttrp/xsttrp46 ${instances_path}/xsttrp/xsttrp47 ${instances_path}/xsttrp/xsttrp48 ${instances_path}/xsttrp/xsttrp49 ${instances_path}/xsttrp/xsttrp50 ${instances_path}/xsttrp/xsttrp51 ${instances_path}/xsttrp/xsttrp52 ${instances_path}/xsttrp/xsttrp53 ${instances_path}/xsttrp/xsttrp54 ${instances_path}/xsttrp/xsttrp55 ${instances_path}/xsttrp/xsttrp56 ${instances_path}/xsttrp/xsttrp57 ${instances_path}/xsttrp/xsttrp58 ${instances_path}/xsttrp/xsttrp59 ${instances_path}/xsttrp/xsttrp60 ${instances_path}/xsttrp/xsttrp61 ${instances_path}/xsttrp/xsttrp62 ${instances_path}/xsttrp/xsttrp63 ${instances_path}/xsttrp/xsttrp64 ${instances_path}/xsttrp/xsttrp65 ${instances_path}/xsttrp/xsttrp66 ${instances_path}/xsttrp/xsttrp67 ${instances_path}/xsttrp/xsttrp68 ${instances_path}/xsttrp/xsttrp69 ${instances_path}/xsttrp/xsttrp70 ${instances_path}/xsttrp/xsttrp71 ${instances_path}/xsttrp/xsttrp72 ${instances_path}/xsttrp/xsttrp73 ${instances_path}/xsttrp/xsttrp74 ${instances_path}/xsttrp/xsttrp75 ${instances_path}/xsttrp/xsttrp76 ${instances_path}/xsttrp/xsttrp77

find ${instances_path} -name '*.tex'|while read file; do mv ${file} .; done;

find -name "*.tex" | while read file; do pdflatex --enable-write18 -interaction=nonstopmode "$file"; done


find . -name "*fig1.pdf" -type f -delete
find . -name "*.tex" -type f -delete
find . -name "*.ps" -type f -delete
find . -name "*.log" -type f -delete
find . -name "*.aux" -type f -delete
find . -name "tmp.inputs" -type f -delete

pdftk p*.pdf cat output mdvrp.pdf
find . -name "p*.pdf" -type f -delete

pdftk coordP*.pdf cat output lrp.pdf
find . -name "coordP*.pdf" -type f -delete

pdftk STTRP*.pdf cat output sttrpsd.pdf
find . -name "STTRP*.pdf" -type f -delete

pdftk chao*.pdf tai*.pdf cat output sttrp_bartolini.pdf
find . -name "chao*.pdf" -type f -delete
find . -name "tai*.pdf" -type f -delete

pdftk TTRP*.pdf cat output sttrp.pdf
find . -name "TTRP*.pdf" -type f -delete

pdftk xsttrp*.pdf cat output xsttrp.pdf
find . -name "xsttrp*.pdf" -type f -delete

rm avxs
