Stable version of the `AVXS` algorithm described in the paper.

#### Table of contents
<!--ts-->
   * [Introduction](#introduction)
   * [Software](#software)
   * [Directories and files](#directories-and-files)
   * [Requirements](#requirements)
   * [Build options](#build-options)
   * [Usage](#usage)
   * [License](#license)
<!--te-->

#### Introduction
Truck and trailer routing problems is a very well studied category of vehicle routing problems in which vehicle capacities may be augmented with trailers. The literature contains several variations that enrich such basic setting by adding particular constraints to better model specific scenarios.


In this paper, we study the Extended Single Truck and Trailer Routing Problem which generalizes a set of existing truck and trailer routing problems by including a variety of vertex types previously independently considered in different problem variants: namely, truck customers, vehicle customers with and without parking facility, and pure parking locations.


The XSTTRP consists in servicing a set of customers with known demand by using a single vehicle composed of a capacitated truck and a non-autonomous and detachable trailer that is initially located at a main depot.


The customers are partitioned in two different sets: *truck customers* and *vehicle customers*.


Truck customers must be visited by the truck only, while vehicle customers can be served either by the whole vehicle or just by the truck.


Vehicle customers are, in turn, split into vehicle customers *with parking facility* and vehicle customers *without parking facility*.


The problem contains an additional set of *satellite depots*, which are pure parking locations and may be possibly visited to park the trailer whenever necessary.


An XSTTRP solution is made up by a *main-route*, traveled by the complete vehicle, which starts from the main depot, visits a subset of vehicle customers and satellite depots and returns to the depot.


At each visited parking location (i.e., either satellite depots or vehicle customers with parking facility) the vehicle can detach its trailer, serve a subset of customers with the truck and return to the parking location to pick-up the trailer. We call this a *sub-route* and the place where the trailer has been decoupled the *root* of the sub-route.


The objective is to serve all customers in a way that minimize the total traveling cost, while  respecting the truck capacity along the sub-routes and taking into account the accessibility constraints.


We developed a comprehensive, still effective, heuristic solution approach for this class of problems. The proposed algorithm is extensively tested  on XSTTRP instances, as well as on some known problems which arise as special cases of the model, such as the Multiple Depot Vehicle Routing Problem, the Location Routing Problem, the Single Truck and Trailer Routing Problem with Satellite Depots and the Single Truck and Trailer Routing Problem.

#### Software
- `avxs` algorithm [Stable version](https://github.com/acco93/avxs/archive/master.zip) [Active repository](#)
- `xmm` XSTTRP map maker [Active repository](https://github.com/acco93/xmm)

#### Directories and files
The stable version of `AVXS` contains the following directories and files
- `build` contains the algorithm executable once compiled. It is automatically generated if removed or not present.
- `instances` contains the instances used to test the algorithm and a readme that describes the supported file formats. 
- `results` contains the log for the results as published in the paper.
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

To successfully execute `scripts/run.sh`
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
It is necessary to rebuild the program to commit the changes e.g. `cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_VND=ON && cmake --build . --target all -- -j 4`


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

#### License
GNU GENERAL PUBLIC LICENSE
Version 3, 29 June 2007
