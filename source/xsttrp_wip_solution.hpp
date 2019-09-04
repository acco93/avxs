//
// Created by acco on 7/23/18.
//

#ifndef GSTTRP_SOLVER2_SOLUTION_HPP
#define GSTTRP_SOLVER2_SOLUTION_HPP


#include <random>
#include <list>
#include "globals.hpp"
#include "xsttrp_solution.hpp"
#include "xsttrp_instance.hpp"
#include "arc.hpp"
#include "route_type.hpp"
#include "avxs_result.hpp"
#include "set_partitioning_pool.hpp"
#include "avxs_parameters.hpp"

#define INVALID_CACHE (-1)


class route;
class routes_slab;


class xsttrp_wip_solution {



    template<class T>
    static void knuth_shuffle(std::vector<T> &elements,
                              std::mt19937 &randEngine) {
        const size_t len = elements.size();
        for (size_t i = 0; i < len; i++) {
            const auto r = static_cast<const int>(randEngine() % len);
            const T temp = elements[i];
            elements[i] = elements[r];
            elements[r] = temp;
        }
    }

    template <class T,class V>
    static void knuth_shuffle(std::vector<T> &elements1,
                              std::vector<V> &elements2,
                              std::mt19937 &randEngine) {

        if(elements1.size() != elements2.size()) {return;}

        const size_t len = elements1.size();
        for (size_t i = 0; i < len; i++) {
            const auto r = static_cast<const int>(randEngine() % len);

            const T temp1 = elements1[i];
            elements1[i] = elements1[r];
            elements1[r] = temp1;

            const V temp2 = elements2[i];
            elements2[i] = elements2[r];
            elements2[r] = temp2;

        }

    }



    int roulette_wheel_selection(const int *elements,
                                 const float *probabilities,
                                 const int size,
                                 std::mt19937 &randEngine) {

        auto probabilitiesSum = 0.0f;
        for (auto k = 0; k < size; k++) {
            probabilitiesSum += probabilities[k];
        }

        if(probabilitiesSum <= 0) {return -1;}

        const double r01 = double(randEngine()) / double(std::mt19937::max());
        const double thr = r01 * probabilitiesSum;

        double sum = 0;
        int idx = 0;

        for (; idx < size; idx++) {
            sum += probabilities[idx];
            if (sum > thr) {
                break;
            }
        }

        return elements[idx];

    }


    const xsttrp_instance& instance;
    std::mt19937& rand_engine;

    int** candidate_list;
    float** candidate_list_probabilities;

    std::vector<arc>& sub_arcs;
    std::vector<arc>& main_arcs;
    routes_slab& routes_pool;

    bool cost_is_up_to_date;
    double cost;

    std::vector<std::vector<int>> park_assignments;

    typedef bool (xsttrp_wip_solution::*local_search_function_type) ();
    std::vector<local_search_function_type> local_search_procedures;



    std::vector<int> customers_permutation;
    std::vector<int> parks_permutation;
    std::vector<int> vertices_permutation;

    std::vector<int> unrouted_customers;
    std::vector<int> open_parking_locations;

    // Given a parking location kLoc, main_route_insertion_position_cache[kLoc] might be
    // (0) INVALID_CACHE
    // (1) a feasible insertion position
    // if (0) it is necessary to scan the main_route to find the best insertion position for kLoc and
    // update
    // (0.a) main_route_insertion_position_cache[kLoc] with the best insertion position
    // (0.b) main_route_insertion_cost_cache[kLoc] with the best insertion cost
    // if (1) just read the values from the arrays
    // n.b. make sure to invalidate the cache when a main_route change occurs
    std::vector<int> main_route_insertion_position_cache;
    std::vector<float> main_route_insertion_cost_cache;

    std::vector<std::list<route*>> routes;
    std::vector<route_type> vertex_served_by_route_type;
    typedef std::list<route*>::iterator route_iterator;
    std::vector<route_iterator> route_iterators;
    std::vector<route*> route_pointers;

    std::vector<std::vector<int>> movegen_truck_customers;
    std::vector<std::vector<int>> movegen_vehicle_customers_no_park;
    std::vector<std::vector<int>> movegen_vehicle_customers_yes_park;
    std::vector<std::vector<int>> movegen_satellites;

    std::vector<std::vector<std::vector<int>>*> movegen_set_customers;
    std::vector<std::vector<std::vector<int>>*> movegen_set_all;
    std::vector<std::vector<int>> movegen_park_to_park;
    std::vector<std::vector<int>> movegen_main_vertices;
    bool** movegen_is_present;

    int shake_type;

    void granular_local_search(avxs_result &result);

    void initialize_from_scratch();
    void initialize_from_solution(const xsttrp_wip_solution &other);
    void release_resources();
    void initialize_route_iterators_and_pointers();

    struct bulk_roots_route_setup {
        int end;
        int root;
        int begin;
        float cost;
        int root_opening_position;
        bool root_opening_main_route_update;
    };


    void build_initial_solution();



    void clarke_and_wright_savings_algorithm(int k,
                                             int capacity,
                                             route_type r_type);

    void update_move_generators(float pi,
                                xsttrp_wip_solution& best_solution);

    void shake_up();

    float remove_vertex(route* route, int i);
    float (xsttrp_wip_solution::*remove_route)(route *route, route_iterator &iterator);
    float remove_route_xsttrp(route *route, route_iterator &iterator);
    float remove_route_mdvrp(route *route, route_iterator &iterator);



    float (xsttrp_wip_solution::*remove_satellite)(int kLoc);
    float remove_satellite_xsttrp(int kLoc);
    float remove_satellite_mdvrp(int kLoc);


    void identify_open_parking_locations(std::vector<int> &storage);

    void remove_all_customers_from_route(route *route, std::vector<int> &storage);

    void (xsttrp_wip_solution::*reset_main_route_insertion_cache)();
    void reset_main_route_insertion_cache_xsttrp();
    void reset_main_route_insertion_cache_mdvrp();

    void perform_best_insertion(int i);

    void build_new_one_customer_sub_route(int customer, int kLoc);

    void add_vertex_to_main_route(int vertex, int position);

    void add_customer_to_route(int customer, int position, const route_iterator &iter);

    float (xsttrp_wip_solution::*get_park_opening_cost)(int kLoc, int &position, bool &main_route_update);
    float get_park_opening_cost_xsttrp(int kLoc, int &position, bool &main_route_update);
    float get_park_opening_cost_mdvrp(int kLoc, int &position, bool &main_route_update);

    bool local_search_move_1_0();

    void local_search_move_1_0_eval_route_insertion(int i,
                                                    int j,
                                                    float &best_cost,
                                                    int &best_customer_position,
                                                    bool &best_in_new_one_customer_route,
                                                    bool &best_needed_main_route_update,
                                                    route_iterator &best_route_iter);

    void local_search_move_1_0_eval_park_insertion(int i,
                                                   int j,
                                                   float &best_cost,
                                                   int &best_customer_position,
                                                   bool &best_in_new_one_customer_route,
                                                   bool &best_needed_main_route_update,
                                                   route_iterator &best_route_iter,
                                                   int &best_park,
                                                   int &best_park_position);

    bool local_search_move_1_1();
    void local_search_move_1_1_eval_swap(int i,
                                         const route *iRoute,
                                         float iRemoval,
                                         int iNext,
                                         int iPrev,
                                         int j,
                                         float &best_cost,
                                         int &best_j);

    bool local_search_2_opt();

    float detach_route(std::list<route *, std::allocator<route *>>::iterator iterator);

    void identify_best_attachment_for_detached_route(route* route, std::vector<int>& movegen, bulk_roots_route_setup& config);

    bool local_search_roots_refine();

    bool local_search_segment_swap();



    #ifdef STATS
    typedef void (avxs_result::*add_ls_function)(float value);
    std::vector<add_ls_function> add_ls_functions;
    typedef void (avxs_result::*add_shake_function)(float value);
    add_shake_function add_shake;
    #endif

    float previous_pi;
    unsigned int previous_main_arcs_index;
    unsigned int previous_sub_arcs_index;

    bool is_feasible();

    void reset();
public:


    xsttrp_wip_solution(const xsttrp_instance &instance,
                    std::mt19937& rand_engine,
                    int **candidate_list,
                    float **candidate_list_probabilities,
                    std::vector<arc>& sub_arcs,
                    std::vector<arc>& main_arcs,
                    routes_slab& routes_slab);

    xsttrp_wip_solution(const xsttrp_wip_solution& other);

    xsttrp_wip_solution& operator=(const xsttrp_wip_solution& other);

    virtual ~xsttrp_wip_solution();


    void print();
    float get_cost();
    int get_routes_num();
    std::list<route*>& get_routes_from(int k);
    void define_from_set_partitioning(set_partitioning_pool &hq_pool, double *x_cplex, int columns_cplex);
    int build_and_improve(set_partitioning_pool &hq_pool, avxs_result &result);

    xsttrp_solution* take_snapshot();


    void add_routes_to_set_partitioning(set_partitioning_pool &pool);


};





#endif //GSTTRP_SOLVER2_SOLUTION_HPP
