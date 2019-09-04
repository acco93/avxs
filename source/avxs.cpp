//
// Created by acco on 10/16/18.
//

#include "avxs.hpp"
#include "timer.hpp"
#include "mm.hpp"
#include "routes_slab.hpp"
#include "utils.hpp"

void use_b_rank_assignment_probability_function(const xsttrp_instance& instance, int **candidate_list, float **candidate_list_probabilities, unsigned long b);
void use_d_near_assignment_probability_function(const xsttrp_instance& instance, int **candidate_list, float **candidate_list_probabilities, unsigned long d);
void arcs_definition(const xsttrp_instance& instance, std::vector<arc>& sub_arcs, std::vector<arc>& main_arcs);
void solve_set_partitioning(const xsttrp_instance &instance, set_partitioning_pool &pool, xsttrp_wip_solution &solution);

/*
 * AVXS algorithm
 */
avxs_result avxs(const xsttrp_instance &instance, unsigned long seed, avxs_parameters& parameters) {

    avxs_result result(seed);

    auto stopwatch = timer();

    stopwatch.start();

    // Structures to store assignment fitness function and sorted candidates
    auto candidate_list = mm::request_raw_contiguous_memory<int>(instance.customers_num, instance.parking_locations_num + 1);
    auto candidate_list_probabilities = mm::request_raw_contiguous_memory<float>(instance.customers_num, instance.parking_locations_num + 1);

    // Check if the users set some parameters for the assignment functions
    if(parameters.use_rank_fitness == parameters.use_near_fitness) {

        // Falling back to defaults!

        if(instance.ptype == problem_type::lrp) {

            use_d_near_assignment_probability_function(instance, candidate_list, candidate_list_probabilities, LRP_NEAR_FACTOR);

        } else {

            use_d_near_assignment_probability_function(instance, candidate_list, candidate_list_probabilities, XSTTRP_NEAR_FACTOR);

        }

    } else {

        // Using user-defined parameters

        if(parameters.use_rank_fitness) {
            use_b_rank_assignment_probability_function(instance, candidate_list, candidate_list_probabilities, parameters.rank_factor);
        } else {
            use_d_near_assignment_probability_function(instance, candidate_list, candidate_list_probabilities, parameters.near_factor);
        }

    }

    // shared container of empty routes
    auto routes_cache = routes_slab(instance);

    // sub-level and main-level arcs
    auto sub_arcs = std::vector<arc>();
    auto main_arcs = std::vector<arc>();
    // define them according to some criteria (they will be used in the cost-based granular sparsification, thus they
    // are sorted according to a non-decreasing ordering
    arcs_definition(instance, sub_arcs, main_arcs);

    // data structure that maintains a set of unique routes (it checks for duplicates and updates improved versions
    // of the same route if necessary)
    auto hq_pool = set_partitioning_pool(instance);

    auto rand_engine = std::mt19937(seed);

    auto current_solution = xsttrp_wip_solution(instance, rand_engine, candidate_list, candidate_list_probabilities, sub_arcs, main_arcs, routes_cache);
    auto best_solution = xsttrp_wip_solution(instance, rand_engine, candidate_list, candidate_list_probabilities, sub_arcs, main_arcs, routes_cache);

    auto total_iterations = 0;

    for(auto restart = 0u; restart < RESTARTS_NUM; restart++) {

        #ifdef STATS

        auto message = std::to_string(total_iterations) + " iter (" + std::to_string(stopwatch.lap()) +" s) z = " + std::to_string(best_solution.get_cost()) + " |P| = " + std::to_string(hq_pool.size());
        print_progress(restart, RESTARTS_NUM, message);

        #endif

        total_iterations += current_solution.build_and_improve(hq_pool, result);

        if(current_solution.get_cost() + TOLERANCE < best_solution.get_cost()) {
            best_solution = current_solution;
        }

    }


    result.set_set_partitioning_pool_size(hq_pool.size());
    result.set_cost_before_set_partitioning(best_solution.get_cost());

    timer sp_stopwatch;

    sp_stopwatch.start();
    #ifdef POLISHING_PHASE
    solve_set_partitioning(instance, hq_pool, best_solution);
    #endif
    sp_stopwatch.stop();

    const auto elapsed_sp = sp_stopwatch.elapsed();
    result.set_set_partitioning_time_seconds(elapsed_sp);

    #if defined STATS

    auto message = std::to_string(total_iterations) + " iter (" + std::to_string(stopwatch.lap()) +" s) z = " + std::to_string(best_solution.get_cost()) + " |P| = " + std::to_string(hq_pool.size()) + " z* = " + std::to_string(best_solution.get_cost());
    print_progress_end(message);
    std::cout << "\n";

    #endif

    result.set_cost_after_set_partitioning(best_solution.get_cost());


    const auto best_solution_snapshot = best_solution.take_snapshot();
    result.set_final_solution(best_solution_snapshot);

    mm::release_raw_contiguous_memory(candidate_list);
    mm::release_raw_contiguous_memory(candidate_list_probabilities);

    stopwatch.stop();
    const auto elapsed_tot = stopwatch.elapsed();
    result.set_total_time_seconds(elapsed_tot);

    return result;
}



