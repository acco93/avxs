//
// Created by acco on 7/23/18.
//

#include <iostream>
#include <algorithm>
#include <cfloat>
#include "xsttrp_wip_solution.hpp"
#include "routes_slab.hpp"
#include "mm.hpp"


xsttrp_wip_solution::xsttrp_wip_solution(const xsttrp_instance &p_instance,
                                 std::mt19937 &p_rand_engine,
                                 int **p_candidate_list,
                                 float **p_candidate_list_probabilities,
                                 std::vector<arc> &sub_arcs,
                                 std::vector<arc> &main_arcs,
                                 routes_slab &p_routes_pool) :
        instance(p_instance),
        rand_engine(p_rand_engine),
        candidate_list(p_candidate_list),
        candidate_list_probabilities(p_candidate_list_probabilities),
        sub_arcs(sub_arcs),
        main_arcs(main_arcs),
        routes_pool(p_routes_pool) {

    initialize_from_scratch();

}


xsttrp_wip_solution::xsttrp_wip_solution(const xsttrp_wip_solution &other) :
        instance(other.instance),
        rand_engine(other.rand_engine),
        candidate_list(other.candidate_list),
        candidate_list_probabilities(other.candidate_list_probabilities),
        sub_arcs(other.sub_arcs),
        main_arcs(other.main_arcs),
        routes_pool(other.routes_pool) {

    initialize_from_scratch();
    initialize_from_solution(other);

}

xsttrp_wip_solution &xsttrp_wip_solution::operator=(const xsttrp_wip_solution &other) {

    release_resources();
    initialize_from_solution(other);
    return *this;

}

xsttrp_wip_solution::~xsttrp_wip_solution() {

    mm::release_raw_contiguous_memory(movegen_is_present);
    release_resources();

}

void xsttrp_wip_solution::initialize_from_scratch() {

    // Allocate memory and initialize it

    routes.resize(instance.parking_locations_num + 1);                 // parking locations + depot

    vertex_served_by_route_type.resize(instance.matrix_size);

    for (auto i = 0; i < instance.matrix_size; i++) {
        vertex_served_by_route_type[i] = route_type::none;
    }

    route_pointers.resize(instance.matrix_size);
    route_iterators.resize(instance.matrix_size);

    cost_is_up_to_date = true;
    cost = FLT_MAX;

    movegen_truck_customers.resize(instance.matrix_size);
    movegen_vehicle_customers_no_park.resize(instance.matrix_size);
    movegen_vehicle_customers_yes_park.resize(instance.matrix_size);
    movegen_satellites.resize(instance.matrix_size);

    movegen_park_to_park.resize(instance.parking_locations_num);
    movegen_main_vertices.resize(instance.matrix_size);

    if (instance.truck_customers_num > 0) {
        movegen_set_customers.push_back(&movegen_truck_customers);
        movegen_set_all.push_back(&movegen_truck_customers);
    }

    if (instance.vehicle_customers_no_park_num > 0) {
        movegen_set_customers.push_back(&movegen_vehicle_customers_no_park);
        movegen_set_all.push_back(&movegen_vehicle_customers_no_park);
    }

    if (instance.vehicle_customers_yes_park_num > 0) {
        movegen_set_customers.push_back(&movegen_vehicle_customers_yes_park);
        movegen_set_all.push_back(&movegen_vehicle_customers_yes_park);
    }

    if(instance.satellites_num > 0) {
        movegen_set_all.push_back(&movegen_satellites);
    }

    park_assignments.resize(instance.parking_locations_num + 1);

    local_search_procedures = {

            #if defined BEST_RNEI_ORDER && defined OPERATOR_SEGSWAP
            &xsttrp_wip_solution::local_search_segment_swap,
            #endif

            #ifdef OPERATOR_RELOCATE
            &xsttrp_wip_solution::local_search_move_1_0,
            #endif

            #ifdef OPERATOR_SWAP
            &xsttrp_wip_solution::local_search_move_1_1,
            #endif

            #ifdef OPERATOR_TWOPT
            &xsttrp_wip_solution::local_search_2_opt,
            #endif

            #if !defined BEST_RNEI_ORDER && defined OPERATOR_SEGSWAP
            &xsttrp_wip_solution::local_search_segment_swap,
            #endif
    };

    #ifdef STATS
    add_ls_functions = {

        #if defined BEST_RNEI_ORDER && defined OPERATOR_SEGSWAP
        &avxs_result::add_segswap,
        #endif

        #ifdef OPERATOR_RELOCATE
        &avxs_result::add_relocate,
        #endif

        #ifdef OPERATOR_SWAP
        &avxs_result::add_swap,
        #endif

        #ifdef OPERATOR_TWOPT
        &avxs_result::add_twopt,
        #endif

        #if !defined BEST_RNEI_ORDER && OPERATOR_SEGSWAP
        &avxs_result::add_segswap,
        #endif

    };
    #endif

    #ifdef OPERATOR_ROOTREF
    if(instance.parking_locations_num > 1) {

        local_search_procedures.push_back(&xsttrp_wip_solution::local_search_roots_refine);

        #ifdef STATS
        add_ls_functions.push_back(&avxs_result::add_rootref);
        #endif

    }
    #endif

    customers_permutation.reserve(instance.customers_num);
    for (auto i = instance.customers_begin; i < instance.customers_end; i++) {
        customers_permutation.emplace_back(i);
    }

    vertices_permutation.reserve(instance.matrix_size);
    for (auto i = instance.customers_begin; i < instance.matrix_size; i++) {
        vertices_permutation.emplace_back(i);
    }

    parks_permutation.reserve(instance.parking_locations_num);
    for(auto kLoc = instance.parking_locations_begin; kLoc < instance.parking_locations_end; kLoc++){
        parks_permutation.emplace_back(kLoc);
    }


    unrouted_customers.reserve(instance.customers_num);

    main_route_insertion_position_cache.resize(instance.matrix_size, INVALID_CACHE);
    main_route_insertion_cost_cache.resize(instance.matrix_size);

    if(instance.ptype == problem_type::xsttrp || instance.ptype == problem_type::lrp) {
        remove_satellite = &xsttrp_wip_solution::remove_satellite_xsttrp;
        remove_route = &xsttrp_wip_solution::remove_route_xsttrp;

        reset_main_route_insertion_cache = &xsttrp_wip_solution::reset_main_route_insertion_cache_xsttrp;
        get_park_opening_cost = &xsttrp_wip_solution::get_park_opening_cost_xsttrp;

    }

    if(instance.ptype == problem_type::mdvrp){
        remove_satellite = &xsttrp_wip_solution::remove_satellite_mdvrp;
        remove_route = &xsttrp_wip_solution::remove_route_mdvrp;
        reset_main_route_insertion_cache = &xsttrp_wip_solution::reset_main_route_insertion_cache_mdvrp;
        get_park_opening_cost = &xsttrp_wip_solution::get_park_opening_cost_mdvrp;
    }


    previous_pi = FLT_MAX;
    previous_main_arcs_index = 0;
    previous_sub_arcs_index = 0;

    movegen_is_present = mm::request_raw_contiguous_memory<bool>(instance.matrix_size, instance.matrix_size);

    shake_type = 1;


}

void xsttrp_wip_solution::initialize_from_solution(const xsttrp_wip_solution &other) {

    // Copy data from an existing solution

    cost_is_up_to_date = other.cost_is_up_to_date;
    cost = other.cost;

    vertex_served_by_route_type = other.vertex_served_by_route_type;

    main_route_insertion_position_cache = other.main_route_insertion_position_cache;
    main_route_insertion_cost_cache = other.main_route_insertion_cost_cache;

    // Copy the sub-routes
    for (auto k = 0; k < instance.parking_locations_num; k++) {
        const auto kLoc = k + instance.parking_locations_begin;
        for (auto route : other.routes[k]) {
            auto new_route = routes_pool.request(kLoc, route_type::sub_route);
            *new_route = *route;                    // make sure not to copy just the pointer
            routes[k].push_back(new_route);
        }
    }

    // Copy the main-route (if any)
    const auto depot_index = instance.parking_locations_num;

    if (!other.routes[depot_index].empty()) {
        auto new_main_route = routes_pool.request(instance.depot, route_type::main_route);
        *new_main_route = *other.routes[depot_index].front();
        routes[depot_index].push_back(new_main_route);
    }

    initialize_route_iterators_and_pointers();


}

void xsttrp_wip_solution::release_resources() {

    // Release previously hold resources

    for (auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for (auto route : routes[k]) {
            routes_pool.release(route);
        }
        routes[k].clear();
    }

}

void xsttrp_wip_solution::initialize_route_iterators_and_pointers() {

    for (auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for (auto ptr = routes[k].begin(); ptr != routes[k].end(); ++ptr) {
            route *route = *ptr;
            for (auto curr = route->next(route->begin()); curr != route->begin(); curr = route->next(curr)){
                route_pointers[curr] = route;
                route_iterators[curr] = ptr;
            }
        }
    }

    route_iterators[instance.depot] = routes[instance.parking_locations_num].begin();
    route_pointers[instance.depot] = *route_iterators[instance.depot];

}

void xsttrp_wip_solution::reset() {

    cost_is_up_to_date = true;
    cost = FLT_MAX;

    for (int i = 0; i < instance.matrix_size; i++) {
        route_pointers[i] = nullptr;
        vertex_served_by_route_type[i] = route_type::none;
    }

    shake_type = 2;

    #ifdef STATS
    add_shake = &avxs_result::add_before_any_shake;
    #endif

    release_resources();

}

void xsttrp_wip_solution::print() {

    if(routes[instance.parking_locations_num].front()){
        routes[instance.parking_locations_num].front()->print();
    }

    for (int k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            route->print();
        }
    }
    std::cout << get_cost() << std::endl;

}

float xsttrp_wip_solution::get_cost() {

    if (!cost_is_up_to_date) {

        auto num = 0;

        cost = 0;
        for (int k = 0; k < instance.parking_locations_num + 1; k++) {
            for (const auto &route : routes[k]) {
                cost += route->cost();
                num++;
            }
        }

        if(instance.ptype == mdvrp) {
            for (int k = 0; k < instance.parking_locations_num; k++) {

                const auto route_excess = static_cast<int>(routes[k].size()) - instance.mdvrp_vehicles_num;
                auto route_excess_cost = 0.0f;
                if (route_excess > 0) {
                    route_excess_cost = route_excess * MDVRP_ROUTE_EXCESS_PENALTY;
                }

                cost += route_excess_cost;

            }
        }

        if(!num){
            // infeasible solution
            cost = FLT_MAX;
        }


        cost_is_up_to_date = true;

    }

    return static_cast<float>(cost);

}

void xsttrp_wip_solution::granular_local_search(__attribute__ ((unused)) avxs_result &result) {

    bool improved;

    #ifndef VND

    #ifdef STATS
    knuth_shuffle(local_search_procedures, add_ls_functions, rand_engine);
    #else
    knuth_shuffle(local_search_procedures, rand_engine);
    #endif


    knuth_shuffle(customers_permutation, rand_engine);
    knuth_shuffle(vertices_permutation, rand_engine);
    knuth_shuffle(parks_permutation, rand_engine);

    #endif


    do {

        improved = false;

        for(auto n = 0u; n < local_search_procedures.size(); n++) {

            auto& procedure = local_search_procedures[n];

            #ifdef STATS
            cost_is_up_to_date = false;
            const auto old_cost = get_cost();
            #endif


            improved |= (this->*(procedure))();

            assert(this->is_feasible());

            #ifdef STATS
            cost_is_up_to_date = false;
            const auto new_cost = get_cost();

            if(new_cost < MDVRP_ROUTE_EXCESS_PENALTY) {
                (result.*(add_ls_functions[n]))(old_cost - new_cost);
                (result.*(add_shake))(old_cost - new_cost);
            }
            #endif


        }

    } while (improved);


    cost_is_up_to_date = false;

}

int xsttrp_wip_solution::build_and_improve(set_partitioning_pool &hq_pool, avxs_result &result) {

    reset();

    build_initial_solution();
    //build_initial_solution_biased_random(restart);

    #ifdef STATS
    result.add_sample(get_cost());
    #endif

    xsttrp_wip_solution best_solution(*this);

    auto non_improving_iterations = 0u;
    auto granular_iterations = 0u;
    auto pi = PI_BASE;
    const auto max_granular_iterations = static_cast<int>(0.2 * ILS_ITERATIONS);

    update_move_generators(pi, best_solution);


    auto total_iterations = 0;

    while(true) {
        
        granular_local_search(result);

        #ifdef STATS
        const auto new_cost = get_cost();
        result.add_sample(new_cost);
        #endif

        if(get_cost() + TOLERANCE < best_solution.get_cost()) {
            best_solution = *this;
            non_improving_iterations = 0;
            granular_iterations = 0;
            pi = PI_BASE;
            update_move_generators(pi, best_solution);
            add_routes_to_set_partitioning(hq_pool);
        } else {
            non_improving_iterations++;
            if (non_improving_iterations >= ILS_ITERATIONS) {
                break;
            }
        }

        total_iterations++;


        *this = best_solution;

        granular_iterations++;
        if (granular_iterations >= max_granular_iterations) {
            pi = pi * 2.0f;
            update_move_generators(pi, best_solution);
            granular_iterations = 0;
        }

        shake_up();


    }

    *this = best_solution;

    return total_iterations;

}

void xsttrp_wip_solution::build_initial_solution() {

    cost_is_up_to_date = false;

    const auto depot_index = instance.parking_locations_num;


    for (auto k = 0; k < instance.parking_locations_num + 1; k++) {
        park_assignments[k].clear();
    }

    for (auto i = instance.truck_customers_begin; i < instance.vehicle_customers_no_park_end; i++) {

        // assign truck customers using a ranking selection strategy
        auto kLocStar = roulette_wheel_selection(candidate_list[i],candidate_list_probabilities[i],instance.parking_locations_num + 1,rand_engine);



        if(instance.q[i] > instance.truck_capacity) {
            kLocStar = depot_index;
        }

        #ifdef RTGUARD
        if(instance.is_truck_customer(i)){
            assert(kLocStar != instance.depot);
        }
        #endif

        assert(instance.is_parking_location(kLocStar) || instance.depot == kLocStar);

        const auto kStar = kLocStar - instance.parking_locations_begin;

        park_assignments[kStar].push_back(i);

        if(kStar == depot_index) {
            vertex_served_by_route_type[i] = route_type::main_route;
        } else {
            vertex_served_by_route_type[i] = route_type::sub_route;
        }


    }

    std::vector<float> temp_candidate_list_probability(instance.parking_locations_num + 1);
    for(auto i = instance.vehicle_customers_yes_park_begin; i < instance.vehicle_customers_yes_park_end; i++){

        const auto k = i - instance.parking_locations_begin;

        if(park_assignments[k].empty() && instance.q[i] <= instance.truck_capacity){

            for(auto n = 0; n <= instance.parking_locations_num; n++){
                const auto sLoc = candidate_list[i][n];
                const auto s = sLoc - instance.parking_locations_begin;
                if(sLoc != instance.depot && park_assignments[s].empty()){
                    temp_candidate_list_probability[n] = 0.0f;
                } else {
                    temp_candidate_list_probability[n] = candidate_list_probabilities[i][n];
                }
            }


            float* raw_vector = &temp_candidate_list_probability[0];
            // assign truck customers using a ranking selection strategy
            auto sLocStar = roulette_wheel_selection(candidate_list[i],
                                                           raw_vector,
                                                           instance.parking_locations_num + 1,
                                                           rand_engine);

            if(sLocStar == -1) {
                // unable to find an open parking location => assigned to the depot.
                sLocStar = instance.depot;
            }


            assert(sLocStar != i);

            assert(instance.is_parking_location(sLocStar) || instance.depot == sLocStar);

            const auto sStar = sLocStar - instance.parking_locations_begin;

            assert(!park_assignments[sStar].empty() || instance.depot == sLocStar);

            park_assignments[sStar].push_back(i);

            if(sStar == depot_index) {
                vertex_served_by_route_type[i] = route_type::main_route;
            } else {
                vertex_served_by_route_type[i] = route_type::sub_route;
            }



        } else {
            park_assignments[depot_index].push_back(i);
            vertex_served_by_route_type[i] = route_type::main_route;
        }

    }

    for(auto kLoc = instance.satellites_begin; kLoc < instance.satellites_end; kLoc++){
        const auto k = kLoc - instance.parking_locations_begin;
        if(!park_assignments[k].empty() || instance.ptype == problem_type::mdvrp) {
            park_assignments[depot_index].push_back(kLoc);
            vertex_served_by_route_type[kLoc] = route_type::main_route;
        } else {
            vertex_served_by_route_type[kLoc] = route_type::none;
        }
    }

    for (auto k = 0; k < instance.parking_locations_num; k++) {

        if (!park_assignments[k].empty()) {

            clarke_and_wright_savings_algorithm(k, instance.truck_capacity, route_type::sub_route);

        }
    }


    // remove duplicates
    std::sort(park_assignments[depot_index].begin(), park_assignments[depot_index].end());
    park_assignments[depot_index].erase(std::unique(park_assignments[depot_index].begin(), park_assignments[depot_index].end() ), park_assignments[depot_index].end() );

    clarke_and_wright_savings_algorithm(depot_index, INT_MAX, route_type::main_route);

    route_iterators[instance.depot] = routes[depot_index].begin();
    route_pointers[instance.depot] = *route_iterators[instance.depot];

#ifdef RTGUARD
    for(auto i = 0; i < instance.matrix_size; i++){
        if(instance.is_satellite(i) && vertex_served_by_route_type[i] == route_type::none) {continue;}
        auto iRoute = *route_iterators[i];
        assert(iRoute->contains(i));
    }
#endif


    assert(routes[depot_index].size() == 1);
    assert(this->is_feasible());

}

void xsttrp_wip_solution::clarke_and_wright_savings_algorithm(int k, int capacity, route_type r_type) {


    assert(!park_assignments[k].empty());

    const auto kLoc = k + instance.parking_locations_begin;

    for (auto customer : park_assignments[k]) {

        const auto route = routes_pool.request(kLoc, r_type);
        route->append(customer);

        route_pointers[customer] = route;
        route_iterators[customer] = routes[k].emplace(routes[k].end(), route);

    }

    struct saving {
        int i;
        int j;
        float value;
    };

    const auto savings_num = static_cast<unsigned int>(park_assignments[k].size() * (park_assignments[k].size() - 1) * 0.5);

    std::vector<saving> savings(savings_num);

    auto index = 0u;

    const auto lambda = 1.0f;

    for (auto in = 0u; in < park_assignments[k].size() - 1; in++) {
        const auto i = park_assignments[k][in];
        for (auto jn = in + 1; jn < park_assignments[k].size(); jn++) {
            const auto j = park_assignments[k][jn];
            const auto value = +instance.c[i][kLoc]
                               + instance.c[kLoc][j]
                               - lambda * instance.c[i][j];
            savings[index].i = i;
            savings[index].j = j;
            savings[index].value = value;
            index++;
        }
    }



    assert(index == savings.capacity());

    std::sort(savings.begin(), savings.end(), [](const saving &a, const saving &b) { return a.value > b.value; });

    for (auto &saving : savings) {

        const auto i = saving.i;
        const auto j = saving.j;

        const auto iRoute = route_pointers[i];
        const auto jRoute = route_pointers[j];

        if (iRoute == jRoute) {
            continue;
        }

        if (iRoute->get_last_vertex() == i &&
            jRoute->get_first_vertex() == j &&
            iRoute->load() + jRoute->load() <= capacity) {

            auto curr = jRoute->get_first_vertex();

            const auto jRoute_iterator = route_iterators[curr];
            const auto iRoute_iterator = route_iterators[iRoute->get_first_vertex()];

            do {

                const auto next = jRoute->next(curr);

                jRoute->remove(curr);
                iRoute->append(curr);
                route_pointers[curr] = iRoute;
                route_iterators[curr] = iRoute_iterator;

                curr = next;

            } while (curr != kLoc);

            routes[k].erase(jRoute_iterator);
            routes_pool.release(jRoute);


            assert(iRoute->get_type() == main_route || iRoute->load() <= instance.truck_capacity);
            assert(jRoute->get_type() == main_route || jRoute->load() <= instance.truck_capacity);


        } else if (jRoute->get_last_vertex() == j &&
                   iRoute->get_first_vertex() == i &&
                   iRoute->load() + jRoute->load() <= capacity) {


            auto curr = iRoute->get_first_vertex();

            const auto iRoute_iterator = route_iterators[curr];
            const auto jRoute_iterator = route_iterators[jRoute->get_first_vertex()];


            do {

                const auto next = iRoute->next(curr);

                iRoute->remove(curr);
                jRoute->append(curr);
                route_pointers[curr] = jRoute;
                route_iterators[curr] = jRoute_iterator;

                curr = next;

            } while (curr != kLoc);

            routes[k].erase(iRoute_iterator);
            routes_pool.release(iRoute);

            assert(iRoute->get_type() == main_route || iRoute->load() <= instance.truck_capacity);
            assert(jRoute->get_type() == main_route || jRoute->load() <= instance.truck_capacity);

        }

    }



}

void xsttrp_wip_solution::update_move_generators(float pi, xsttrp_wip_solution& best_solution) {

    if(previous_pi == pi) {
        return;
    }

    const float sub_arcs_in_solution = instance.customers_num + instance.satellites_num;

    const auto main_arcs_in_solution = instance.vehicle_customers_num + instance.satellites_num + 1;

    auto sub_arcs_num = size_t(pi * sub_arcs_in_solution);

    if(sub_arcs_num % 2 != 0) {
        sub_arcs_num++;
    }

    if (sub_arcs_num > sub_arcs.size()) {
        sub_arcs_num = sub_arcs.size();
    }

    auto main_arcs_num = size_t(pi * main_arcs_in_solution);

    if(main_arcs_num % 2 != 0) {
        main_arcs_num++;
    }

    if(main_arcs_num > main_arcs.size()) {
        main_arcs_num = main_arcs.size();
    }

    auto sub_arcs_index = 0u;
    auto main_arcs_index = 0u;

    if(pi < previous_pi) {

        for (int i = instance.customers_begin; i < instance.matrix_size; i++) {

            movegen_vehicle_customers_no_park[i].clear();
            movegen_vehicle_customers_yes_park[i].clear();
            movegen_truck_customers[i].clear();
            movegen_satellites[i].clear();
            movegen_main_vertices[i].clear();

            for(auto j = 0; j < instance.matrix_size; j++){
                movegen_is_present[i][j] = false;
            }

        }

        for(auto i = 0; i < instance.parking_locations_num; i++){
            movegen_park_to_park[i].clear();
        }

    } else {

        sub_arcs_index = previous_sub_arcs_index;
        main_arcs_index = previous_main_arcs_index;

    }

    for(; sub_arcs_index < sub_arcs_num; sub_arcs_index++) {

        const auto i = sub_arcs[sub_arcs_index].i;
        const auto j = sub_arcs[sub_arcs_index].j;

        if(movegen_is_present[i][j]) {
            continue;
        }

        movegen_is_present[i][j] = true;


        if (instance.is_truck_customer(j)) {
            movegen_truck_customers[i].push_back(j);
        } else if (instance.is_vehicle_customer_yes_park(j)) {
            movegen_vehicle_customers_yes_park[i].push_back(j);
            if(instance.is_parking_location(i)) {
                movegen_park_to_park[i - instance.parking_locations_begin].push_back(j);
            }
        } else if (instance.is_vehicle_customer_no_park(j)) {
            movegen_vehicle_customers_no_park[i].push_back(j);
        } else if(instance.is_satellite(j)) {
            movegen_satellites[i].push_back(j);
            if(instance.is_parking_location(i)) {
                movegen_park_to_park[i - instance.parking_locations_begin].push_back(j);
            }
        }

    }


    for(; main_arcs_index < main_arcs_num; main_arcs_index++) {

        const auto i = main_arcs[main_arcs_index].i;
        const auto j = main_arcs[main_arcs_index].j;

        if(movegen_is_present[i][j]) {
            continue;
        }

        movegen_is_present[i][j] = true;

        if (instance.is_vehicle_customer_yes_park(j)) {
            movegen_vehicle_customers_yes_park[i].push_back(j);
            if(instance.is_parking_location(i)) {
                movegen_park_to_park[i - instance.parking_locations_begin].push_back(j);
            }
        } else if (instance.is_vehicle_customer_no_park(j)) {
            movegen_vehicle_customers_no_park[i].push_back(j);
        } else if(instance.is_satellite(j)) {
            movegen_satellites[i].push_back(j);
            if(instance.is_parking_location(i)) {
                movegen_park_to_park[i - instance.parking_locations_begin].push_back(j);
            }
        }

    }

    previous_pi = pi;
    previous_sub_arcs_index = sub_arcs_index;
    previous_main_arcs_index = main_arcs_index;



    // add itself
    for(auto kLoc = instance.parking_locations_begin; kLoc < instance.parking_locations_end; kLoc++) {
        const auto k = kLoc - instance.parking_locations_begin;

        if(movegen_is_present[kLoc][kLoc]) {
            continue;
        }
        movegen_is_present[kLoc][kLoc] = true;

        movegen_park_to_park[k].push_back(kLoc);
    }



        //  Add to the move generators the sub_arcs of the current best solution


    const auto main_route = best_solution.routes[instance.parking_locations_num].front();
    for(auto curr = main_route->get_first_vertex(); curr != instance.depot; curr = main_route->next(curr)) {

        const int next = main_route->next(curr);

        if(movegen_is_present[curr][next]) {
            continue;
        }
        movegen_is_present[curr][next] = true;

        if (instance.is_vehicle_customer_yes_park(next)) {
            movegen_vehicle_customers_yes_park[curr].push_back(next);
        } else if (instance.is_vehicle_customer_no_park(next)) {
            movegen_vehicle_customers_no_park[curr].push_back(next);
        } else if (instance.is_satellite(next)) {
            movegen_satellites[curr].push_back(next);
        }

        if(instance.is_parking_location(curr) && instance.is_parking_location(next)) {
            movegen_park_to_park[curr - instance.parking_locations_begin].push_back(next);
        }

    }

    for (int k = 0; k < instance.parking_locations_num; k++) {

        const int kLoc = k + instance.parking_locations_begin;
        for (auto &route: best_solution.routes[k]) {
            for (int curr = route->get_first_vertex(); curr != kLoc; curr = route->next(curr)) {
                const int next = route->next(curr);

                if(movegen_is_present[curr][next]) {
                    continue;
                }
                movegen_is_present[curr][next] = true;

                if (instance.is_truck_customer(next)) {
                    movegen_truck_customers[curr].push_back(next);
                } else if (instance.is_vehicle_customer_yes_park(next)) {
                    movegen_vehicle_customers_yes_park[curr].push_back(next);
                } else if (instance.is_vehicle_customer_no_park(next)) {
                    movegen_vehicle_customers_no_park[curr].push_back(next);
                }

            }
        }

    }


}

bool xsttrp_wip_solution::is_feasible() {

    for (auto k = 0; k < instance.parking_locations_num; k++) {
        for (const auto &route : routes[k]) {
            if(route->load() > instance.truck_capacity) {
                return false;
            }
            if(instance.vtype[route->begin()] == vertex_type::trk ||
                    instance.vtype[route->begin()] == vertex_type::vln) {
                return false;
            }
        }
    }

    for(auto i = instance.truck_customers_begin; i < instance.truck_customers_end; i++) {
        const auto route = route_pointers[i];
        if(route->get_type() == main_route) {
            return false;
        }
    }

    return true;

}















