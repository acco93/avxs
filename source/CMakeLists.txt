cmake_minimum_required(VERSION 3.10)
project(avxs)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(OPT_FLAGS "-O3 -march=native -fwhole-program -flto")
set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wuninitialized")
set(SANITIZERS_FLAGS "-fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -fsanitize=leak")

set(CPLEX_INC_DIR   "/opt/ibm/ILOG/CPLEX_Studio128/cplex/include")
set(CPLEX_LIB_DIR   "/opt/ibm/ILOG/CPLEX_Studio128/cplex/lib/x86-64_linux/static_pic")
set(CONCERT_INC_DIR "/opt/ibm/ILOG/CPLEX_Studio128/concert/include")
set(CONCERT_LIB_DIR "/opt/ibm/ILOG/CPLEX_Studio128/concert/lib/x86-64_linux/static_pic")

OPTION(ENABLE_STATS "Enable statistics logging" OFF)                    #disabled by defauls
IF(ENABLE_STATS)
    ADD_DEFINITIONS(-DSTATS)
ENDIF(ENABLE_STATS)

OPTION(ENABLE_VND "Use standard VND without randomization" OFF)         #disabled by defauls
IF(ENABLE_VND)
    ADD_DEFINITIONS(-DVND)
ENDIF(ENABLE_VND)

OPTION(ENABLE_NGRANULAR "Use complete neighborhood exploration" OFF)    #disabled by defauls
IF(ENABLE_NGRANULAR)
    ADD_DEFINITIONS(-DNGRANULAR)
ENDIF(ENABLE_NGRANULAR)

OPTION(ENABLE_VERBOSE "Print very detailed results" OFF)                #disabled by defauls
IF(ENABLE_VERBOSE)
    ADD_DEFINITIONS(-DVERBOSE)
ENDIF(ENABLE_VERBOSE)

OPTION(ENABLE_LONGER_IMPROVEMENT "Enable longer improvement phases" OFF)                #disabled by defauls
IF(ENABLE_LONGER_IMPROVEMENT)
    ADD_DEFINITIONS(-DLONGER_IMPROVEMENT)
ENDIF(ENABLE_LONGER_IMPROVEMENT)

OPTION(ENABLE_OPERATOR_RELOCATE "Enable the relocate operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_OPERATOR_RELOCATE)
    ADD_DEFINITIONS(-DOPERATOR_RELOCATE)
ENDIF(ENABLE_OPERATOR_RELOCATE)

OPTION(ENABLE_OPERATOR_SWAP "Enable the swap operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_OPERATOR_SWAP)
    ADD_DEFINITIONS(-DOPERATOR_SWAP)
ENDIF(ENABLE_OPERATOR_SWAP)

OPTION(ENABLE_OPERATOR_TWOPT "Enable the twopt operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_OPERATOR_TWOPT)
    ADD_DEFINITIONS(-DOPERATOR_TWOPT)
ENDIF(ENABLE_OPERATOR_TWOPT)

OPTION(ENABLE_OPERATOR_SEGSWAP "Enable the segswap operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_OPERATOR_SEGSWAP)
    ADD_DEFINITIONS(-DOPERATOR_SEGSWAP)
ENDIF(ENABLE_OPERATOR_SEGSWAP)

OPTION(ENABLE_OPERATOR_ROOTREF "Enable the rootref operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_OPERATOR_ROOTREF)
    ADD_DEFINITIONS(-DOPERATOR_ROOTREF)
ENDIF(ENABLE_OPERATOR_ROOTREF)

OPTION(ENABLE_POLISHING_PHASE "Enable the rootref operator during the local search phase" ON)                #enabled by defauls
IF(ENABLE_POLISHING_PHASE)
    ADD_DEFINITIONS(-DPOLISHING_PHASE)
ENDIF(ENABLE_POLISHING_PHASE)

OPTION(ENABLE_BEST_RNEI_ORDER "Order the operators according to the RNEI" OFF)                                #enabled by defauls
IF(ENABLE_BEST_RNEI_ORDER)
    ADD_DEFINITIONS(-DBEST_RNEI_ORDER)
ENDIF(ENABLE_BEST_RNEI_ORDER)

# Add the following directives to add_definitions to change the algorithm behaviour
# -DSTATS                       to enable statistics logging
# -DVND                         to use a standard VND without randomization
# -DNGRANULAR                   to use complete neighborhood exploration
# -DVERBOSE                     in combination with -DSTATS, to print very detailed results such as each complete solution (as sequence of vertices),
#                               rnei values for each instance, average solutions evolution (max: 10000 iterations) as set of points
#                               (iteration, gap) and (time, gap). NB: gap is average over all instances and all runs.
# -DLONGER_IMPROVEMENT          to use delta = 1000 iterations instead of delta = 100
# -DOPERATOR_XXX                where XXX can be: RELOCATE, SWAP, TWOPT, SEGSWAP, ROOTREF to enable the operators in the local search execution (all enabled by default)
# -DPOLISHING                   enable the post-optimization polishing phase
# -BEST_RNEI_ORDER              order the neighborhoods according to their RNEI (to be used in combination with -DVND)
# recompile to commit the changes
add_definitions(-DIL_STD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${SANITIZERS_FLAGS} -pg -no-pie")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS} ")

include_directories(SYSTEM ${CPLEX_INC_DIR} ${CONCERT_INC_DIR})

link_directories( ${CPLEX_LIB_DIR} ${CONCERT_LIB_DIR}  )

set(CommonLibs ilocplex cplex concert pthread dl)

add_executable(avxs
        main.cpp
        mm.hpp
        xsttrp_instance.hpp
        xsttrp_instance.cpp
        route.hpp
        route.cpp
        globals.hpp
        routes_slab.hpp
        routes_slab.cpp
        xsttrp_wip_solution.cpp
        xsttrp_wip_solution.hpp
        xsttrp_wip_solution_ls_procedures.cpp
        xsttrp_wip_solution_h_procedures.cpp
        set_partitioning_pool.cpp
        set_partitioning_pool.hpp
        avxs.hpp
        avxs.cpp
        avxs_h_procedures.cpp
        timer.hpp
        avxs_cplex_procedures.cpp
        xsttrp_solution.cpp
        xsttrp_solution.hpp
        arc.hpp
        route_type.hpp
        avxs_result.cpp
        avxs_result.hpp
        avxs_parameters.hpp
        cmdline_arguments_parser.cpp
        cmdline_arguments_parser.hpp
        program_options.hpp
        latex_drawer.cpp
        latex_drawer.hpp
        utils.hpp
        avxs_result_analyzer.cpp
        utils.cpp)

target_link_libraries(avxs ${CommonLibs})
