//
// Created by acco on 8/17/18.
//

#ifndef GSTTRP_SP_POOL_HPP
#define GSTTRP_SP_POOL_HPP


#include "globals.hpp"

class xsttrp_instance;
class route;

class set_partitioning_pool {

    const xsttrp_instance& instance;

    unsigned int words_num;
    unsigned int *word;
    unsigned int *bit;

    static const unsigned int HASH_LOAD = 257;
    static const unsigned int BUCKETS_NUM = 7057;
    unsigned int hash_route_len = 0;

    unsigned int(*head)[HASH_LOAD][BUCKETS_NUM] = nullptr;
    unsigned int *next = nullptr;

    unsigned short* route_load = nullptr;
    unsigned int** route_set = nullptr;

    unsigned int entries_num = 0;

public:

    explicit set_partitioning_pool(const xsttrp_instance& instance);

    virtual ~set_partitioning_pool();

    inline unsigned int size() {
        return entries_num;
    }

    inline unsigned int max_size() {
        return SET_PARTITIONING_MAX_POOL_SIZE - SET_PARTITIONING_POOL_RESERVED_SPACE;
    }

    inline unsigned int begin(){
        return 0;
    }

    inline unsigned int end() {
        return entries_num;
    }

    unsigned int lookup_and_insert_if_not_exists(route* r);

    float* route_cost;
    unsigned short** route_seq;
    unsigned short* route_len;
};


#endif //GSTTRP_SP_POOL_HPP
