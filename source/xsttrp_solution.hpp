//
// Created by acco on 10/16/18.
//

#ifndef XSTTRP_XSTTRP_SOLUTION_HPP
#define XSTTRP_XSTTRP_SOLUTION_HPP

#include <vector>
#include "route.hpp"
#include "xsttrp_instance.hpp"

class xsttrp_wip_solution;

class xsttrp_solution {

private:

    xsttrp_instance instance;
    std::vector<std::vector<route>> routes;

public:

    xsttrp_solution(xsttrp_instance instance, xsttrp_wip_solution& wip_solution);

    xsttrp_solution(const xsttrp_solution& other);

    void print() const;

    void print(std::ofstream& ofstream);

    std::vector<std::vector<route>> get_routes();

    float get_cost();

};


#endif //XSTTRP_XSTTRP_SOLUTION_HPP
