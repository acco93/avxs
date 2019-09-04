//
// Created by acco on 3/1/18.
//

#ifndef GSTTRP_INSTANCE_HPP
#define GSTTRP_INSTANCE_HPP


#include <string>
#include <vector>
#include <cmath>
#include <climits>
#include "globals.hpp"
#include "avxs_parameters.hpp"

enum vertex_type {
    dep,    // depot
    sat,    // satellite
    trk,    // truck customer
    vly,    // vehicle customer yes park
    vln,    // vehicle customer no park
};

enum problem_type {
    xsttrp,
    mdvrp,
    lrp
};

class xsttrp_instance {

public:

    explicit xsttrp_instance(std::string path, unsigned long decimal_precision = 99, bool round_arc_costs=false);

    xsttrp_instance(const xsttrp_instance& other);

    xsttrp_instance& operator=(const xsttrp_instance& other) = delete;

    ~xsttrp_instance();

    inline bool is_customer(const int curr) const { return curr >= customers_begin && curr < customers_end; }
    inline bool is_vehicle_customer_yes_park(const int curr) const { return curr >= vehicle_customers_yes_park_begin && curr < vehicle_customers_yes_park_end; }
    inline bool is_vehicle_customer_no_park(const int curr) const { return curr >= vehicle_customers_no_park_begin && curr < vehicle_customers_no_park_end; }
    inline bool is_satellite(const int curr) const { return curr >= satellites_begin && curr < satellites_end; }
    inline bool is_truck_customer(const int curr) const { return curr >= truck_customers_begin && curr < truck_customers_end; }
    inline bool is_vehicle_customer(const int curr) const { return curr >= vehicle_customers_begin && curr < vehicle_customers_end; }
    inline bool is_parking_location(const int curr) const { return curr >= parking_locations_begin && curr < parking_locations_end; }
    inline bool is_main_level_vertex(const int curr) const { return curr >= vehicle_customers_begin && curr < satellites_end; }

    int truck_customers_num = 0;
    int truck_customers_begin = 0;
    int truck_customers_end = 0;

    int vehicle_customers_no_park_num = 0;
    int vehicle_customers_no_park_begin = 0;
    int vehicle_customers_no_park_end = 0;

    int vehicle_customers_yes_park_num = 0;
    int vehicle_customers_yes_park_begin = 0;
    int vehicle_customers_yes_park_end = 0;

    int satellites_num = 0;
    int satellites_begin = 0;
    int satellites_end = 0;

    int depot = 0;

    int matrix_size = 0;

    int customers_num = 0;
    int customers_begin = 0;
    int customers_end = 0;

    int vehicle_customers_num = 0;
    int vehicle_customers_begin = 0;
    int vehicle_customers_end = 0;

    int parking_locations_num = 0;
    int parking_locations_begin = 0;
    int parking_locations_end = 0;

    int main_level_vertices_num = 0;
    int main_level_vertices_begin = 0;
    int main_level_vertices_end = 0;

    short* q = nullptr;
    vertex_type * vtype = nullptr;
    float** c = nullptr;

    int truck_capacity = 0;
    int trailer_capacity = 0;

    bool is_valid = false;

    std::vector<float> x;
    std::vector<float> y;

    int mdvrp_vehicles_num = INT_MAX;

    problem_type ptype;

    float fixed_satellite_opening_cost = 0.0f;
    float fixed_sub_route_opening_cost = 0.0f;

    std::string path;


private:

    problem_type identify_instance_type(std::string path);
    void parse_xsttrp(std::string path, unsigned int decimal_precision, bool b);
    void parse_mdvrp(std::string path, unsigned int decimal_precision, bool b);
    void parse_lrp(std::string basic_string, unsigned int decimal_precision, bool b);
    void set_precision_and_rounding(unsigned long decimals, bool b);
};



#endif //GSTTRP_INSTANCE_HPP
