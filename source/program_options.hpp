//
// Created by acco on 11/1/18.
//

#ifndef XSTTRP_OTHER_PARAMETERS_HPP
#define XSTTRP_OTHER_PARAMETERS_HPP

#include <string>
#include <vector>

struct program_options {

    unsigned long runs = 1;
    std::vector<std::string> paths;
    bool random_log_file = true;
    std::string log_file;
    unsigned int costs_decimals = 99;
    unsigned int round_results = 2;
    bool fixed_seed = false;
    unsigned long seed;
    std::string bks_path = "../bks.txt";
    bool tex_picture = false;
    bool round_arc_costs = false;
};

#endif //XSTTRP_OTHER_PARAMETERS_HPP
