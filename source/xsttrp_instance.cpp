//
// Created by acco on 3/1/18.
//

#include "xsttrp_instance.hpp"

#include "mm.hpp"

#include <cmath>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cfloat>


xsttrp_instance::xsttrp_instance(const std::string path, unsigned long decimal_precision, bool round_arc_costs) {

    this->path = path;

    auto type = identify_instance_type(path);

    switch (type) {
        case xsttrp:
            parse_xsttrp(path, decimal_precision, round_arc_costs);
            break;
        case mdvrp:
            parse_mdvrp(path, decimal_precision, round_arc_costs);
            break;
        case lrp:
            parse_lrp(path, decimal_precision, round_arc_costs);
            break;
    }

}

xsttrp_instance::~xsttrp_instance() {

    mm::release_raw_contiguous_memory(c);
    mm::release_raw_contiguous_memory(q);
    mm::release_raw_contiguous_memory(vtype);

}

void xsttrp_instance::parse_xsttrp(std::string path, unsigned int decimal_precision, bool round_arc_costs) {

    try {

        ptype = problem_type::xsttrp;

        std::ifstream stream(path);
        std::string line;
        std::string::size_type sz = 0;

        /* Line 1:
        * truckCustomers satellites_num+1 <vehicle_customers_no_park_num> <vehicle_customers_yes_park_num>
        */

        std::getline(stream, line);
        truck_customers_num = stoi(line, &sz);
        line.erase(0, sz);
        satellites_num = stoi(line, &sz) - 1;
        line.erase(0, sz);
        try {
            vehicle_customers_no_park_num = stoi(line, &sz);
            line.erase(0, sz);
            vehicle_customers_yes_park_num = stoi(line, &sz);
            line.erase(0, sz);
        }
        catch (...) {
            vehicle_customers_no_park_num = 0;
            vehicle_customers_yes_park_num = 0;
        }
        sz = 0;

        /*
        * Vertices layout setup:
        *
        * | -- truck customers -- | -- vehicle customers without parking facility -- | -- vehicle customers with parking facility -- | -- satellites -- | -- depot -- |
        *						   | ==									vehicle customers										   == |
        * | ==										customers																	   == |
        *																			  | ==					parking locations										== |
        */
        truck_customers_begin = 0;
        truck_customers_end = truck_customers_begin + truck_customers_num;

        vehicle_customers_no_park_begin = truck_customers_end;
        vehicle_customers_no_park_end = vehicle_customers_no_park_begin + vehicle_customers_no_park_num;

        vehicle_customers_yes_park_begin = vehicle_customers_no_park_end;
        vehicle_customers_yes_park_end = vehicle_customers_yes_park_begin + vehicle_customers_yes_park_num;

        vehicle_customers_num = vehicle_customers_no_park_num + vehicle_customers_yes_park_num;
        vehicle_customers_begin = vehicle_customers_no_park_begin;
        vehicle_customers_end = vehicle_customers_yes_park_end;

        customers_num = truck_customers_num + vehicle_customers_num;
        customers_begin = truck_customers_begin;
        customers_end = vehicle_customers_end;

        satellites_begin = vehicle_customers_yes_park_end;
        satellites_end = satellites_begin + satellites_num;

        parking_locations_num = vehicle_customers_yes_park_num + satellites_num;
        parking_locations_begin = vehicle_customers_yes_park_begin;
        parking_locations_end = satellites_end;

        main_level_vertices_num = satellites_num + vehicle_customers_num;
        main_level_vertices_begin = vehicle_customers_begin;
        main_level_vertices_end = satellites_end;

        depot = satellites_end;
        matrix_size = depot + 1;

        if(vehicle_customers_num > 0 && ptype == problem_type::mdvrp) {
            throw "Invalid MDVRP instance";
        }

        /*
        * Line 2:
        * truck_capacity trailer_capacity
        */
        std::getline(stream, line);
        truck_capacity = stoi(line, &sz);
        line.erase(0, sz);
        trailer_capacity = stoi(line, &sz);
        line.erase(0, sz);
        sz = 0;

        q = mm::request_raw_contiguous_memory<short>(matrix_size);
        vtype = mm::request_raw_contiguous_memory<vertex_type>(matrix_size);

        x.resize(matrix_size);
        y.resize(matrix_size);

        /*
        * Line 3:
        * xDepot yDepot
        */
        std::getline(stream, line);
        x[depot] = stoi(line, &sz);
        line.erase(0, sz);
        y[depot] = stoi(line, &sz);
        line.erase(0, sz);
        q[depot] = 0;
        sz = 0;
        vtype[depot] = vertex_type::dep;

        /*
        * Lines 4 ... 4 + satellites_num:
        * xSatellite ySatellite
        */
        for (int k = 0; k < satellites_num; k++) {

            const int index = k + satellites_begin;
            std::getline(stream, line);
            x[index] = stoi(line, &sz);
            line.erase(0, sz);
            y[index] = stoi(line, &sz);
            line.erase(0, sz);
            q[index] = 0;
            sz = 0;
            vtype[index] = vertex_type::sat;

        }

        /*
        * Lines satellites_end ... satellites_end + customers_num
        * xTruckCustomer yTruckCustomer qTruckCustomer
        * ...
        * xVehicleCustomerNoPark yVehicleCustomerNoPark qVehicleCustomerNoPark
        * ...
        * xVehicleCustomerYesPark yVehicleCustomerYesPark qVehicleCustomerYesPark
        */
        for (int i = 0; i < customers_num; i++) {

            const int index = i + customers_begin;
            std::getline(stream, line);
            x[index] = stoi(line, &sz);
            line.erase(0, sz);
            y[index] = stoi(line, &sz);
            line.erase(0, sz);
            q[index] = static_cast<short>(stoi(line, &sz));
            line.erase(0, sz);
            sz = 0;

        }

        for(auto i = truck_customers_begin; i < truck_customers_end; i++){
            vtype[i] = vertex_type::trk;
        }
        for(auto i = vehicle_customers_no_park_begin; i < vehicle_customers_no_park_end; i++){
            vtype[i] = vertex_type::vln;
        }
        for(auto i = vehicle_customers_yes_park_begin; i < vehicle_customers_yes_park_end; i++){
            vtype[i] = vertex_type::vly;
        }

        c = mm::request_raw_contiguous_memory<float>(matrix_size, matrix_size);

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                c[i][j] = static_cast<float>(pow(pow((x[i] - x[j]), 2.0) + pow((y[i] - y[j]), 2.0), 0.5));
            }
        }

        set_precision_and_rounding(decimal_precision, round_arc_costs);

        is_valid = true;



    }
    catch (std::exception& e) {

        is_valid = false;
    }

}

void xsttrp_instance::parse_mdvrp(std::string path, unsigned int decimal_precision, bool round_arc_costs) {

    try {

        ptype = problem_type::mdvrp;

        std::ifstream stream(path);
        std::string line;
        std::string::size_type sz = 0;

        std::getline(stream, line);
        const auto ptype = stoi(line, &sz);
        if(ptype != 2) {
            throw "Not a mdvrp instance!";
        }
        assert(ptype == 2);
        line.erase(0, sz);
        
        mdvrp_vehicles_num = stoi(line, &sz);
        line.erase(0, sz);
        
        truck_customers_num = stoi(line, &sz);
        line.erase(0, sz);
        
        satellites_num = stoi(line, &sz);
        line.erase(0, sz);
        
        vehicle_customers_no_park_num = 0;
        vehicle_customers_yes_park_num = 0;



        sz = 0;

        /*
        * Vertices layout setup:
        *
        * | -- truck customers -- | -- vehicle customers without parking facility -- | -- vehicle customers with parking facility -- | -- satellites -- | -- depot -- |
        *						   | ==									vehicle customers										   == |
        * | ==										customers																	   == |
        *																			  | ==					parking locations										== |
        */
        truck_customers_begin = 0;
        truck_customers_end = truck_customers_begin + truck_customers_num;

        vehicle_customers_no_park_begin = truck_customers_end;
        vehicle_customers_no_park_end = vehicle_customers_no_park_begin + vehicle_customers_no_park_num;

        vehicle_customers_yes_park_begin = vehicle_customers_no_park_end;
        vehicle_customers_yes_park_end = vehicle_customers_yes_park_begin + vehicle_customers_yes_park_num;

        vehicle_customers_num = vehicle_customers_no_park_num + vehicle_customers_yes_park_num;
        vehicle_customers_begin = vehicle_customers_no_park_begin;
        vehicle_customers_end = vehicle_customers_yes_park_end;

        customers_num = truck_customers_num + vehicle_customers_num;
        customers_begin = truck_customers_begin;
        customers_end = vehicle_customers_end;

        satellites_begin = vehicle_customers_yes_park_end;
        satellites_end = satellites_begin + satellites_num;

        parking_locations_num = vehicle_customers_yes_park_num + satellites_num;
        parking_locations_begin = vehicle_customers_yes_park_begin;
        parking_locations_end = satellites_end;

        main_level_vertices_num = satellites_num + vehicle_customers_num;
        main_level_vertices_begin = vehicle_customers_begin;
        main_level_vertices_end = satellites_end;

        depot = satellites_end;
        matrix_size = depot + 1;

        std::getline(stream, line);
        stoi(line, &sz);
        line.erase(0, sz);
        truck_capacity = stoi(line, &sz);
        line.erase(0, sz);
        sz = 0;
        trailer_capacity = INT_MAX;

        for(auto i = 1; i < satellites_num; i++){
            std::getline(stream, line);
        }

        q = mm::request_raw_contiguous_memory<short>(matrix_size);
        vtype = mm::request_raw_contiguous_memory<vertex_type>(matrix_size);

        x.resize(matrix_size);
        y.resize(matrix_size);

        x[depot] = 0;
        y[depot] = 0;
        q[depot] = 0;
        vtype[depot] = vertex_type::dep;

        for (int i = 0; i < truck_customers_num; i++) {

            const int index = i + truck_customers_begin;

            std::getline(stream, line);
            // vertex num
            stoi(line, &sz);
            line.erase(0, sz);

            x[index] = stoi(line, &sz);
            line.erase(0, sz);

            y[index] = stoi(line, &sz);
            line.erase(0, sz);

            // service duration
            stoi(line, &sz);
            line.erase(0, sz);

            q[index] = static_cast<short>(stoi(line, &sz));
            line.erase(0, sz);
            sz = 0;

            vtype[index] = vertex_type::trk;
        }



        /*
        * Lines 4 ... 4 + satellites_num:
        * xSatellite ySatellite
        */
        for (int k = 0; k < satellites_num; k++) {



            const int index = k + satellites_begin;

            std::getline(stream, line);
            // vertex num

            stoi(line, &sz);
            line.erase(0, sz);

            x[index] = stoi(line, &sz);
            line.erase(0, sz);

            y[index] = stoi(line, &sz);
            line.erase(0, sz);

            q[index] = 0;
            sz = 0;

            vtype[index] = vertex_type::sat;

        }


        c = mm::request_raw_contiguous_memory<float>(matrix_size, matrix_size);

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                c[i][j] = static_cast<float>(pow(pow((x[i] - x[j]), 2.0) + pow((y[i] - y[j]), 2.0), 0.5));
            }
        }


        for(auto s = satellites_begin; s < matrix_size; s++) {
            for(auto k = satellites_begin; k < matrix_size; k++){
                c[s][k] = 0.0f;
            }
        }

        set_precision_and_rounding(decimal_precision, round_arc_costs);

        is_valid = true;



    }
    catch (std::exception& e) {

        is_valid = false;
    }


}

void xsttrp_instance::parse_lrp(std::string basic_string, unsigned int decimal_precision, bool round_arc_costs) {

    try {

        ptype = problem_type::lrp;

        std::ifstream stream(basic_string);
        std::string line;
        std::string::size_type sz = 0;


        std::getline(stream, line);
        truck_customers_num = stoi(line, &sz);
        line.erase(0, sz);


        std::getline(stream, line);
        sz = 0;
        satellites_num = stoi(line, &sz);
        line.erase(0, sz);

        // empty line
        std::getline(stream, line);
        sz = 0;

        vehicle_customers_no_park_num = 0;
        vehicle_customers_yes_park_num = 0;

        /*
        * Vertices layout setup:
        *
        * | -- truck customers -- | -- vehicle customers without parking facility -- | -- vehicle customers with parking facility -- | -- satellites -- | -- depot -- |
        *						   | ==									vehicle customers										   == |
        * | ==										customers																	   == |
        *																			  | ==					parking locations										== |
        */
        truck_customers_begin = 0;
        truck_customers_end = truck_customers_begin + truck_customers_num;

        vehicle_customers_no_park_begin = truck_customers_end;
        vehicle_customers_no_park_end = vehicle_customers_no_park_begin + vehicle_customers_no_park_num;

        vehicle_customers_yes_park_begin = vehicle_customers_no_park_end;
        vehicle_customers_yes_park_end = vehicle_customers_yes_park_begin + vehicle_customers_yes_park_num;

        vehicle_customers_num = vehicle_customers_no_park_num + vehicle_customers_yes_park_num;
        vehicle_customers_begin = vehicle_customers_no_park_begin;
        vehicle_customers_end = vehicle_customers_yes_park_end;

        customers_num = truck_customers_num + vehicle_customers_num;
        customers_begin = truck_customers_begin;
        customers_end = vehicle_customers_end;

        satellites_begin = vehicle_customers_yes_park_end;
        satellites_end = satellites_begin + satellites_num;

        parking_locations_num = vehicle_customers_yes_park_num + satellites_num;
        parking_locations_begin = vehicle_customers_yes_park_begin;
        parking_locations_end = satellites_end;

        main_level_vertices_num = satellites_num + vehicle_customers_num;
        main_level_vertices_begin = vehicle_customers_begin;
        main_level_vertices_end = satellites_end;

        depot = satellites_end;
        matrix_size = depot + 1;

        q = mm::request_raw_contiguous_memory<short>(matrix_size);
        vtype = mm::request_raw_contiguous_memory<vertex_type>(matrix_size);

        x.resize(matrix_size);
        y.resize(matrix_size);

        x[depot] = 0;
        y[depot] = 0;
        q[depot] = 0;
        vtype[depot] = vertex_type::dep;


        for (int k = 0; k < satellites_num; k++) {

            const int index = k + satellites_begin;
            std::getline(stream, line);
            x[index] = stof(line, &sz);
            line.erase(0, sz);
            y[index] = stof(line, &sz);
            line.erase(0, sz);
            q[index] = 0;
            sz = 0;
            vtype[index] = vertex_type::sat;

        }

        for (int i = 0; i < truck_customers_num; i++) {

            const int index = i + customers_begin;
            std::getline(stream, line);
            x[index] = stof(line, &sz);
            line.erase(0, sz);
            y[index] = stof(line, &sz);
            line.erase(0, sz);
            sz = 0;
            vtype[index] = vertex_type::trk;

        }

        // empty line
        std::getline(stream, line);
        sz = 0;

        std::getline(stream, line);
        truck_capacity = stoi(line, &sz);
        line.erase(0, sz);

        // empty line
        std::getline(stream, line);
        sz = 0;

        for(auto k = 0; k < satellites_num; k++){
            std::getline(stream, line);
        }

        // empty line
        std::getline(stream, line);
        sz = 0;

        for (int i = 0; i < truck_customers_num; i++) {

            const int index = i + customers_begin;
            std::getline(stream, line);
            q[index] = static_cast<short>(stoi(line, &sz));
            line.erase(0, sz);
            sz = 0;

        }

        // empty line
        std::getline(stream, line);
        sz = 0;

        std::getline(stream, line);
        fixed_satellite_opening_cost = stof(line, &sz);
        line.erase(0, sz);
        sz = 0;

        for(auto k = 1; k < satellites_num; k++){
            std::getline(stream, line);
        }

        // empty line
        std::getline(stream, line);
        sz = 0;

        std::getline(stream, line);
        fixed_sub_route_opening_cost = stof(line, &sz);
        line.erase(0, sz);
        sz = 0;

        c = mm::request_raw_contiguous_memory<float>(matrix_size, matrix_size);

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                //c[i][j] = static_cast<float>(pow(pow((x[i] - x[j]), 2.0) + pow((y[i] - y[j]), 2.0), 0.5));
                c[i][j] = std::sqrt((x[i] - x[j]) * (x[i]-x[j]) + (y[i] - y[j])*(y[i] - y[j]));
            }
        }

        for(auto s = satellites_begin; s < matrix_size; s++) {
            for(auto k = satellites_begin; k < matrix_size; k++){
                c[s][k] = 0.0f;
            }
        }

        float value = fixed_satellite_opening_cost / 2.0f;

        for(auto k = satellites_begin; k < satellites_end; k ++) {
            c[k][depot] = value;
            c[depot][k] = value;
        }

        for(auto k = satellites_begin; k < satellites_end - 1; k++){
            for(auto s = k + 1; s < satellites_end; s++){
                c[k][s] = fixed_satellite_opening_cost;
                c[s][k] = fixed_satellite_opening_cost;
            }
        }

        value = fixed_sub_route_opening_cost / 2.0f;

        for(auto i = truck_customers_begin; i < truck_customers_end; i++) {
            for(auto k = satellites_begin; k < satellites_end; k++){
                c[i][k] += value;
                c[k][i] += value;
            }
        }

        set_precision_and_rounding(decimal_precision, round_arc_costs);

        is_valid = true;



    }
    catch (std::exception& e) {

        is_valid = false;
    }

}

problem_type xsttrp_instance::identify_instance_type(const std::string path) {


    std::ifstream stream(path);
    std::string line;
    std::string::size_type sz = 0;

    std::getline(stream, line);
    std::istringstream iss(line);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    if(results.size() == 1) {
        return lrp;
    }

    if(results.size() < 4) {
        return xsttrp;
    }

    stoi(line, &sz);
    line.erase(0, sz);

    stoi(line, &sz);
    line.erase(0, sz);

    stoi(line, &sz);
    line.erase(0, sz);

    const auto first_number = stoi(line, &sz);


    for(auto n = 0; n < first_number; n++){
        std::getline(stream, line);
    }

    std::getline(stream, line);
    std::istringstream iss2(line);
    std::vector<std::string> results2((std::istream_iterator<std::string>(iss2)), std::istream_iterator<std::string>());

    if(results2.size() > 4) {
        return mdvrp;
    } else {
        return xsttrp;
    }

}

void xsttrp_instance::set_precision_and_rounding(unsigned long decimals, bool round_arc_costs) {

    if(decimals < 32) {

        const auto factor = static_cast<float>(std::pow(10, decimals));

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                c[i][j] = std::floor(factor * c[i][j]) / factor;
            }
        }

    }


    if(round_arc_costs) {

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                c[i][j] = std::round(c[i][j]);
            }
        }

    }

}

xsttrp_instance::xsttrp_instance(const xsttrp_instance &other) {

     truck_customers_num = other.truck_customers_num;
     truck_customers_begin = other.truck_customers_begin;
     truck_customers_end = other.truck_customers_end;

     vehicle_customers_no_park_num = other.vehicle_customers_no_park_num;
     vehicle_customers_no_park_begin = other.vehicle_customers_no_park_begin;
     vehicle_customers_no_park_end = other.vehicle_customers_no_park_end;

     vehicle_customers_yes_park_num = other.vehicle_customers_yes_park_num;
     vehicle_customers_yes_park_begin = other.vehicle_customers_yes_park_begin;
     vehicle_customers_yes_park_end = other.vehicle_customers_yes_park_end;

     satellites_num = other.satellites_num;
     satellites_begin = other.satellites_begin;
     satellites_end = other.satellites_end;

     depot = other.depot;

     matrix_size = other.matrix_size;

     customers_num = other.customers_num;
     customers_begin = other.customers_begin;
     customers_end = other.customers_end;

     vehicle_customers_num = other.vehicle_customers_num;
     vehicle_customers_begin = other.vehicle_customers_begin;
     vehicle_customers_end = other.vehicle_customers_end;

     parking_locations_num = other.parking_locations_num;
     parking_locations_begin = other.parking_locations_begin;
     parking_locations_end = other.parking_locations_end;

     main_level_vertices_num = other.main_level_vertices_begin;
     main_level_vertices_begin = other.main_level_vertices_begin;
     main_level_vertices_end = other.main_level_vertices_end;

    q = mm::request_raw_contiguous_memory<short>(matrix_size);
    vtype = mm::request_raw_contiguous_memory<vertex_type>(matrix_size);
    c = mm::request_raw_contiguous_memory<float>(matrix_size, matrix_size);

    for (int i = 0; i < matrix_size; i++) {
        q[i] = other.q[i];
        vtype[i] = other.vtype[i];
        for (int j = 0; j < matrix_size; j++) {
            c[i][j] = other.c[i][j];
        }
    }

    truck_capacity = other.truck_capacity;
    trailer_capacity = other.trailer_capacity;

    is_valid = other.is_valid;

     x = other.x;
    y = other.y;

     mdvrp_vehicles_num = other.mdvrp_vehicles_num;

     ptype = other.ptype;

     fixed_satellite_opening_cost = other.fixed_satellite_opening_cost;
     fixed_sub_route_opening_cost = other.fixed_sub_route_opening_cost;

    path = other.path;

}


