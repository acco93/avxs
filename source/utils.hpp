//
// Created by acco on 11/16/18.
//

#ifndef XSTTRP_UTILS_HPP
#define XSTTRP_UTILS_HPP


#include <string>
#include <vector>

void check_bks_availability(std::vector<std::string> instances, const std::string &bks_file_path);

void print_progress(unsigned long current, unsigned long total, std::string message);

void print_progress_end(std::string message);

#endif //XSTTRP_UTILS_HPP
