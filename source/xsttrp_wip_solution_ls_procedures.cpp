//
// Created by acco on 7/23/18.
//

#include <algorithm>
#include <set>
#include <cfloat>
#include "xsttrp_wip_solution.hpp"

// because they are forward declared in xsttrp_wip_solution.hpp
#include "routes_slab.hpp"


void xsttrp_wip_solution::shake_up() {


    shake_type = (shake_type + 1) % 2;


    if(shake_type == 0){

        #ifdef STATS
        add_shake = &avxs_result::add_sunload;
        #endif

        auto route_load_sum = 0;
        auto routes_num = 0;

        for(auto k = 0; k < instance.parking_locations_num; k++){
            for(auto route : routes[k]) {
                route_load_sum += route->load();
                routes_num++;
            }
        }

        const auto route_load_avg = static_cast<float>(route_load_sum) / static_cast<float>(routes_num);

        // Shake strategy 1

        const auto load_th = (ZETA_SUNLOAD_MIN + (rand_engine() % ZETA_SUNLOAD_MAX)) / 100.0f * route_load_avg;

        for(int i : customers_permutation) {

            // skip customers served by route1
            // they will be treated subsequently
            if(routes[instance.parking_locations_num].front()->contains(i)){
                continue;
            }

            const auto route = route_pointers[i];

            if(route->load() > load_th) {
                remove_vertex(route, i);
                unrouted_customers.push_back(i);
            }


        }


    } else {


        #ifdef STATS
        add_shake = &avxs_result::add_srem;
        #endif


        // Shake strategy 2
        open_parking_locations.clear();

        identify_open_parking_locations(open_parking_locations);

        if(open_parking_locations.empty()) {
            return;
        }

        knuth_shuffle(open_parking_locations, rand_engine);

        auto random_park_num = rand_engine() % open_parking_locations.size();

        if(random_park_num == 0) {
            random_park_num = 1;
        }

        for(auto n = 0u; n < random_park_num; n++) {

            const auto kLoc = open_parking_locations[n];
            const auto k = kLoc - instance.parking_locations_begin;

            auto random_routes_num = 0ul;

            if(instance.is_satellite(kLoc)) {

                // the mdvrp contains only satellites and there is no gain in reducing
                // the main-route so we avoid to close the satellite with 100% probability
                if(instance.ptype != mdvrp /*&& instance.parking_locations_num != 1*/) {

                    random_routes_num = routes[k].size();

                } else {

                    random_routes_num = rand_engine() % routes[k].size();

                    if(random_routes_num == 0){
                        random_routes_num = 1;
                    }

                }



            } else {

                random_routes_num = rand_engine() % routes[k].size();

                if(random_routes_num == 0){
                    random_routes_num = 1;
                }

                if(random_routes_num == routes[k].size()) {

                    unrouted_customers.push_back(kLoc);
                    vertex_served_by_route_type[kLoc] = route_type::none;
                    route_pointers[kLoc] = nullptr;
                    routes[instance.parking_locations_num].front()->remove(kLoc);

                    unrouted_customers.push_back(kLoc);

                } else {

                    routes[k].sort([](route* a, route* b) -> bool {
                        return a->size() > b->size();
                    });

                }



            }

            auto curr_iterator = routes[k].begin();

            for(auto l = 0u; l < random_routes_num; l++) {

                const auto next_iterator = std::next(curr_iterator);

                const auto route = *curr_iterator;

                remove_all_customers_from_route(route, unrouted_customers);

                (this->*remove_route)(route, curr_iterator);

                curr_iterator = next_iterator;


            }

        }


    }


    if(instance.vehicle_customers_num != 0){

        const auto main_route = routes[instance.parking_locations_num].front();

        auto curr = main_route->get_first_vertex();

        while(curr != instance.depot) {

            const auto next = main_route->next(curr);

            if(instance.is_parking_location(curr) && !routes[curr - instance.parking_locations_begin].empty()) {
                // skip root vertices
                curr = next;
                continue;
            }

            const auto r = static_cast<double>(rand_engine()) / static_cast<double>(std::mt19937::max());

            if(r < ETA_MREM) {

                unrouted_customers.push_back(curr);
                vertex_served_by_route_type[curr] = route_type::none;
                route_pointers[curr] = nullptr;
                main_route->remove(curr);

            }

            curr = next;

        }

    }

    const auto order = rand_engine() % 5;

    switch (order) {
        case 0:
        std::sort(unrouted_customers.begin(), unrouted_customers.end(), [this](int i, int j)->bool {
            return instance.x[i] < instance.x[j];
        });
            break;
        case 1:
            std::sort(unrouted_customers.begin(), unrouted_customers.end(), [this](int i, int j)->bool {
                return instance.x[i] > instance.x[j];
            });
            break;
        case 2:
            std::sort(unrouted_customers.begin(), unrouted_customers.end(), [this](int i, int j)->bool {
                return instance.y[i] < instance.y[j];
            });
            break;
        case 3:
            std::sort(unrouted_customers.begin(), unrouted_customers.end(), [this](int i, int j)->bool {
                return instance.y[i] > instance.y[j];
            });
            break;
        case 4:
            knuth_shuffle(unrouted_customers, rand_engine);
            break;
        default:
            break;
    }

    (this->*reset_main_route_insertion_cache)();

    for(const auto i : unrouted_customers) {

        if(vertex_served_by_route_type[i] != route_type::none) {
            continue;
        }

        perform_best_insertion(i);

    }


    unrouted_customers.clear();

    cost_is_up_to_date = false;






}

void xsttrp_wip_solution::local_search_move_1_0_eval_route_insertion(int i,
                                                                 int j,
                                                                 float &best_cost,
                                                                 int &best_customer_position,
                                                                 bool &best_in_new_one_customer_route,
                                                                 bool &best_needed_main_route_update,
                                                                 route_iterator &best_route_iter) {



    const auto jRoute_iter = route_iterators[j];
    const auto jRoute = *jRoute_iter;

    const auto capacity = jRoute->get_type() == route_type::main_route? INT_MAX : instance.truck_capacity;

    if(jRoute->load() + instance.q[i] > capacity) {
        return;
    }

    const auto jPrev = jRoute->prev(j);

    const auto cost = - instance.c[jPrev][j]
                      + instance.c[jPrev][i]
                      + instance.c[i][j];

    if(cost < best_cost) {

        best_cost = cost;
        best_customer_position = j;
        best_in_new_one_customer_route = false;
        best_needed_main_route_update = false;
        best_route_iter = jRoute_iter;

    }

}

void xsttrp_wip_solution::local_search_move_1_0_eval_park_insertion(int i, int kLoc, float &best_cost,
                                                                int &best_customer_position,
                                                                bool &best_in_new_one_customer_route,
                                                                bool &best_needed_main_route_update,
                                                                xsttrp_wip_solution::route_iterator &best_route_iter,
                                                                int &best_park,
                                                                int &best_park_position) {



    const auto k = kLoc - instance.parking_locations_begin;

    for(auto kIter = routes[k].begin(); kIter != routes[k].end(); ++kIter) {

        const auto route = *kIter;

        if(route->load() + instance.q[i] > instance.truck_capacity) {
            continue;
        }

        const auto kPrev = route->get_last_vertex();

        const auto cost = - instance.c[kPrev][kLoc]
                          + instance.c[kPrev][i]
                          + instance.c[i][kLoc];

        if(cost < best_cost) {

            best_cost = cost;
            best_customer_position = kLoc;
            best_in_new_one_customer_route = false;
            best_needed_main_route_update = false;
            best_route_iter = kIter;

        }

    }

    // as a new one-customer sub-route

    {


        const auto route_excess = static_cast<int>(routes[k].size()) - instance.mdvrp_vehicles_num;

        auto route_excess_cost = 0.0f;
        if(route_excess > 0) {
            route_excess_cost += route_excess * MDVRP_ROUTE_EXCESS_PENALTY;
        }



        auto park_position = -1;
        auto needed_main_route_update = false;


        const auto cost0 = 2.0f * instance.c[kLoc][i] + (this->*get_park_opening_cost)(kLoc, park_position, needed_main_route_update) + route_excess_cost /*+ instance.lrp_sub_route_opening_cost*/;


        if(cost0 < best_cost) {

            best_cost = cost0;
            best_park = kLoc;
            best_in_new_one_customer_route = true;
            best_needed_main_route_update = needed_main_route_update;
            best_park_position = park_position;

        }

    }

}

bool xsttrp_wip_solution::local_search_move_1_0(){


    bool improved = false;

    (this->*reset_main_route_insertion_cache)();

    auto main_route = routes[instance.parking_locations_num].front();

    __repeat__:

    for(const auto i : customers_permutation) {

        // skip routes root
        if(instance.is_vehicle_customer_yes_park(i) && !routes[i - instance.parking_locations_begin].empty()) {
            continue;
        }

        auto best_route_iter = route_iterators[i];
        const auto current_route = *best_route_iter;
        auto best_park = current_route->begin();
        auto best_customer_position = current_route->next(i);
        auto best_in_new_one_customer_route = current_route->size() == 2;
        auto best_park_position = main_route->next(best_park);
        auto best_needed_main_route_update = instance.is_satellite(best_park) &&
                                             routes[best_park - instance.parking_locations_begin].size() == 1 &&
                                             instance.ptype != problem_type::mdvrp;

        const auto i_was_in_main_route = current_route == main_route;

        const auto current_cost = -remove_vertex(current_route, i);
        auto best_cost = current_cost;

        if(best_needed_main_route_update || i_was_in_main_route) {
            (this->*reset_main_route_insertion_cache)();
        }

        for(const auto j : movegen_truck_customers[i]) {
            local_search_move_1_0_eval_route_insertion(i, j, best_cost, best_customer_position,
                                                       best_in_new_one_customer_route, best_needed_main_route_update,
                                                       best_route_iter);
        }

        for(const auto j : movegen_satellites[i]) {
            local_search_move_1_0_eval_park_insertion(i, j, best_cost, best_customer_position,
                                                      best_in_new_one_customer_route,
                                                      best_needed_main_route_update, best_route_iter, best_park,
                                                      best_park_position);
        }

        for(const auto j : movegen_vehicle_customers_no_park[i]) {

            if(vertex_served_by_route_type[j] == route_type::sub_route) {
                local_search_move_1_0_eval_route_insertion(i, j, best_cost, best_customer_position,
                                                           best_in_new_one_customer_route,
                                                           best_needed_main_route_update,
                                                           best_route_iter);
            } else {
                if(instance.is_vehicle_customer(i)) {
                    local_search_move_1_0_eval_route_insertion(i, j, best_cost, best_customer_position,
                                                               best_in_new_one_customer_route,
                                                               best_needed_main_route_update,
                                                               best_route_iter);
                }
            }

        }

        for(const auto j : movegen_vehicle_customers_yes_park[i]) {

            if(vertex_served_by_route_type[j] == route_type::sub_route) {
                local_search_move_1_0_eval_route_insertion(i, j, best_cost, best_customer_position,
                                                           best_in_new_one_customer_route,
                                                           best_needed_main_route_update,
                                                           best_route_iter);
            } else {

                local_search_move_1_0_eval_park_insertion(i, j, best_cost, best_customer_position,
                                                          best_in_new_one_customer_route,
                                                          best_needed_main_route_update, best_route_iter, best_park,
                                                          best_park_position);
                if(instance.is_vehicle_customer(i)) {
                    local_search_move_1_0_eval_route_insertion(i, j, best_cost, best_customer_position,
                                                               best_in_new_one_customer_route,
                                                               best_needed_main_route_update,
                                                               best_route_iter);
                }
            }

        }

        if(best_in_new_one_customer_route) {

            build_new_one_customer_sub_route(i, best_park);

            if(best_needed_main_route_update) {
                add_vertex_to_main_route(best_park, best_park_position);
                (this->*reset_main_route_insertion_cache)();
            }

        } else {

            add_customer_to_route(i, best_customer_position, best_route_iter);
            const auto best_route = *best_route_iter;
            if(best_route->get_type() == route_type::main_route) {
                (this->*reset_main_route_insertion_cache)();
            }

        }

        assert(best_cost - TOLERANCE <= current_cost);

        if(best_cost + TOLERANCE < current_cost) {
            improved = true;
            goto __repeat__;
        }

    }

    return improved;

}

void xsttrp_wip_solution::local_search_move_1_1_eval_swap(int i,
                                                          const route *iRoute,
                                                          float iRemoval,
                                                          int iNext,
                                                          int iPrev,
                                                          int j,
                                                          float &best_cost,
                                                          int &best_j){

    const auto jRoute = route_pointers[j];

    // move 1-0
    if(j == iNext || j == iPrev) {
        return;
    }

    const auto jPrev = jRoute->prev(j);
    const auto jNext = jRoute->next(j);

    const auto jRemoval = -instance.c[jPrev][j]
                          - instance.c[j][jNext];

    const auto jCapacity = jRoute->get_type() == route_type::main_route ? INT_MAX : instance.truck_capacity;
    const auto iCapacity = iRoute->get_type() == route_type::main_route ? INT_MAX : instance.truck_capacity;

    if(iRoute->load() - instance.q[i] + instance.q[j] > iCapacity ||
            jRoute->load() - instance.q[j] + instance.q[i] > jCapacity) {
        return;
    }

    const auto cost = iRemoval + instance.c[jPrev][i] + instance.c[i][jNext]
                       + jRemoval + instance.c[iPrev][j] + instance.c[j][iNext];

    if(cost < best_cost) {
        best_cost = cost;
        best_j = j;
    }

}

bool xsttrp_wip_solution::local_search_move_1_1() {


    bool improved = false;

    __repeat__:

    for (const auto i : customers_permutation) {

        // skip routes root
        if (instance.is_vehicle_customer_yes_park(i) && !routes[i - instance.parking_locations_begin].empty()) {
            continue;
        }

        auto iRoute_iter = route_iterators[i];
        auto iRoute = *iRoute_iter;
        const auto iPrev = iRoute->prev(i);
        const auto iNext = iRoute->next(i);

        const auto iRemoval = -instance.c[iPrev][i]
                              - instance.c[i][iNext];

        auto best_cost = static_cast<float>(FLT_MAX);
        auto best_j = -1;


        for(const auto& movegen : movegen_set_all) {
            for (const auto jNext : (*movegen)[i]) {



                if(vertex_served_by_route_type[jNext] == route_type::none) {
                    continue;
                }

                auto jRoute_iter = route_iterators[jNext];
                route* jRoute = *jRoute_iter;


                const auto j = jRoute->prev(jNext);




                if(i == j) {continue;}



                switch(instance.vtype[j]){
                    case dep:
                    case sat:
                        if(vertex_served_by_route_type[i] == route_type::main_route) {
                            local_search_move_1_1_eval_swap(i, iRoute, iRemoval, iNext, iPrev, j, best_cost, best_j);
                        }
                        break;
                    case trk:
                        if(vertex_served_by_route_type[i] == route_type::sub_route) {
                            local_search_move_1_1_eval_swap(i, iRoute, iRemoval, iNext, iPrev, j, best_cost, best_j);
                        }
                        break;
                    case vly:
                        if(vertex_served_by_route_type[i] == vertex_served_by_route_type[j] ||
                           ((instance.vtype[i] == vertex_type::vln || instance.vtype[i] == vertex_type::vly)
                            && routes[j - instance.parking_locations_begin].empty())) {
                            local_search_move_1_1_eval_swap(i, iRoute, iRemoval, iNext, iPrev, j, best_cost, best_j);
                        }
                        break;
                    case vln:
                        if(vertex_served_by_route_type[i] == vertex_served_by_route_type[j] ||
                           instance.vtype[i] == vertex_type::vln || instance.vtype[i] == vertex_type::vly) {
                            local_search_move_1_1_eval_swap(i, iRoute, iRemoval, iNext, iPrev, j, best_cost, best_j);
                        }
                        break;
                }


            }
        }


        if (best_cost < -TOLERANCE) {

            const auto jRoute_iter = route_iterators[best_j];
            auto jRoute = route_pointers[best_j];

            route::swap_vertices(iRoute, i, jRoute, best_j, best_cost);

            route_iterators[i] = jRoute_iter;
            route_pointers[i] = jRoute;
            vertex_served_by_route_type[i] = jRoute->get_type();

            route_iterators[best_j] = iRoute_iter;
            route_pointers[best_j] = iRoute;
            vertex_served_by_route_type[best_j] = iRoute->get_type();

            improved = true;

            goto __repeat__;

        }


    }

    return improved;

}

bool xsttrp_wip_solution::local_search_2_opt() {

    for(auto k = 0; k < instance.parking_locations_num; k++){
        for(auto& route : routes[k]) {
            route->compute_cumulative_load();
        }
    }

    bool improved = false;

    __repeat__:

    for(const auto i : vertices_permutation) {

        if(vertex_served_by_route_type[i] == route_type::none) {
            continue;
        }

        auto iRoute_iter = route_iterators[i];
        auto iRoute = *iRoute_iter;
        const auto iLoc = iRoute->begin();

        for(const auto& movegen : movegen_set_all) {
            for(const auto j : (*movegen)[i]) {

                if(vertex_served_by_route_type[j] == route_type::none) {
                    continue;
                }

                auto jRoute_iter = route_iterators[j];
                route* jRoute = *jRoute_iter;
                const auto jLoc = jRoute->begin();

                if(iRoute == jRoute) {

                    // intra-route 2opt move

                    const auto iNext = iRoute->next(i);
                    const auto jNext = jRoute->next(j);

                    if(iNext == j || jNext == i) {
                        continue;
                    }

                    const auto delta = + instance.c[i][j]
                                       + instance.c[iNext][jNext]
                                       - instance.c[i][iNext]
                                       - instance.c[j][jNext];

                    if(delta < -TOLERANCE) {
                        iRoute->reverse(iNext, j);
                        iRoute->compute_cumulative_load();
                        improved = true;


                        goto __repeat__;
                    }

                } else if(iLoc == jLoc) {

                    // intra-satellite 2opt move

                    {
                        // move 1

                        if(iRoute->get_load_before(i) + jRoute->get_load_after(j) <= instance.truck_capacity &&
                           iRoute->get_load_after(i) - instance.q[i] +jRoute->get_load_before(j) - instance.q[j] <= instance.truck_capacity) {

                            const auto iNext = iRoute->next(i);
                            const auto jPrev = jRoute->prev(j);

                            const auto delta = + instance.c[i][j]
                                               + instance.c[jPrev][iNext]
                                               - instance.c[i][iNext]
                                               - instance.c[jPrev][j];

                            if(delta < -TOLERANCE) {

                                auto curr = iNext;
                                while(curr != iLoc) {
                                    const auto next = iRoute->next(curr);
                                    iRoute->remove(curr);
                                    jRoute->insert(j, curr);
                                    route_iterators[curr] = jRoute_iter;
                                    route_pointers[curr] = jRoute;
                                    curr = next;
                                }

                                curr = j;
                                while(curr != jLoc) {
                                    const auto next = jRoute->next(curr);
                                    jRoute->remove(curr);
                                    iRoute->append(curr);
                                    route_iterators[curr] = iRoute_iter;
                                    route_pointers[curr] = iRoute;
                                    curr = next;
                                }


                                if(iRoute->is_empty()) {
                                    (this->*remove_route)(iRoute, iRoute_iter);
                                } else {
                                    iRoute->compute_cumulative_load();
                                }

                                if(jRoute->is_empty()) {
                                    (this->*remove_route)(jRoute, jRoute_iter);
                                } else {
                                    jRoute->compute_cumulative_load();
                                }


                                improved = true;
                                goto __repeat__;
                            }

                        }

                    }

                    {
                        // move 2
                        if(iRoute->get_load_before(i) + jRoute->get_load_before(j) <= instance.truck_capacity &&
                           iRoute->get_load_after(i) - instance.q[i] + jRoute->get_load_after(j) - instance.q[j] <= instance.truck_capacity) {

                            const auto iNext = iRoute->next(i);
                            const auto jNext = jRoute->next(j);

                            const auto delta = + instance.c[i][j]
                                               + instance.c[jNext][iNext]
                                               - instance.c[i][iNext]
                                               - instance.c[j][jNext];

                            if(delta < -TOLERANCE) {


                                auto curr = j;
                                while(curr != jLoc) {
                                    const auto prev = jRoute->prev(curr);
                                    jRoute->remove(curr);
                                    iRoute->insert(iNext, curr);
                                    route_iterators[curr] = iRoute_iter;
                                    route_pointers[curr] = iRoute;
                                    curr = prev;
                                }

                                auto before = jNext;
                                curr = iNext;
                                while(curr != iLoc) {
                                    const auto next = iRoute->next(curr);
                                    iRoute->remove(curr);
                                    jRoute->insert(before,curr);
                                    route_iterators[curr] = jRoute_iter;
                                    route_pointers[curr] = jRoute;
                                    before = curr;
                                    curr = next;
                                }


                                if(iRoute->is_empty()) {
                                    (this->*remove_route)(iRoute, iRoute_iter);
                                } else {
                                    iRoute->compute_cumulative_load();
                                }

                                if(jRoute->is_empty()) {
                                    (this->*remove_route)(jRoute, jRoute_iter);
                                } else {
                                    jRoute->compute_cumulative_load();
                                }


                                improved = true;
                                goto __repeat__;

                            }


                        }

                    }

                }




            }
        }

    }


    return improved;

}

bool xsttrp_wip_solution::local_search_segment_swap() {

    for (auto k = 0; k < instance.parking_locations_num; k++) {
        for (auto &route : routes[k]) {
            route->compute_cumulative_load();
        }
    }

    bool improved = false;

    __repeat__:


    for (const auto iSegment_end : customers_permutation) {

        if (vertex_served_by_route_type[iSegment_end] != route_type::sub_route) {
            continue;
        }

        auto iRoute_iter = route_iterators[iSegment_end];
        auto iRoute = *iRoute_iter;
        const auto iLoc = iRoute->begin();

        const auto iSegment_end_next = iRoute->next(iSegment_end);

        for(auto iSegment_begin = iRoute->prev(iSegment_end), iSegment_len = 2; iSegment_begin != iLoc && iSegment_len < SUB_ROUTES_SEGMENTS_SWAP_MAX_LENGTH; iSegment_begin = iRoute->prev(iSegment_begin), iSegment_len++){

            const auto iSegment_begin_prev = iRoute->prev(iSegment_begin);


            const auto iSegment_load = iRoute->get_load_before(iSegment_end) - iRoute->get_load_before(iSegment_begin) + instance.q[iSegment_begin];

            for (const auto &movegen : movegen_set_customers) {
                for (const auto j : (*movegen)[iSegment_end]) {

                    if (vertex_served_by_route_type[j] != route_type::sub_route) {
                        continue;
                    }

                    auto jRoute_iter = route_iterators[j];
                    route *jRoute = *jRoute_iter;

                    if(iRoute == jRoute) {
                        continue;
                    }

                    const auto jLoc = jRoute->begin();

                    auto jSegment_end = jRoute->prev(j);

                    // try to move the segment iPrev-iSegment_end before j

                    // if it does not work expand the segment from j and try to swap them

                    if(jRoute->load() + iSegment_load <= instance.truck_capacity) {

                        const auto delta = + instance.c[iSegment_begin_prev][iSegment_end_next]
                                           + instance.c[jSegment_end][iSegment_begin]
                                           + instance.c[iSegment_end][j]
                                           - instance.c[iSegment_begin_prev][iSegment_begin]
                                           - instance.c[iSegment_end][iSegment_end_next]
                                           - instance.c[jSegment_end][j];

                        if(delta < -TOLERANCE) {


                            auto curr = iSegment_begin;
                            while(curr != iSegment_end_next) {
                                const auto next = iRoute->next(curr);
                                iRoute->remove(curr);
                                jRoute->insert(j, curr);
                                route_iterators[curr] = jRoute_iter;
                                route_pointers[curr] = jRoute;
                                curr = next;
                            }


                            if(iRoute->is_empty()) {
                                (this->*remove_route)(iRoute, iRoute_iter);
                            } else {
                                iRoute->compute_cumulative_load();
                            }

                            jRoute->compute_cumulative_load();

                            improved = true;
                            goto __repeat__;


                        }

                    }

                    if(jSegment_end == jRoute->begin()){
                        continue;
                    }


                    for(auto jSegment_begin = jRoute->prev(jSegment_end), jSegment_len = 2; jSegment_begin != jLoc && jSegment_len < SUB_ROUTES_SEGMENTS_SWAP_MAX_LENGTH; jSegment_begin = jRoute->prev(jSegment_begin), jSegment_len++){

                        const auto jSegment_load = jRoute->get_load_before(jSegment_end) - jRoute->get_load_before(jSegment_begin) + instance.q[jSegment_begin];

                        if(iRoute->load() - iSegment_load + jSegment_load <= instance.truck_capacity &&
                           jRoute->load() - jSegment_load + iSegment_load <= instance.truck_capacity) {

                            const auto jSegment_begin_prev = jRoute->prev(jSegment_begin);

                            const auto delta = + instance.c[iSegment_begin_prev][jSegment_begin]
                                               + instance.c[jSegment_end][iSegment_end_next]
                                               + instance.c[jSegment_begin_prev][iSegment_begin]
                                               + instance.c[iSegment_end][j]
                                               - instance.c[iSegment_begin_prev][iSegment_begin]
                                               - instance.c[iSegment_end][iSegment_end_next]
                                               - instance.c[jSegment_begin_prev][jSegment_begin]
                                               - instance.c[jSegment_end][j];

                            if(delta < -TOLERANCE) {

                                auto curr = iSegment_begin;
                                while(curr != iSegment_end_next) {
                                    const auto next = iRoute->next(curr);
                                    iRoute->remove(curr);
                                    jRoute->insert(j, curr);
                                    route_iterators[curr] = jRoute_iter;
                                    route_pointers[curr] = jRoute;
                                    curr = next;
                                }


                                curr = jSegment_begin;
                                while(curr != iSegment_begin) {
                                    const auto next = jRoute->next(curr);
                                    jRoute->remove(curr);
                                    iRoute->insert(iSegment_end_next, curr);
                                    route_iterators[curr] = iRoute_iter;
                                    route_pointers[curr] = iRoute;
                                    curr = next;
                                }

                             
                                assert(iRoute->load() <= instance.truck_capacity);
                                assert(jRoute->load() <= instance.truck_capacity);

                                iRoute->compute_cumulative_load();
                                jRoute->compute_cumulative_load();

                                improved = true;
                                goto __repeat__;


                            }

                        }

                    }



                }
            }
        }



        }

return improved;

}

bool xsttrp_wip_solution::local_search_roots_refine() {


    bool improved = false;

    auto main_route_iter = routes[instance.parking_locations_num].begin();
    auto main_route = *main_route_iter;

    std::vector<route_iterator> routes_iter;

    (this->*reset_main_route_insertion_cache)();

    __repeat__:

    routes_iter.clear();

    for(auto k = 0; k < instance.parking_locations_num; k++){
        for(auto iter = routes[k].begin(); iter != routes[k].end(); ++iter) {
            routes_iter.push_back(iter);
            assert(*iter != main_route);
        }
    }

    knuth_shuffle(routes_iter, rand_engine);

    for(auto route_iter : routes_iter) {

        auto route = *route_iter;

        const auto kLoc = route->begin();
        auto kLoc_included_in_sub_route = false;
        const auto k = kLoc - instance.parking_locations_begin;


        auto detach_gain = detach_route(route_iter);


        if(instance.is_satellite(kLoc) && routes[k].empty()) {

            detach_gain += (this->*remove_satellite)(kLoc);

            (this->*reset_main_route_insertion_cache)();

        } else if (route->load() + instance.q[kLoc] <= instance.truck_capacity && routes[k].empty()) {

            // kLoc was a vehicle customer with parking facility

            const auto kLoc_main_route_next = main_route->next(kLoc);
            const auto kLoc_main_route_prev = main_route->prev(kLoc);

            detach_gain +=  + instance.c[kLoc_main_route_prev][kLoc_main_route_next]
                            - instance.c[kLoc_main_route_prev][kLoc]
                            - instance.c[kLoc][kLoc_main_route_next];

            main_route->remove(kLoc);

            (this->*reset_main_route_insertion_cache)();

            // In detach_route I've changed the route root as the next of kLoc in that route
            const auto kLoc_sub_route_next = route->begin();
            const auto kLoc_sub_route_prev = route->prev(kLoc_sub_route_next);
            route->insert(kLoc_sub_route_next, kLoc);

            detach_gain +=  + instance.c[kLoc_sub_route_prev][kLoc]
                            + instance.c[kLoc][kLoc_sub_route_next]
                            - instance.c[kLoc_sub_route_prev][kLoc_sub_route_next];




            kLoc_included_in_sub_route = true;

        }


        struct bulk_roots_route_setup config {};
        identify_best_attachment_for_detached_route(route, movegen_park_to_park[k], config);


        const auto best_k = config.root - instance.parking_locations_begin;


        if(config.root == kLoc && kLoc_included_in_sub_route) {

            // if kLoc is re-selected as best root and it was previously included in the sub-route itself
            // remove it from the sub-route.

            // procedure identify_best_attachment_for_detached_route does not know that kLoc has been inserted in the sub-route
            // if kLoc is selected as best root then the procedure returns cost 0 because kLoc was already there.

            //config.cost = -detach_gain;

            route->set_root(kLoc);

        } else {
            route->set_root(config.begin, config.root);
//            route->insert(config.begin, config.root);
        }

        if(config.root != kLoc && kLoc_included_in_sub_route) {
            route_iterators[kLoc] = route_iter;
            route_pointers[kLoc] = route;
            vertex_served_by_route_type[kLoc] = route_type::sub_route;
        }

//        route->set_root(config.root);
        //route->change_root(config.root);

        assert(route->load() <= instance.truck_capacity);

        if(!main_route->contains(config.root)) {
            main_route->insert(config.root_opening_position, config.root);
            route_iterators[config.root] = main_route_iter;
            route_pointers[config.root] = main_route;
            vertex_served_by_route_type[config.root] = route_type::main_route;
            (this->*reset_main_route_insertion_cache)();
        }

        const auto iter = routes[best_k].insert(routes[best_k].end(), route);
        for(auto curr = route->get_first_vertex(); curr != config.root; curr = route->next(curr)) {
            route_iterators[curr] = iter;
            route_pointers[curr] = route;
        }



        const auto current_cost = -detach_gain;

        assert(config.cost - TOLERANCE <= current_cost);

        if(config.cost + TOLERANCE < current_cost) {

            improved = true;
            goto __repeat__;

        }

    }


    return improved;

}











