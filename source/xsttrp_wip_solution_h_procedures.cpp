//
// Created by acco on 7/24/18.
//

#include <cfloat>
#include "xsttrp_wip_solution.hpp"

// because they are forward declared in xsttrp_wip_solution.hpp
#include "routes_slab.hpp"

void xsttrp_wip_solution::identify_open_parking_locations(std::vector<int> &storage) {

    for(auto k = 0; k < instance.parking_locations_num; k++){
        if(!routes[k].empty()) {
            storage.push_back(k + instance.parking_locations_begin);
        }
    }

}

void xsttrp_wip_solution::remove_all_customers_from_route(route *const route, std::vector<int> &storage) {

    for(auto curr = route->get_first_vertex(); curr != route->begin(); curr = route->next(curr)) {
        route_pointers[curr] = nullptr;
        vertex_served_by_route_type[curr] = route_type::none;
        storage.push_back(curr);
    }

}

void xsttrp_wip_solution::reset_main_route_insertion_cache_xsttrp() {

    for(auto i = instance.parking_locations_begin; i < instance.parking_locations_end; i++){
        main_route_insertion_position_cache[i] = INVALID_CACHE;
    }

}

void xsttrp_wip_solution::reset_main_route_insertion_cache_mdvrp() {

}

float xsttrp_wip_solution::remove_vertex(route* route, int i) {

    auto delta = route->remove(i);

    if(route->is_empty()){
        delta += (this->*remove_route)(route, route_iterators[i]);
    }

    route_pointers[i] = nullptr;
    vertex_served_by_route_type[i] = route_type::none;

    return delta;

}

float xsttrp_wip_solution::remove_route_xsttrp(route *route, xsttrp_wip_solution::route_iterator &iterator) {

    const auto kLoc = route->begin();
    const auto k = kLoc - instance.parking_locations_begin;

    routes[k].erase(iterator);

    routes_pool.release(route);

    if(instance.is_satellite(kLoc) && routes[k].empty()) {

        return remove_satellite_xsttrp(kLoc);

    } else {

        return 0.0f;

    }

}

float xsttrp_wip_solution::remove_route_mdvrp(route *route, xsttrp_wip_solution::route_iterator &iterator) {

    const auto kLoc = route->begin();
    const auto k = kLoc - instance.parking_locations_begin;

    routes[k].erase(iterator);
    routes_pool.release(route);

    const auto routes_excess = routes[k].size() - instance.mdvrp_vehicles_num;

    if(routes_excess > 0) {
        return -MDVRP_ROUTE_EXCESS_PENALTY * routes_excess;
    } else {
        return 0.0f;
    }




}

float xsttrp_wip_solution::remove_satellite_xsttrp(int kLoc) {

    vertex_served_by_route_type[kLoc] = route_type::none;
    return routes[instance.parking_locations_num].front()->remove(kLoc);

}

float xsttrp_wip_solution::remove_satellite_mdvrp(int) {
    return 0.0f;
}

void xsttrp_wip_solution::build_new_one_customer_sub_route(const int customer, const int kLoc) {

    assert(instance.is_customer(customer) && instance.is_parking_location(kLoc));

    const auto k = kLoc - instance.parking_locations_begin;
    auto route = routes_pool.request(kLoc, route_type::sub_route);
    route->append(customer);
    route_iterators[customer] = routes[k].insert(routes[k].end(), route);
    route_pointers[customer] = route;
    vertex_served_by_route_type[customer] = route_type::sub_route;

}

void xsttrp_wip_solution::add_vertex_to_main_route(const int vertex, const int position) {

    assert(instance.is_main_level_vertex(vertex));
    assert(routes[instance.parking_locations_num].front()->contains(position));

    const auto main_route_iter = routes[instance.parking_locations_num].begin();
    const auto main_route = *main_route_iter;

    main_route->insert(position, vertex);
    route_pointers[vertex] = main_route;
    route_iterators[vertex] = main_route_iter;
    vertex_served_by_route_type[vertex] = route_type::main_route;

}

void xsttrp_wip_solution::add_customer_to_route(const int customer,
                                                const int position,
                                                const route_iterator& route_iter) {

    //assert(instance.is_customer(customer));

    auto route = *route_iter;
    route->insert(position, customer);

    route_pointers[customer] = route;
    route_iterators[customer] = route_iter;

    vertex_served_by_route_type[customer] = route->get_type();

}

float xsttrp_wip_solution::get_park_opening_cost_xsttrp(const int kLoc, int &position, bool &main_route_update) {

    assert(vertex_served_by_route_type[kLoc] != route_type::sub_route);

    const auto main_route = routes[instance.parking_locations_num].front();

    if(main_route->contains(kLoc)) {

        main_route_update = false;
        return 0.0f;

    }

    main_route_update = true;

    if(main_route_insertion_position_cache[kLoc] == INVALID_CACHE) {

        main_route_insertion_cost_cache[kLoc] = main_route->find_best_insertion(kLoc, position) /*+ instance.lrp_satellite_opening_cost*/;
        main_route_insertion_position_cache[kLoc] = position;
        return main_route_insertion_cost_cache[kLoc];

    } else {

        position = main_route_insertion_position_cache[kLoc];
        return main_route_insertion_cost_cache[kLoc];

    }

}

float xsttrp_wip_solution::get_park_opening_cost_mdvrp(const int , int& , bool &main_route_update) {

    main_route_update = false;
    return 0.0f;

}

void xsttrp_wip_solution::perform_best_insertion(const int customer) {

    assert(instance.is_customer(customer));

    auto best_cost = static_cast<float>(FLT_MAX);
    auto best_customer_position = -1;
    auto best_build_new_route = false;
    auto best_park_insertion_in_main_route = false;
    route_iterator best_route_iter;
    auto best_park_position = -1;
    auto best_park = -1;

    for(auto k = 0; k < instance.parking_locations_num; k++){

        const auto kLoc = k + instance.parking_locations_begin;

        // skip parking locations served by sub-routes and auto-assignments
        if(vertex_served_by_route_type[kLoc] == route_type::sub_route || customer == kLoc) {
            continue;
        }

        // try insertion in existing route
        for(auto route_iter = routes[k].begin(); route_iter != routes[k].end(); ++route_iter) {

            auto route = *route_iter;

            if(route->load() + instance.q[customer] > instance.truck_capacity) {
                continue;
            }

            auto position = -1;
            auto cost = route->find_best_insertion(customer, position);

            if(cost < best_cost) {

                best_cost = cost;
                best_customer_position = position;
                best_build_new_route = false;
                best_route_iter = route_iter;

            }

        }

        // or as new one-customer sub-route
        {


            const auto route_excess = static_cast<int>(routes[k].size()) - instance.mdvrp_vehicles_num;
            auto route_excess_cost = 0.0f;
            if(route_excess > 0) {
                route_excess_cost = route_excess * MDVRP_ROUTE_EXCESS_PENALTY;
            }


            auto park_position = -1;
            auto park_insertion_in_main_route = false;


            auto cost = 2.0f * instance.c[kLoc][customer] + (this->*get_park_opening_cost)(kLoc, park_position, park_insertion_in_main_route) + route_excess_cost /*+ instance.lrp_sub_route_opening_cost*/;


            if(cost < best_cost) {

                best_cost = cost;
                best_build_new_route = true;
                best_park = kLoc;
                best_park_position = park_position;
                best_park_insertion_in_main_route = park_insertion_in_main_route;

            }

        }

    }

    // if vehicle customer try also the insertion in the main-route
    if(instance.is_vehicle_customer(customer)) {

        const auto main_route_iter = routes[instance.parking_locations_num].begin();
        const auto main_route = *main_route_iter;

        auto position = -1;
        const auto cost = main_route->find_best_insertion(customer, position);

        if(cost < best_cost) {

            best_cost = cost;
            best_customer_position = position;
            best_build_new_route = false;
            best_route_iter = main_route_iter;

        }

    }


    if(best_build_new_route) {

        build_new_one_customer_sub_route(customer, best_park);

        if(best_park_insertion_in_main_route) {

            add_vertex_to_main_route(best_park, best_park_position);
            (this->*reset_main_route_insertion_cache)();

        }

    } else {

        add_customer_to_route(customer, best_customer_position, best_route_iter);

        const auto best_route = *best_route_iter;
        if(best_route->get_type() == route_type::main_route) {
            (this->*reset_main_route_insertion_cache)();
        }

    }


}

float xsttrp_wip_solution::detach_route(route_iterator iter) {

    const auto route = *iter;
    const auto kLoc = route->begin();
    const auto k = kLoc - instance.parking_locations_begin;

    //const auto first = route->get_first_vertex();
    routes[k].erase(iter);

    const auto route_excess = static_cast<int>(routes[k].size()) - instance.mdvrp_vehicles_num;

    auto route_excess_gain = 0.0f;
    if(route_excess > 0) {
        route_excess_gain = -MDVRP_ROUTE_EXCESS_PENALTY;
    }


  //  route->change_root(first);



//    return route->remove(kLoc) + route_excess_gain;
    return route->remove_root() + route_excess_gain;



}

void xsttrp_wip_solution::identify_best_attachment_for_detached_route(route *route, std::vector<int> &movegen, bulk_roots_route_setup &config) {
    const auto route_initial_vertex = route->begin();

    config.cost = static_cast<float>(FLT_MAX);


    for(auto root : movegen) {

        if(vertex_served_by_route_type[root] == route_type::sub_route) {
            continue;
        }



        const auto route_excess = static_cast<int>(routes[root - instance.parking_locations_begin].size()) - instance.mdvrp_vehicles_num;
        auto route_excess_cost = 0.0f;
        if(route_excess > 0) {
            route_excess_cost = route_excess * MDVRP_ROUTE_EXCESS_PENALTY;
        }

        auto curr = route_initial_vertex;
        auto next = route->next(curr);

        auto root_opening_position = -1;
        auto root_opening_main_route_update = false;


        const auto park_opening_cost = (this->*get_park_opening_cost)(root, root_opening_position, root_opening_main_route_update) + route_excess_cost;


        do {



            const auto delta = + instance.c[curr][root]
                               + instance.c[root][next]
                               + park_opening_cost
                               - instance.c[curr][next];



            if(delta < config.cost) {
                config.end = curr;
                config.begin = next;
                config.root = root;
                config.cost = delta;
                config.root_opening_position = root_opening_position;
                config.root_opening_main_route_update = root_opening_main_route_update;
            }

            curr = next;
            next = route->next(next);
        } while(curr != route_initial_vertex);

    }

}

std::list<route*>& xsttrp_wip_solution::get_routes_from(int k) {
    return routes[k];
}

int xsttrp_wip_solution::get_routes_num() {

    auto num = 0;
    for (int k = 0; k < instance.parking_locations_num + 1; k++) {
        num += routes[k].size();
    }

    return num;

}

void xsttrp_wip_solution::define_from_set_partitioning(set_partitioning_pool &hq_pool, double *x_cplex, int columns_cplex) {

    reset();
    auto num = 0;
    cost = 0;
    cost_is_up_to_date = false;

    for(auto n = 0; n< columns_cplex; n++) {
        if(x_cplex[n] > TOLERANCE) {

            num++;

            const auto root = hq_pool.route_seq[n][0];
            auto rtype = route_type::sub_route;
            if(root == instance.depot) {
                rtype = route_type::main_route;
            }

            auto route = routes_pool.request(root, rtype);
            for(auto p = 1; p < hq_pool.route_len[n]-1; p++){
                route->append(hq_pool.route_seq[n][p]);
                vertex_served_by_route_type[hq_pool.route_seq[n][p]] = rtype;
            }

            const auto k = root - instance.parking_locations_begin;
            routes[k].push_back(route);

        }
    }

    if(!num) {
        cost = FLT_MAX;
    }

    this->initialize_route_iterators_and_pointers();
    this->update_move_generators(PI_BASE, *this);


}

void xsttrp_wip_solution::add_routes_to_set_partitioning(set_partitioning_pool &pool) {

    if(pool.size() < pool.max_size()) {

        for (auto k = 0; k <= instance.parking_locations_num; k++) {
            for (auto& route : routes[k]) { // era best_solution

                if(pool.size() < pool.max_size()) {
                    pool.lookup_and_insert_if_not_exists(route);
                }

            }

        }

    }

}

xsttrp_solution* xsttrp_wip_solution::take_snapshot() {

    return new xsttrp_solution(instance, *this);

}
