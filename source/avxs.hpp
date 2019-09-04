//
// Created by acco on 10/16/18.
//

#ifndef XSTTRP_AVXS_HPP
#define XSTTRP_AVXS_HPP

#include "xsttrp_instance.hpp"
#include "xsttrp_wip_solution.hpp"
#include "avxs_result.hpp"
#include "program_options.hpp"

//Solve the given instance using the AVXS algorithm with the given parameters
avxs_result avxs(const xsttrp_instance &instance, unsigned long seed, avxs_parameters& parameters);

// Bulk analysis of results
void analyze(std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list, avxs_parameters& parameters, program_options& options);

#endif //XSTTRP_AVXS_HPP
