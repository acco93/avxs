//
// Created by acco on 10/17/18.
//

#ifndef XSTTRP_AVXS_RESULT_HPP
#define XSTTRP_AVXS_RESULT_HPP



#include <vector>
#include <chrono>
#include "xsttrp_solution.hpp"



#ifdef STATS
struct sample_point {
    unsigned long time;
    float objective_value;
};
#endif

class avxs_result {

    unsigned long seed = 0;

    xsttrp_solution* final_solution= nullptr;
    float cost_after_set_partitioning = 0;
    unsigned long total_time_seconds = 0;

    unsigned long set_partitioning_time_seconds = 0;
    float cost_before_set_partitioning = 0;
    unsigned long set_partitioning_pool_size = 0;

    #ifdef STATS
    std::chrono::time_point<std::chrono::system_clock> run_started_at = std::chrono::high_resolution_clock::now();



    float initial_improvement = 0;
    float relocate_improvement = 0;
    float swap_improvement = 0;
    float twopt_improvement = 0;
    float rootref_improvement = 0;
    float segswap_improvement = 0;

    unsigned long relocate_executions = 0;
    unsigned long swap_executions = 0;
    unsigned long twopt_executions = 0;
    unsigned long rootref_executions = 0;
    unsigned long segswap_executions = 0;

    float sunload_improvement = 0;
    float srem_improvement = 0;


    std::vector<sample_point> evolution_points;
    #endif



public:

    explicit avxs_result(unsigned long seed);
    avxs_result(const avxs_result& other);
   // avxs_result& operator=(const avxs_result& other);

    unsigned long get_seed() const;

    virtual ~avxs_result();

    void set_final_solution(xsttrp_solution* solution);
    xsttrp_solution * get_final_solution() const;

    void set_cost_after_set_partitioning(float value);
    float get_cost_after_set_partitioning() const;

    void set_total_time_seconds(unsigned long time);
    unsigned long get_total_time_seconds() const;

    void print() const;

    void set_cost_before_set_partitioning(float value);
    float get_cost_before_set_partitioning() const;

    void set_set_partitioning_time_seconds(unsigned long time);
    unsigned long get_set_partitioning_time_seconds() const;


    void set_set_partitioning_pool_size(unsigned long size);
    unsigned long get_set_partitioning_pool_size() const;

    #ifdef STATS

    void add_sample(float objective_value);
    bool has_sample(unsigned long iteration);
    sample_point get_sample(unsigned long iteration) const;

    void add_initial_improvement(float value);
    void add_before_any_shake(float value);

    void add_sunload(float value);
    float get_sunload_improvement() const;

    void add_srem(float value);
    float get_srem_improvement() const;


    void add_relocate(float value);
    float get_relocate_improvement() const;
    unsigned long get_relocate_successful_applications() const;

    void add_swap(float value);
    float get_swap_improvement() const;
    unsigned long get_swap_successful_applications() const;

    void add_twopt(float value);
    float get_twopt_improvement() const;
    unsigned long get_twopt_successful_applications() const;

    void add_rootref(float value);
    float get_rootref_improvement() const;
    unsigned long get_rootref_successful_applications() const;

    void add_segswap(float value);
    float get_segswap_improvement() const;
    unsigned long get_segswap_successful_applications() const;

    #endif

    void copy(const avxs_result &result);


    void set_instance_parsing_time(long time);

    long instance_parsing_time;
};



#endif //XSTTRP_AVXS_RESULT_HPP
