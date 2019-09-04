//
// Created by acco on 11/1/18.
//

#ifndef XSTTRP_CMDLINE_ARGUMENTS_PARSER_HPP
#define XSTTRP_CMDLINE_ARGUMENTS_PARSER_HPP


#include <vector>
#include "avxs_parameters.hpp"
#include "program_options.hpp"

class cmdline_arguments_parser {

    program_options options;
    avxs_parameters parameters;

    void print_usage();
    void print_avxs_parameters();

public:

    cmdline_arguments_parser(int argc, char* argv[]);

    avxs_parameters get_avxs_parameters();
    program_options get_program_options();

    void print_program_options();
};


#endif //XSTTRP_CMDLINE_ARGUMENTS_PARSER_HPP
