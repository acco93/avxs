//
// Created by acco on 10/17/18.
//


#include <iostream>
#include "avxs_result.hpp"

avxs_result::avxs_result(unsigned long seed_param) {
    seed = seed_param;
}

avxs_result::avxs_result(const avxs_result &other) {
    copy(other);
}

/*avxs_result &avxs_result::operator=(const avxs_result &other) {
    copy(other);
    return *this;
}*/

void avxs_result::copy(const avxs_result &result) {

    seed = result.seed;

    final_solution = new xsttrp_solution(*result.final_solution);
    cost_after_set_partitioning = result.cost_after_set_partitioning;
    total_time_seconds = result.total_time_seconds;

    set_partitioning_time_seconds = result.set_partitioning_time_seconds;
    cost_before_set_partitioning = result.cost_before_set_partitioning;
    set_partitioning_pool_size = result.set_partitioning_pool_size;

    instance_parsing_time = result.instance_parsing_time;

    #ifdef STATS
    run_started_at = result.run_started_at;

    initial_improvement = result.initial_improvement;
    relocate_improvement = result.relocate_improvement;
    swap_improvement = result.swap_improvement;
    twopt_improvement = result.twopt_improvement;
    rootref_improvement = result.rootref_improvement;
    segswap_improvement = result.segswap_improvement;

    relocate_executions = result.relocate_executions;
    swap_executions = result.swap_executions;
    twopt_executions = result.twopt_executions;
    rootref_executions = result.rootref_executions;
    segswap_executions = result.segswap_executions;

    sunload_improvement = result.sunload_improvement;
    srem_improvement = result.srem_improvement;

    evolution_points = result.evolution_points;
    #endif

}


avxs_result::~avxs_result() {
    delete final_solution;
}

void avxs_result::set_cost_after_set_partitioning(float value) {
    cost_after_set_partitioning = value;
}

void avxs_result::set_total_time_seconds(unsigned long time) {
    total_time_seconds = time;
    if(total_time_seconds == 0) {
        total_time_seconds = 1;
    }
}

void avxs_result::set_final_solution(xsttrp_solution* solution) {
    final_solution = solution;
}

float avxs_result::get_cost_after_set_partitioning() const {
    return cost_after_set_partitioning;
}

unsigned long avxs_result::get_total_time_seconds() const {
    return total_time_seconds+instance_parsing_time;
}

xsttrp_solution * avxs_result::get_final_solution() const {
    return final_solution;
}

unsigned long avxs_result::get_seed() const {
    return seed;
}

void avxs_result::print() const {

    #ifdef STATS
    for(auto n = 0u; n < evolution_points.size(); n++){
        auto& point = evolution_points[n];
        std::cout << n << "\t" << point.time << "\t"<< point.objective_value << "\n";
    }
    #endif
}

void avxs_result::set_cost_before_set_partitioning(float value) {
    cost_before_set_partitioning = value;
}

void avxs_result::set_set_partitioning_time_seconds(unsigned long time) {
    set_partitioning_time_seconds = time;
}

void avxs_result::set_set_partitioning_pool_size(unsigned long size) {
    set_partitioning_pool_size = size;
}

float avxs_result::get_cost_before_set_partitioning() const {
    return cost_before_set_partitioning;
}

unsigned long avxs_result::get_set_partitioning_time_seconds() const {
    return set_partitioning_time_seconds;
}

unsigned long avxs_result::get_set_partitioning_pool_size() const {
    return set_partitioning_pool_size;
}

void avxs_result::set_instance_parsing_time(long time) {
    instance_parsing_time = time;
}


#ifdef STATS

void avxs_result::add_sample(float objective_value) {
    unsigned long time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - run_started_at).count();
    evolution_points.push_back({time, objective_value});
}

void avxs_result::add_sunload(float value) {
    if(value > TOLERANCE) {
        sunload_improvement += value;
    }
}

void avxs_result::add_srem(float value) {
    if(value > TOLERANCE) {
        srem_improvement += value;
    }
}

void avxs_result::add_relocate(float value) {
    if(value > TOLERANCE) {
        relocate_improvement += value;
        relocate_executions++;
    }
}

void avxs_result::add_swap(float value) {
    if(value > TOLERANCE) {
        swap_improvement += value;
        swap_executions++;
    }
}

void avxs_result::add_twopt(float value) {
    if(value > TOLERANCE) {
        twopt_improvement += value;
        twopt_executions++;
    }
}

void avxs_result::add_rootref(float value) {
    if(value > TOLERANCE) {
        rootref_improvement += value;
        rootref_executions++;
    }
}

void avxs_result::add_segswap(float value) {
    if(value > TOLERANCE) {
        segswap_improvement += value;
        segswap_executions++;
    }
}

void avxs_result::add_before_any_shake(__attribute__ ((unused)) float value) {
}

void avxs_result::add_initial_improvement(float value) {
    initial_improvement += value;
}

bool avxs_result::has_sample(unsigned long iteration) {
    return iteration < evolution_points.size();
}

sample_point avxs_result::get_sample(unsigned long iteration) const {
    return evolution_points[iteration];
}


float avxs_result::get_relocate_improvement() const {
    return relocate_improvement;
}

unsigned long avxs_result::get_relocate_successful_applications() const {
    return relocate_executions;
}

float avxs_result::get_swap_improvement() const {
    return swap_improvement;
}

unsigned long avxs_result::get_swap_successful_applications() const {
    return swap_executions;
}

float avxs_result::get_twopt_improvement() const {
    return twopt_improvement;
}

unsigned long avxs_result::get_twopt_successful_applications() const {
    return twopt_executions;
}

float avxs_result::get_rootref_improvement() const {
    return rootref_improvement;
}

unsigned long avxs_result::get_rootref_successful_applications() const {
    return rootref_executions;
}

float avxs_result::get_segswap_improvement() const {
    return segswap_improvement;
}

unsigned long avxs_result::get_segswap_successful_applications() const {
    return segswap_executions;
}

float avxs_result::get_sunload_improvement() const {
    return sunload_improvement;
}

float avxs_result::get_srem_improvement() const {
    return srem_improvement;
}


#endif





