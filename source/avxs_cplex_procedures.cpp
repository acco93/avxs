//
// Created by acco on 10/16/18.
//
// Cplex interface.
//


#include "avxs.hpp"
#include "mm.hpp"
#include <ilcplex/ilocplex.h>

void add_some_columns(CPXENVptr& env,
                      CPXLPptr& lp,
                      const xsttrp_instance& instance,
                      set_partitioning_pool& set_partitioning_routes_pool,
                      double* obj,
                      CPXNNZ* mat_beg,
                      int* mat_ind,
                      double* mat_val,
                      const int mip_model_rows_number,
                      const int begin,
                      const int end) {

    const auto columns_num = end - begin;
    const auto non_zeros_num = mip_model_rows_number * columns_num;

    long long progressive_index = 0;

    for(auto c = 0; c < columns_num; c++) {
        obj[c] = set_partitioning_routes_pool.route_cost[begin+c];
        mat_beg[c] = c * mip_model_rows_number;
        for(auto r = 0; r < mip_model_rows_number; r++) {
            mat_ind[progressive_index] = r;
            mat_val[progressive_index] = 0.0;
            progressive_index++;
        }
    }




    for(auto n = begin, column_index = 0; n < end; n++, column_index++) {

        const long long base_index = column_index * mip_model_rows_number;

        unsigned short* route = set_partitioning_routes_pool.route_seq[n];
        unsigned short route_len = set_partitioning_routes_pool.route_len[n];



        const auto constraint2_base_position = base_index;



        auto constraint5_base_position = base_index + mip_model_rows_number - 1;

        // in the mdvrp we have 'parking_locations_num' constraints more
        if(instance.ptype == mdvrp) {
            constraint5_base_position -= instance.parking_locations_num;
        }



        for(auto p = 1; p < route_len - 1; p++) {
            const auto vertex = route[p];
            if(instance.is_customer(vertex)) {
                // skip satellites
                const auto position = constraint2_base_position + vertex;
                mat_val[position] = 1;
            }
        }

        const auto kLoc = route[0];

        if(kLoc == instance.depot) {

            for (auto p = 1; p < route_len - 1; p++) {
                const auto vertex = route[p];
                if(instance.is_parking_location(vertex)) {
                    const auto kPark = vertex - instance.parking_locations_begin;
                    const auto constraint4_base_position = base_index + instance.customers_num + kPark * instance.customers_num;
                    for(auto i = 0; i < instance.customers_num; i++){
                        mat_val[constraint4_base_position + i] = -1;
                    }
                }
            }


            mat_val[constraint5_base_position] = 1;


        } else {

            const auto k = kLoc - instance.parking_locations_begin;

            const auto constraint4_base_position = base_index + instance.customers_num + (k * instance.customers_num);

            for (auto p = 1; p < route_len - 1; p++) {
                const auto vertex = route[p];
                mat_val[constraint4_base_position + vertex] = 1;
            }


            if(instance.ptype == mdvrp) {
                auto last = k + constraint5_base_position + 1;
                mat_val[last] = 1;
            }

        }



    }

    const auto status = CPXXaddcols(env, lp, columns_num, non_zeros_num, obj, mat_beg, mat_ind, mat_val, nullptr, nullptr, nullptr);

    if (status) {
        std::cout << "Fatal error while using CPLEX: error while adding columns! Code = %d. Aborting.\n";
        exit(1);
    }

}

void solve_set_partitioning(const xsttrp_instance &instance, set_partitioning_pool &pool, xsttrp_wip_solution &solution) {

    auto status = 0;

    auto env = CPXXopenCPLEX(&status);
    auto lp = CPXXcreateprob(env, &status, "F");
    status = CPXXchgobjsen(env, lp, CPX_MIN);

    auto mip_model_rows_number =  + instance.customers_num                                        // constraints  (2) and (3)
                                  + (instance.parking_locations_num * instance.customers_num)     // constraints  (4)
                                  + 1;                                                             // constraint   (5)
    if(instance.ptype == mdvrp) {
        mip_model_rows_number +=  + instance.parking_locations_num;                               // mdvrp max-vehicles constraints
    }


    const auto max_columns_at_once = 1000;

    auto sense = mm::request_raw_contiguous_memory<char>(static_cast<const size_t>(mip_model_rows_number));
    auto rhs = mm::request_raw_contiguous_memory<double>(static_cast<const size_t>(mip_model_rows_number));

    int rowIndex = 0;

    /**
     * Constraints 2 - 3
     */
    for (int i = 0; i < instance.customers_num; i++) {
        sense[rowIndex] = 'E';
        rhs[rowIndex] = 1.0;
        rowIndex++;
    }


    /**
     * Constraints 4
     */
    for(int kLoc = instance.parking_locations_begin; kLoc < instance.parking_locations_end; kLoc++){
        for(int i = instance.customers_begin; i < instance.customers_end; i++){
            // avoid skipping mDepot == i
            sense[rowIndex] = 'L';
            rhs[rowIndex] = 0.0;
            rowIndex++;
        }
    }

    /**
     * Constraint 5
     */
    sense[rowIndex] = 'E';
    rhs[rowIndex] = 1.0;
    rowIndex++;

    if(instance.ptype == mdvrp) {
        for (int kLoc = instance.parking_locations_begin; kLoc < instance.parking_locations_end; kLoc++) {
            sense[rowIndex] = 'L';
            rhs[rowIndex] = static_cast<float>(instance.mdvrp_vehicles_num);
            rowIndex++;
        }
    }


    /**
     * Create the rows
     */
    status = CPXXnewrows(env, lp, mip_model_rows_number, rhs, sense, nullptr, nullptr);
    if (status) {
        std::cout << "Fatal error while using CPLEX: error while defining rows. Aborting.\n";
        exit(1);
    }

    mm::release_raw_contiguous_memory(sense);
    mm::release_raw_contiguous_memory(rhs);


    const auto max_non_zero_at_once = mip_model_rows_number * max_columns_at_once;

    auto obj = mm::request_raw_contiguous_memory<double>(max_columns_at_once);
    auto mat_beg = mm::request_raw_contiguous_memory<CPXNNZ>(max_columns_at_once);
    auto mat_ind = mm::request_raw_contiguous_memory<int>(max_non_zero_at_once);
    auto mat_val = mm::request_raw_contiguous_memory<double>(max_non_zero_at_once);

    // prepare the indices for the mip start
    const auto mcnt = 1;
    const auto nzcnt = solution.get_routes_num();
    int beg[1];
    beg[0] = 0;
    auto varindices = mm::request_raw_contiguous_memory<int>(nzcnt);
    auto values = mm::request_raw_contiguous_memory<double>(nzcnt);

    auto n = 0;
    for(auto k = 0; k < instance.parking_locations_num + 1; k++) {
        for (auto &route : solution.get_routes_from(k)){
            const auto index = pool.lookup_and_insert_if_not_exists(route);

            varindices[n] = index;
            values[n] = 1.0;
            n++;

        }
    }


    const auto columns_num = pool.end() - pool.begin();
    const auto chunks_num = columns_num / max_columns_at_once;

    auto current_column = pool.begin();
    for(auto chunk = 0u; chunk < chunks_num; chunk++) {

        const auto end = current_column + max_columns_at_once;
        add_some_columns(env, lp, instance, pool, obj, mat_beg, mat_ind, mat_val, mip_model_rows_number, current_column, end);

        current_column += max_columns_at_once;

    }

    add_some_columns(env, lp, instance, pool, obj, mat_beg, mat_ind, mat_val, mip_model_rows_number, current_column, pool.end());


    auto xctype = (char*)malloc(sizeof(char) * CPXgetnumcols(env, lp));
    if(!xctype) {
        std::cout << "Fatal error while requesting memory. Aborting.\n";
        exit(1);
    }


    for (int i = 0; i < CPXgetnumcols(env, lp); i++) {
        xctype[i] = 'B';
    }

    status = CPXcopyctype(env, lp, xctype);
    if (status) {
        std::cout << "Fatal error while using CPLEX: cannot set variables as binary. Aborting.\n";
        exit(1);
    }


    free(xctype);

    CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_OFF);
    CPXsetintparam(env, CPX_PARAM_MIPDISPLAY, 3);
    CPXsetintparam(env, CPX_PARAM_THREADS, 1);
    CPXsetdblparam(env, CPXPARAM_DetTimeLimit, static_cast<double>(CPLEX_TICKS_PER_SECOND * CPLEX_TIME_LIMIT));

    int effortlevel[1];
    effortlevel[0] = CPX_MIPSTART_AUTO;
    status = CPXaddmipstarts(env, lp, mcnt, nzcnt, beg, varindices, values, effortlevel, nullptr);


    if(status){
        std::cout << "Fatal error while using CPLEX: cannot set warm-start. Aborting.\n";
        exit(1);
    }

    mm::release_raw_contiguous_memory(varindices);
    mm::release_raw_contiguous_memory(values);


    status = CPXmipopt(env, lp);

    if(status){
        std::cout << "Fatal error while using CPLEX: cannot solve the MIP. Aborting.\n";
        exit(1);
    }

    double objectiveValue;
    CPXXgetobjval(env, lp, &objectiveValue);

    auto x  = mm::request_raw_contiguous_memory<double>(CPXgetnumcols(env, lp));

    CPXXgetx(env, lp, x, 0, CPXXgetnumcols(env, lp)-1);

    solution.define_from_set_partitioning(pool, x, CPXgetnumcols(env, lp));

    mm::release_raw_contiguous_memory(x);

    mm::release_raw_contiguous_memory(obj);
    mm::release_raw_contiguous_memory(mat_beg);
    mm::release_raw_contiguous_memory(mat_ind);
    mm::release_raw_contiguous_memory(mat_val);

    if (lp != nullptr) {

        status = CPXfreeprob(env, &lp);

        if(status) {
            std::cout << "Fatal error while using CPLEX: failed to free problem. Aborting.\n";
            exit(1);
        }

    }

    if (env != nullptr) {

        status = CPXcloseCPLEX(&env);

        if (status) {
            std::cout << "Fatal error while using CPLEX: failed to free environment. Aborting.\n";
            exit(1);
        }
    }


}
