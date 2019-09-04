//
// Created by acco on 8/18/18.
//

#include "routes_slab.hpp"


routes_slab::routes_slab(const xsttrp_instance &instance) {

    const auto num = instance.matrix_size * 5;

    pool.reserve(num);

    for(auto n = 0; n < num; n++) {
        pool.emplace_back(instance, DUMMY_ROOT, route_type::none);
        const auto route = &pool[n];
        pointers.push(route);

    }

}

route *routes_slab::request(int root, route_type type)  {

    assert(!pointers.empty());

    const auto route = pointers.top();
    pointers.pop();

    route->reset(root, type);

    return route;

}

void routes_slab::release(route *route) {

        pointers.push(route);

    }

