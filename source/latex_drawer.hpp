//
// Created by acco on 11/9/18.
//

#ifndef XSTTRP_LATEX_DRAWER_HPP
#define XSTTRP_LATEX_DRAWER_HPP


#include "xsttrp_instance.hpp"
#include "xsttrp_solution.hpp"

class latex_drawer {

    static void satellite(std::ofstream& stream, float x, float y);
    static void depot(std::ofstream &ofstream, float x, float y);
    static void truck_customer(std::ofstream &ofstream, float x, float y);
    static void vehicle_customer_no_park(std::ofstream &ofstream, float x, float y);
    static void vehicle_customer_yes_park(std::ofstream &ofstream, float x, float y);


public:
    static void draw(xsttrp_instance& instance, xsttrp_solution& solution, int run);

    static float map(float value, float min, float max);

    static void line(std::ofstream &ofstream, float x1, float y1, float x2, float y2);

    static void dashed(std::ofstream &stream, float x1, float y1, float x2, float y2);
};


#endif //XSTTRP_LATEX_DRAWER_HPP
