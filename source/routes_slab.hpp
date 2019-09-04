//
// Created by acco on 7/23/18.
//

#ifndef XSTTRP_ROUTES_SLAB_HPP
#define XSTTRP_ROUTES_SLAB_HPP

#include <vector>
#include <stack>
#include <cassert>
#include <iostream>

#include "globals.hpp"

#include "xsttrp_instance.hpp"
#include "route.hpp"
#include "route_type.hpp"

#define DUMMY_ROOT (0)

class routes_slab {

private:

    std::vector<route> pool;
    std::stack<route*> pointers;

public:

    explicit routes_slab(const xsttrp_instance& instance);

    route* request(int root, route_type type);

    void release(route* route);

};


#endif //GSTTRP_ROUTES_SLAB_HPP
