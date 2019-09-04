//
// Created by acco on 8/17/18.
//

#include <cmath>
#include <algorithm>
#include "set_partitioning_pool.hpp"
#include "mm.hpp"
#include "route.hpp"
#include "xsttrp_instance.hpp"

set_partitioning_pool::set_partitioning_pool(const xsttrp_instance &p_instance) :instance(p_instance) {

    words_num = static_cast<unsigned int>(std::ceil(instance.matrix_size / 30.0));
    word = mm::request_raw_contiguous_memory<unsigned int>(instance.matrix_size);
    bit = mm::request_raw_contiguous_memory<unsigned int>(instance.matrix_size);

    int pow = 0;
    for (int i = 0; i < instance.matrix_size; i++) {
        word[i] = static_cast<unsigned int>(i / 30.0);
        bit[i] = static_cast<unsigned int>(std::pow(2.0, pow));
        pow++;
        if (pow == 30) { pow = 0; }
    }

    std::vector<short> q_copy(instance.matrix_size);
    for(auto i = 0; i < instance.matrix_size; i++){
        q_copy[i] = instance.q[i];
    }
    std::sort(q_copy.begin(), q_copy.end(), [](const short i, const short j){return i < j;});
    auto sum = 0;
    auto num = 0;
    for(; num < instance.matrix_size; num++){
        sum+=q_copy[num];
        if(sum > instance.truck_capacity){
            break;
        }
    }

    hash_route_len = static_cast<unsigned int>(num + 1);

    head = mm::request_raw_contiguous_memory<unsigned int[HASH_LOAD][BUCKETS_NUM]>(instance.matrix_size);
    next = mm::request_raw_contiguous_memory<unsigned int>(SET_PARTITIONING_MAX_POOL_SIZE);

    entries_num = 0;

    route_seq = mm::request_raw_contiguous_memory<unsigned short>(SET_PARTITIONING_MAX_POOL_SIZE, instance.matrix_size);
    route_load = mm::request_raw_contiguous_memory<unsigned short>(SET_PARTITIONING_MAX_POOL_SIZE);
    route_cost = mm::request_raw_contiguous_memory<float>(SET_PARTITIONING_MAX_POOL_SIZE);
    route_len = mm::request_raw_contiguous_memory<unsigned short>(SET_PARTITIONING_MAX_POOL_SIZE);
    route_set = mm::request_raw_contiguous_memory<unsigned int>(SET_PARTITIONING_MAX_POOL_SIZE, words_num);

}

set_partitioning_pool::~set_partitioning_pool() {

    mm::release_raw_contiguous_memory(word);
    mm::release_raw_contiguous_memory(bit);

    mm::release_raw_contiguous_memory(head);
    mm::release_raw_contiguous_memory(next);

    mm::release_raw_contiguous_memory(route_seq);
    mm::release_raw_contiguous_memory(route_load);
    mm::release_raw_contiguous_memory(route_cost);
    mm::release_raw_contiguous_memory(route_len);
    mm::release_raw_contiguous_memory(route_set);

}

unsigned int set_partitioning_pool::lookup_and_insert_if_not_exists(route* r) {

    assert(entries_num < SET_PARTITIONING_MAX_POOL_SIZE);


    const auto insertion_position = entries_num;

    #ifdef RTGUARD
    for(auto n = 0u; n < words_num; n++){
        assert(route_set[insertion_position][n] == 0);
    }
    #endif

    route_len[insertion_position] = 0;
    route_cost[insertion_position] = r->cost();
    route_load[insertion_position] = r->load();

    for(auto n = 0, node = r->begin(); n < r->size() + 1; n++, node = r->next(node)) {
        route_set[insertion_position][word[node]] |= bit[node];
        route_seq[insertion_position][route_len[insertion_position]++] = static_cast<unsigned short>(node);
    }

    auto key = 0ul;
    for (auto w = 0u; w < words_num; w++) {
        key += route_set[insertion_position][w];
    }
    key %= BUCKETS_NUM;

    const auto l = r->load() % HASH_LOAD;

    const auto j = r->size() % hash_route_len;

    auto curr = head[j][l][key];

    while (curr) {
        // also make sure they have the same root otherwise
        // things like
        // 36 25 12 39 19 28 3 36
        // 28 19 39 12 25 36 3 28
        // appear to be the same but are actually rooted in different satellites!
        if (route_load[curr] != route_load[insertion_position] || route_len[curr] != route_len[insertion_position] || route_seq[curr][0] != route_seq[insertion_position][0]) {
            goto notFound;
        }

        for (auto w = 0u; w < words_num; w++) {
            if (route_set[curr][w] != route_set[insertion_position][w]) {
                goto notFound;
            }
        }

        // *******
        //  FOUND
        // *******

        for (auto w = 0u; w < words_num; w++) {
            route_set[insertion_position][w] = 0;
        }

        if(route_cost[insertion_position] + TOLERANCE < route_cost[curr]) {


            //std::cout << "** " << route_cost[curr] << " -> " << route_cost[insertion_position] << " ** \t";

            route_cost[curr] = route_cost[insertion_position];
            for(auto n = 0; n < route_len[curr]; n++){
                route_seq[curr][n] = route_seq[insertion_position][n];
            }

            //r->print();

        }

        return curr;

        notFound:
        curr = next[curr];

    }

    // not found
    next[insertion_position] = head[j][l][key];
    head[j][l][key] = insertion_position;

    //std::cout << "+ " << insertion_position << " + \t";
    //r->print();

    entries_num++;

    return insertion_position;

}


