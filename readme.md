Stable version of the `AVXS` algorithm described in the paper.

#### Directories and files
- `build` contains the algorithm executable once compiled. It is automatically generated if removed or not present.
- `instances` contains the instances used to test the algorithm and a readme that describes the supported file formats. 
- `scripts` contains a set of utility scripts to build the source code and to reproduce the published results.
 All the scripts assume that the directories structure is the one define in this readme. 
  - `analysis.sh` performs some analysis as in the Analysis section of the paper. It automatically takes care to compile the code with the required directives and run it with the required input parameters.
  - `build.sh` compiles the code from the source directory and outputs an executable in the build directory.
  - `cmake_install.sh` installs cmake (the minimum required version is defined in `source/CMakeLists.txt`).
  - `run.sh` performs the experiments as published in the paper. It automatically takes care to compile the code with the required directives and run it with the required input parameters.
- `source` contains the algorithm source code in the `source` directory and produces an executable in the `build` directory.
- `bks.txt` contains the list of best known solution values for the considered instances. It is used to compute the gap within the algorithm.

#### Requirements
The following programs are needed either to build the code or to perform some extra tasks such as creating pdf files with a graphical representation of the solutions.

To correctly compile and build the executable and to execute `scripts/build.sh` are required
- `cmake` >= 3.10
- `g++` >= 6.3.0*
- `cplex` >= 12.8*

To successfully execute scripts/run.sh
- `cmake` >= 3.10
- `g++` >= 6.3.0*
- `cplex` >= 12.8*
- `pdftk`
- `pdflatex`


\* suggested despite not mandatory. Previous recent versions *should* be usable too.

#### Build options
You can provide the following optional cmake options.
- `ENABLE_STATS` to enable detailed statistics.
- `ENABLE_VERBOSE` to enable very detailed results such as each solution representation and if `ENABLE_STATS` is enabled, the partial evolution in terms of 2D points `(iteration, % gap)` of the algorithm averaged over all instances and runs.
- `ENABLE_VND` to disable randomization in the improvement phase.
- `ENABLE_NGRANULAR` to use complete neighborhood exploration instead of granular neighborhoods.

The directives must be given using `-DENABLE_XXXX=ON`. See the example below.
It is necessary to rebuild the program to commit the changes e.g. `cmake -DENABLE_VND=ON && cmake --build . --target all -- -j 4`


#### Usage
The algorithm requires a unique mandatory argument
- `files PATH1 PATH2 ... PATHN` where each `PATH` identifies an instance to be processed

You can provide the following optional command line arguments
- `help` shows the help and exit
- `runs N` integer value, it tells the algorithm to execute `N` runs for every instance. Default is `1`.
- `seed N` integer value, specifies the seed. In case of multiple runs, it is used as a base seed and the real seed is computed as `seed + run`. Default is `std::random_device`.
- `bks PATH` specifies where to find the file with the best known solution values. Default is `../bks.txt`.
- `near N` integer value, it forces the algorithm to use the `N-near` assignment fitness function during the assignment phase. Default `1-near` for instances with fixed costs and `5-near` otherwise.
- `rank R` real value, it forces the algorithm to use the `R-rank` assignment fitness function during the assignment phase. Default `1-near` for instances with fixed costs and `5-near` otherwise.
- `log PATH` specifies where to store the log. Default is a randomly named file in the current directory.
- `precision N` integer value, it specifies to use `N` decimal places to use in computing the arcs cost. The numbers are truncated thus, if using too few places, undefined behaviour might happen. Default is standard floating point precision.
- `round-results N` integer value, it specifies to use `N` decimal places in showing the results. Default is `2`.
- `tex-picture` for each instance and for each run a .tex file corresponding to the graphical representation of the solution is generated.

All arguments must be provided with `--`, see the example below. Argument can be given in any order.

`./avxs --files ../instances/xsttrp/xsttrp25 ../instances/mdvrp/p18 --runs 5 --tex-pictures --log file.txt`
