//
// Created by acco on 10/16/18.
//
// Some helper procedures definition
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include "avxs.hpp"
#include "timer.hpp"

void use_d_near_assignment_probability_function(const xsttrp_instance& instance, int **candidate_list, float **candidate_list_probabilities, unsigned long d) {

    const auto num = instance.parking_locations_num + 1; /* +1 for the depot */

    for (auto i = instance.customers_begin; i < instance.customers_end; i++) {

        for (auto kLoc = instance.parking_locations_begin, n = 0; kLoc < instance.matrix_size; kLoc++, n++) {
            candidate_list[i][n] = kLoc;
        }

        std::sort(candidate_list[i], candidate_list[i] + num, [i, &instance](int k1, int k2) -> bool { return instance.c[i][k1] < instance.c[i][k2]; });

        for (auto k = 0; k < num; k++) {
            candidate_list_probabilities[i][k] = 0.0f;
        }

    }


    for(auto i = instance.truck_customers_begin; i < instance.truck_customers_end; i++) {
        auto remaining = d;
        for(auto n = 0; n < num && remaining; n++) {
            const auto kLoc = candidate_list[i][n];
            if(kLoc != instance.depot) {    // truck customers cannot be assigned to the depot!
                candidate_list_probabilities[i][n] = 1.0f;
                remaining--;
            }
        }
    }

    for(auto i = instance.vehicle_customers_no_park_begin; i < instance.vehicle_customers_no_park_end; i++) {
        auto remaining = d;
        for(auto n = 0; n < num && remaining; n++) {
            candidate_list_probabilities[i][n] = 1.0f;
            remaining--;
        }
    }

    for(auto i = instance.vehicle_customers_yes_park_begin; i < instance.vehicle_customers_yes_park_end; i++){
        auto remaining = d;
        for(auto n = 0; n < num && remaining; n++) {
            const auto kLoc = candidate_list[i][n];
            if(kLoc != i) {                 // avoid auto assignment
                candidate_list_probabilities[i][n] = 1.0f;
                remaining--;
            }
        }
    }

}

void use_b_rank_assignment_probability_function(const xsttrp_instance& instance, int **candidate_list, float **candidate_list_probabilities, unsigned long b) {

    /*
    * Define the customer to satellite assignment probability. Given a customer i, sort the parking locations according to their
    * distance to i. The probability of assigning i to satellite k that in the sorted array has position n is n * n + 1.
    * This will be used in the customer to satellites grasp assignment.
    */
    for (auto i = instance.customers_begin; i < instance.customers_end; i++) {

        for (auto kLoc = instance.parking_locations_begin, n = 0; kLoc < instance.matrix_size; kLoc++, n++) {
            candidate_list[i][n] = kLoc;
        }

        std::sort(candidate_list[i], candidate_list[i] + instance.parking_locations_num + 1 /* the depot */,
                  [i, &instance](int k1, int k2) -> bool { return instance.c[i][k1] > instance.c[i][k2]; });
        for (auto k = 0; k < instance.parking_locations_num; k++) {
            const auto v = static_cast<float>(k);
            candidate_list_probabilities[i][k] = static_cast<float>(std::pow(v, b) + 1.0f);
        }

    }


    // remove depot assignment for truck customers
    for (auto i = instance.truck_customers_begin; i < instance.truck_customers_end; i++) {
        for (auto n = 0; n <= instance.parking_locations_num; n++) {
            const auto kLoc = candidate_list[i][n];
            if (kLoc == instance.depot) {
                candidate_list_probabilities[i][n] = 0.0f;
                break;
            }
        }
    }

    // avoid auto assignments
    for (auto i = instance.vehicle_customers_yes_park_begin; i < instance.vehicle_customers_yes_park_end; i++) {
        for (auto n = 0; n <= instance.parking_locations_num; n++) {
            const auto kLoc = candidate_list[i][n];
            if (kLoc == i) {
                candidate_list_probabilities[i][n] = 0.0f;
                break;
            }
        }
    }

}

void arcs_definition(const xsttrp_instance &instance, std::vector<arc> &sub_arcs, std::vector<arc> &main_arcs) {

    const auto sub_arcs_num = instance.matrix_size * (instance.matrix_size - 1);
    sub_arcs.resize(sub_arcs_num);

    auto n = 0;
    for (auto i = instance.customers_begin; i < instance.matrix_size - 1; i++) {
        for (auto j = i + 1; j < instance.matrix_size; j++) {
            sub_arcs[n].i = i;
            sub_arcs[n].j = j;
            sub_arcs[n].cost = instance.c[i][j];
            n++;
            sub_arcs[n].i = j;
            sub_arcs[n].j = i;
            sub_arcs[n].cost = instance.c[j][i];
            n++;
        }
    }
    std::stable_sort(sub_arcs.begin(), sub_arcs.end(), [](const arc &a, const arc &b) -> bool { return a.cost < b.cost; });

    const auto main_vertices_and_depot_num = instance.main_level_vertices_num + 1;
    const auto main_arcs_num = main_vertices_and_depot_num * (main_vertices_and_depot_num - 1);
    main_arcs.resize(main_arcs_num);

    n = 0;
    for(auto i = instance.vehicle_customers_begin; i < instance.matrix_size -1; i++){
        for(auto j = i+1; j < instance.matrix_size; j++){
            main_arcs[n].i = i;
            main_arcs[n].j = j;
            main_arcs[n].cost = instance.c[i][j];
            n++;
            main_arcs[n].i = j;
            main_arcs[n].j = i;
            main_arcs[n].cost = instance.c[i][j];
            n++;
        }
    }
    std::stable_sort(main_arcs.begin(), main_arcs.end(), [](const arc &a, const arc &b) -> bool { return a.cost < b.cost; });

}

