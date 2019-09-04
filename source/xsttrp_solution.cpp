//
// Created by acco on 10/16/18.
//

#include <iostream>
#include <fstream>
#include <cfloat>
#include "xsttrp_solution.hpp"
#include "xsttrp_wip_solution.hpp"

xsttrp_solution::xsttrp_solution(const xsttrp_instance p_instance, xsttrp_wip_solution& wip_solution) : instance(p_instance) {

    routes.resize(instance.parking_locations_num+1);

    for(auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for(const auto& route : wip_solution.get_routes_from(k)) {
            routes[k].push_back(*route);
        }
    }

}

void xsttrp_solution::print() const {

    auto cost = 0.0f;
    if(!routes[instance.parking_locations_num].empty()) {
        routes[instance.parking_locations_num].front().print();
        cost += routes[instance.parking_locations_num].front().cost();
    }

    for (int k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            route.print();
            cost += route.cost();
        }
    }
    std::cout <<"Cost = "<< cost << std::endl;

}

xsttrp_solution::xsttrp_solution(const xsttrp_solution &other) : instance(other.instance) {

    routes.resize(instance.parking_locations_num+1);
    for(auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for(const auto& route : other.routes.at(k)) {
            routes.at(k).push_back(route);
        }
    }

}

/*xsttrp_solution &xsttrp_solution::operator=(const xsttrp_solution &other) {
    for(auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for(const auto& route : other.routes[k]) {
            routes[k].push_back(route);
        }
    }
    return *this;
}*/

void xsttrp_solution::print(std::ofstream& ofstream) {

    auto cost = 0.0f;
    routes[instance.parking_locations_num].front().print(ofstream);
    cost += routes[instance.parking_locations_num].front().cost();
    for (int k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            route.print(ofstream);
            cost += route.cost();
        }
    }
    ofstream <<"Cost = "<< cost << std::endl;

}

std::vector<std::vector<route>> xsttrp_solution::get_routes() {
    return routes;
}

float xsttrp_solution::get_cost() {



    auto num = 0;

    auto cost = 0.0f;
    if(!routes[instance.parking_locations_num].empty()) {
        // it could be empty in case of infeasible solutions
        cost += routes[instance.parking_locations_num].front().cost();
        num++;
    }

    for (int k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            cost += route.cost();
            num++;
        }
    }

    if(num == 0) {
        cost = FLT_MAX;
    }

    return cost;
}
