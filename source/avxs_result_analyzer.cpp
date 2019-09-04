//
// Created by acco on 11/24/18.
//

#include <map>
#include "avxs.hpp"
#include "program_options.hpp"
#include "utils.hpp"
#include "latex_drawer.hpp"
#include <iostream>
#include <iomanip>
#include <cfloat>

#define MIN_DECIMALS_FOR_ROUNDING (8)

float decimal_rounding(const float value, const unsigned int decimals) {

    if(decimals > MIN_DECIMALS_FOR_ROUNDING) {
        return value;
    }

    const auto power = std::pow(10, decimals);

    const auto rounded = static_cast<float>(std::round(value * power) / power);

    return rounded;
}



bool are_points_randomly_distributed(const xsttrp_instance& instance);

void print_main_table(std::ofstream &stream, std::map<std::string, float> &bks_map,
                      std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                      program_options &options);

void print_main_table_before_set_partitioning(std::ofstream &stream, std::map<std::string, float> &bks_map,
                      std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                      program_options &options);


void print_absolute_costs_table(std::ofstream &stream,
                                std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                program_options &options);

void print_absolute_costs_before_sp_table(std::ofstream &stream,
                                std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                program_options &options);

void print_set_partitioning_table(std::ofstream &stream, std::map<std::string, float> &bks_map,
                                  std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                  program_options &options);

void
print_seeds_table(std::ofstream &stream, std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list);

void
draw_images(std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list, program_options &options);

void print_instances_and_solutions(std::ofstream &stream,
                                   std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list);

#ifdef STATS

void print_rnei(std::ofstream &stream, std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list);

void print_shake_statistics(std::ofstream &stream,
                            std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list);

void print_avg_solutions_evolution(std::ofstream &stream, std::map<std::string, float> &bks_map,
                                   std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list);

#endif


void print_some_blank_lines(std::ofstream& stream);

void analyze(std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list, avxs_parameters& parameters, program_options& options) {

    std::cout << "Please wait, I'm processing the results " << std::flush;

    #ifdef STATS
    std::cout << "and it may take some minutes " << std::flush;
    #endif

    std::cout << "... " << std::flush;

    std::ifstream infile(options.bks_path, std::ios_base::app);

    std::string key;
    float value;

    std::map<std::string, float> bks_map;
    while (infile >> key >> value) {
        bks_map[key] = value;
    }

    auto filename = std::string();

    if(options.random_log_file) {

        static const int filename_len = 10;
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        std::random_device rd;
        auto rand_engine = std::mt19937(rd());


        for (int i = 0; i < filename_len; ++i) {
            filename.push_back(alphanum[rand_engine() % (sizeof(alphanum) - 1)]);
        }

    } else {

        filename = options.log_file;

    }

    retry:
    std::ofstream stream(filename);
    if (!stream.is_open()) {
        std::cout << "Oops\n";
        std::cout << "Unable to open file '" << filename
                  << "' in writing mode! Provide a path where to write the results: ";
        std::cin >> filename;
        std::cout << "Trying again ... " << std::flush;
        goto retry;
    }

    stream << std::setprecision(options.round_results) << std::fixed;

    stream << "This file summarizes the results obtained with AVXS.\n The level of details depends on the preprocessor "
              "directives used to compile the algorithm.\n The results are aggregated over the runs and over the instances.\n";



    print_some_blank_lines(stream);

    stream << "Assignment fitness function\n";
    if(parameters.use_near_fitness == parameters.use_rank_fitness) {
        stream << "- default values\n";
        stream << "-- " << static_cast<int>(LRP_NEAR_FACTOR) << "-near for problems with fixed costs\n";
        stream << "-- " << XSTTRP_NEAR_FACTOR << "-near otherwise\n";
    } else {

        stream << "- custom values\n";

        if(parameters.use_rank_fitness) {
            stream << "-- " << parameters.rank_factor << "-rank for all instances\n";
        } else {
            stream << "-- " << static_cast<int>(parameters.near_factor) << "-near for all instances\n";
        }
    }
    print_some_blank_lines(stream);

    print_main_table(stream, bks_map, info_list, options);
    print_some_blank_lines(stream);

    print_main_table_before_set_partitioning(stream, bks_map, info_list, options);
    print_some_blank_lines(stream);

    print_absolute_costs_table(stream, info_list, options);
    print_some_blank_lines(stream);

    print_set_partitioning_table(stream, bks_map, info_list, options);
    print_some_blank_lines(stream);

    print_absolute_costs_before_sp_table(stream, info_list, options);
    print_some_blank_lines(stream);

    print_seeds_table(stream, info_list);
    print_some_blank_lines(stream);

    draw_images(info_list, options);

    #ifdef VERBOSE
    print_instances_and_solutions(stream, info_list);
    print_some_blank_lines(stream);
    #endif

    #ifdef STATS

    print_rnei(stream, info_list);
    print_some_blank_lines(stream);

    print_shake_statistics(stream, info_list);
    print_some_blank_lines(stream);

    #ifdef VERBOSE
    print_avg_solutions_evolution(stream, bks_map, info_list);
    print_some_blank_lines(stream);
    #endif

    #endif



    stream.close();
    std::cout << "done\n";
    std::cout << "Results saved in '" << filename << "'\n";

}

void print_main_table(std::ofstream &stream, std::map<std::string, float> &bks_map,
                      std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                      program_options &options) {

    stream << "Objective value and timings (csv format)\n\n";
    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- the best known solution (bks)\n"
              "- the best gap over the runs (best)\n"
              "- the average gap over the runs (avg)\n"
              "- the worst gap over the runs (worst)\n"
              "- the total time to perform the runs in seconds (t)"
              "\n\n";

    stream << "instance, bks, best, avg, worst, t\n";

    auto average_best = 0.0f;
    auto average_average = 0.0f;
    auto average_worst = 0.0f;
    auto average_total_time = 0.0f;

    std::vector<std::pair<std::string, float>> new_best_solutions;

    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        auto bks = -1.0f;
        if (bks_map.count(instance.path)) {
            bks = bks_map[instance.path];
        }

        auto best_cost = 0.0f;

        auto best = FLT_MAX;
        auto worst = -FLT_MAX;
        auto average = 0.0f;
        auto total_time = 0ul;

        for (const auto &result : results) {

            auto cost = decimal_rounding(result.get_cost_after_set_partitioning(), options.round_results);

            const auto gap = 100.0f * (cost - bks) / bks;

            if (gap < best) {
                best = gap;
                best_cost = cost;
            }

            if (gap > worst) {
                worst = gap;
            }

            average += gap;

            total_time += result.get_total_time_seconds();
        }

        average /= results.size();

        average_best += best;
        average_average += average;
        average_worst += worst;
        average_total_time += total_time;

        stream << instance.path << ", " << bks << ", " << best << ", " << average << ", " << worst << ", " << total_time << "\n";

        if(best_cost + TOLERANCE < bks) {
            new_best_solutions.emplace_back(instance.path, best_cost);
        }
    }

    average_best /= info_list.size();
    average_average /= info_list.size();
    average_worst /= info_list.size();
    average_total_time /= static_cast<float>(info_list.size());

    stream << "\n\n== Summary ==\n";
    stream << "Average best gap = " << average_best << "\n";
    stream << "Average avg gap = " << average_average << "\n";
    stream << "Average worst gap = " << average_worst << "\n";
    stream << "Average total time (10 runs) = " << average_total_time << "\n";


    if(!new_best_solutions.empty()) {
        stream << "== New best solutions ==\n";
        for(auto p : new_best_solutions) {
            stream << p.first << " with cost " << p.second << "\n";
        }
    }

}

void print_main_table_before_set_partitioning(std::ofstream &stream, std::map<std::string, float> &bks_map,
                      std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                      program_options &options) {

    stream << "Objective value and timings BEFORE set partitiong (csv format)\n\n";
    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- the best known solution (bks)\n"
              "- the best gap over the runs (best)\n"
              "- the average gap over the runs (avg)\n"
              "- the worst gap over the runs (worst)\n"
              "- the total time to perform the runs in seconds (t)"
              "\n\n";

    stream << "instance, bks, best, avg, worst, t\n";

    auto average_best = 0.0f;
    auto average_average = 0.0f;
    auto average_worst = 0.0f;
    auto average_total_time = 0.0f;

    std::vector<std::pair<std::string, float>> new_best_solutions;

    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        auto bks = -1.0f;
        if (bks_map.count(instance.path)) {
            bks = bks_map[instance.path];
        }

        auto best_cost = 0.0f;

        auto best = FLT_MAX;
        auto worst = -FLT_MAX;
        auto average = 0.0f;
        auto total_time = 0ul;

        for (const auto &result : results) {

            auto cost = decimal_rounding(result.get_cost_before_set_partitioning(), options.round_results);

            const auto gap = 100.0f * (cost - bks) / bks;

            if (gap < best) {
                best = gap;
                best_cost = cost;
            }

            if (gap > worst) {
                worst = gap;
            }

            average += gap;

            total_time += (result.get_total_time_seconds() - result.get_set_partitioning_time_seconds());
        }

        average /= results.size();

        average_best += best;
        average_average += average;
        average_worst += worst;
        average_total_time += total_time;

        stream << instance.path << ", " << bks << ", " << best << ", " << average << ", " << worst << ", " << total_time << "\n";

        if(best_cost + TOLERANCE < bks) {
            new_best_solutions.emplace_back(instance.path, best_cost);
        }
    }

    average_best /= info_list.size();
    average_average /= info_list.size();
    average_worst /= info_list.size();
    average_total_time /= static_cast<float>(info_list.size());

    stream << "\n\n== Summary ==\n";
    stream << "Average best gap = " << average_best << "\n";
    stream << "Average avg gap = " << average_average << "\n";
    stream << "Average worst gap = " << average_worst << "\n";
    stream << "Average total time (10 runs) = " << average_total_time << "\n";

}

void print_set_partitioning_table(std::ofstream &stream, std::map<std::string, float> &bks_map,
                                  std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                  program_options &options) {

    auto average_best = 0.0f;
    auto average_average = 0.0f;
    auto average_set_partitioning_percentage_time = 0.0f;
    auto average_average_set_partitioning_pool_size = 0.0f;

    stream << "Set-partitioning post-optimization";

    #ifdef VERBOSE
    stream << "(csv format)";
    #endif

    stream << "\n\n";

    #ifdef VERBOSE
    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- the best improvement over the runs (best)\n"
              "- the average improvement over the runs (avg)\n"
              "- the average percentage time of the polishing phase with respect to the total algorithm time (\\% t)\n"
              "- the average pool size over the runs (size)"
              "\n\n";

    stream << "instance, best, avg, \\%, size\n";
    #endif

    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        auto bks = -1.0f;
        if (bks_map.count(instance.path)) {
            bks = bks_map[instance.path];
        }

        auto best_improvement = -FLT_MAX;
        auto average_improvement = 0.0f;
        auto total_time = 0ul;
        auto algo_time = 0ul;
        auto pool_size = 0ul;

        for (const auto &result : results) {

            auto cost_before_set_partitioning = decimal_rounding(result.get_cost_before_set_partitioning(), options.round_results);
            auto cost_after_set_partitioning = decimal_rounding(result.get_cost_after_set_partitioning(), options.round_results);

            const auto gap_before = 100.0f * (cost_before_set_partitioning - bks) / bks;
            const auto gap_after = 100.0f * (cost_after_set_partitioning - bks) / bks;
            const auto gap_improvement = gap_before - gap_after;

            if (gap_improvement > best_improvement) {
                best_improvement = gap_improvement;
            }

            average_improvement += gap_improvement;

            total_time += result.get_set_partitioning_time_seconds();
            algo_time += result.get_total_time_seconds();

            pool_size += result.get_set_partitioning_pool_size();
        }

        average_improvement /= static_cast<float>(results.size());
        const auto average_pool_size = static_cast<float>(pool_size) / static_cast<float>(results.size());
        average_average_set_partitioning_pool_size += average_pool_size;

        average_best += best_improvement;
        average_average += average_improvement;

        const auto set_partitioning_percentage_time = 100.0f * (static_cast<float>(total_time) / static_cast<float>(algo_time));
        average_set_partitioning_percentage_time += set_partitioning_percentage_time;


        #ifdef VERBOSE
        stream << instance.path << ", " << best_improvement << ", " << average_improvement << ", " << set_partitioning_percentage_time << ", "<< average_pool_size << "\n";
        #endif

    }

    average_best /= info_list.size();
    average_average /= info_list.size();
    average_set_partitioning_percentage_time /= info_list.size();
    average_average_set_partitioning_pool_size /= info_list.size();
    stream << "== Summary ==\n";
    stream << "Average improvement = " << average_average << "\n";
    stream << "(Average best improvement = " << average_best << ")\n";
    stream << "Average percentage time = " << average_set_partitioning_percentage_time << "\n";
    stream << "Average pool size = " << average_average_set_partitioning_pool_size << "\n";

}

void
print_seeds_table(std::ofstream &stream, std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list) {

    stream << "Seeds (csv format)\n\n";
    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- a column for each runs which identifies the seed used in that run (run N)"
              "\n\n";

    stream << "instance";
    for(auto run =0ul; run < info_list[0].second.size(); run++) {
        stream << ", run " << run;
    }
    stream << "\n";
    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        stream << instance.path;

        int run = 0;

        for (const auto &result : results) {
            stream << ", " << result.get_seed();
            run++;
        }
        stream << "\n";
    }

}

void print_some_blank_lines(std::ofstream& stream) {
    stream << "\n\n\n\n\n\n\n";
    stream << std::flush;
}

void print_absolute_costs_table(std::ofstream &stream,
                                std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                program_options &options) {

    stream << "Absolute costs (csv format)\n\n";

    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- for each run the value of the objective function for the best found solution (run N)\n"
              "- the best objective function value (best)"
              "\n\n";

    stream << "instance";
    for(auto run =0ul; run < info_list[0].second.size(); run++) {
        stream << ", run " << run;
    }
    stream << ", best\n";
    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        stream << instance.path;

        int run = 0;

        auto min = FLT_MAX;

        for (const auto &result : results) {

            auto cost = decimal_rounding(result.get_final_solution()->get_cost(), options.round_results);

            stream << ", " << cost;
            run++;

            if(cost < min) {
                min = cost;
            }

        }
        stream << ", " << min << "\n";
    }

}

void print_absolute_costs_before_sp_table(std::ofstream &stream,
                                std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list,
                                program_options &options) {

    stream << "Absolute costs before set partitioning execution (csv format)\n\n";

    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- for each run the value of the objective function for the best found solution (run N)\n"
              "- the best objective function value (best)"
              "\n\n";

    stream << "instance";
    for(auto run =0ul; run < info_list[0].second.size(); run++) {
        stream << ", run " << run;
    }
    stream << ", best\n";
    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        stream << instance.path;

        int run = 0;

        auto min = FLT_MAX;

        for (const auto &result : results) {

            auto cost = decimal_rounding(result.get_cost_before_set_partitioning(), options.round_results);

            stream << ", " << cost;
            run++;

            if(cost < min) {
                min = cost;
            }

        }
        stream << ", " << min << "\n";
    }

}

void
draw_images(std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list, program_options &options) {

    if(options.tex_picture) {
        for (const auto &info : info_list) {

            auto instance = info.first;
            auto results = info.second;

            for (auto run = 0u; run < results.size(); run++) {
                auto& result = results[run];
                latex_drawer::draw(instance, *result.get_final_solution(), run);
            }
        }
    }

}

bool hopkins_statistic(const xsttrp_instance& instance) {

    std::random_device rd;
    std::mt19937 rand_engine(rd());

    const auto m = static_cast<unsigned long>(instance.matrix_size/10.0f);
    auto random_points = std::vector<std::pair<float, float>>(m);
    auto vertices = std::vector<int>(instance.matrix_size);

    auto u = std::vector<float>(m);
    auto uSum = 0.0f;
    auto w = std::vector<float>(m);
    auto wSum = 0.0f;

    auto xmin = FLT_MAX;
    auto xmax = -xmin;
    auto ymin = FLT_MAX;
    auto ymax = -ymin;

    for(auto n = 0; n< instance.matrix_size;n++) {
        if(instance.x[n] < xmin) { xmin = instance.x[n];}
        if(instance.x[n] > xmax) { xmax = instance.x[n];}
        if(instance.y[n] < ymin) { ymin = instance.y[n];}
        if(instance.y[n] > ymax) { ymax = instance.y[n];}
        vertices[n] = n;
    }

    std::uniform_int_distribution<int> xuni(static_cast<int>(xmin), static_cast<int>(xmax));
    std::uniform_int_distribution<int> yuni(static_cast<int>(ymin), static_cast<int>(ymax));

    // generate m uniformly distributed pointes
    for(auto i = 0u; i < m; i++) {
        random_points[i].first = xuni(rand_engine);
        random_points[i].second = yuni(rand_engine);
    }

    // for each generated point compute the distance to the nearest neighbor
    for(auto i = 0u; i < m; i++) {
        u[i] = FLT_MAX;
        for(auto n = 0; n < instance.matrix_size; n++) {
            const auto distance = std::sqrt((random_points[i].first - instance.x[n]) * (random_points[i].first - instance.x[n])
                                            + (random_points[i].second - instance.y[n])*(random_points[i].second - instance.y[n]));
            if(distance < u[i]) {
                u[i] = distance;
            }
        }
        uSum += u[i];
    }

    for (size_t i = 0; i < vertices.size(); i++) {
        const auto r = static_cast<const int>(rand_engine() % vertices.size());
        const auto temp = vertices[i];
        vertices[i] = vertices[r];
        vertices[r] = temp;
    }

    for(auto i = 0u; i < m; i++) {
        w[i] = FLT_MAX;
        const auto index = vertices[i];
        for(auto n = 0; n < instance.matrix_size; n++) {
            if(n == index) {
                continue;
            }
            const auto distance = std::sqrt((instance.x[index] - instance.x[n]) * (instance.x[index] - instance.x[n])
                                            + (instance.y[index] - instance.y[n])*(instance.y[index] - instance.y[n]));
            if(distance < w[i]) {
                w[i] = distance;
            }
        }
        wSum += w[i];
    }

    const auto H = uSum / (uSum + wSum);


    return H <= 0.75;

}

bool are_points_randomly_distributed(const xsttrp_instance& instance) {

    int attempts = 100;
    int counter = 0;

    for(auto n = 0; n < attempts; n++) {
        if(hopkins_statistic(instance)) {
            counter++;
        }
    }

    return counter > attempts / 2.0;
}

void print_instances_and_solutions(std::ofstream &stream,
                                   std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list) {

    stream << "Solutions (xsttrp format)\n\n";

    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        stream << instance.path << "\n";
        stream << "According to the Hopkins statistic, it should be a " << (are_points_randomly_distributed(instance) ? "randomly distributed" : "clustered") << " instance\n";
        stream << "Truck capacity = " << instance.truck_capacity << "\n";
        stream << "Trailer capacity = " << instance.trailer_capacity << "\n";
        if(instance.fixed_satellite_opening_cost > 0) {
            stream << "Fixed cost on opening a satellite = " << instance.fixed_satellite_opening_cost << "\n";
        }
        if(instance.fixed_sub_route_opening_cost > 0) {
            stream << "Fixed cost on performing sub-routes = " << instance.fixed_sub_route_opening_cost << "\n";
        }
        stream << "Nodes index definition: \n";
        stream << "Depot:\n\t" << instance.depot << "\t(" << instance.x[instance.depot] << ", " << instance.y[instance.depot] << ")\n";
        if(instance.satellites_num) {
            stream << "Satellites: \n";
            for(auto i = instance.satellites_begin; i < instance.satellites_end; i++) {
                stream << "\t" << i << "\t(" << instance.x[i] << ", " << instance.y[i] << ")\n";
            }
        }
        if(instance.truck_customers_num) {
            stream << "Truck customers ("<< instance.truck_customers_num<<"): \n";
            for(auto i = instance.truck_customers_begin; i < instance.truck_customers_end; i++) {
                stream << "\t" << i << "\t(" << instance.x[i] << ", " << instance.y[i] << ") \t q = " << instance.q[i] << "\n";
            }
        }
        if(instance.vehicle_customers_no_park_num) {
            stream << "Vehicle customers without parking facility ("<< instance.vehicle_customers_no_park_num<<"): \n";
            for(auto i = instance.vehicle_customers_no_park_begin; i < instance.vehicle_customers_no_park_end; i++) {
                stream << "\t" << i << "\t(" << instance.x[i] << ", " << instance.y[i] << ") \t q = " << instance.q[i] << "\n";
            }
        }
        if(instance.vehicle_customers_yes_park_num) {
            stream << "Truck customers with parking facility ("<< instance.vehicle_customers_yes_park_num<<"): \n";
            for(auto i = instance.vehicle_customers_yes_park_begin; i < instance.vehicle_customers_yes_park_begin; i++) {
                stream << "\t" << i << "\t(" << instance.x[i] << ", " << instance.y[i] << ") \t q = " << instance.q[i] << "\n";
            }
        }

        stream << "\n";

        for(auto n = 0ul; n < results.size(); n++){

            auto solution = results[n].get_final_solution();
            stream << "Run " << n << "\n\n";
            solution->print(stream);
            stream << "\n";
        }

    }


}

#ifdef STATS

void print_rnei(std::ofstream &stream, std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list) {



    #ifdef VERBOSE

    stream << "Relative neighborhood effectiveness index (csv format)\n";

    stream << "For each instance, the table contains the following columns\n "
              "- instance identifier (instance)\n"
              "- the RELOCATE percentage improvement (RELOCATE \\% impr) \n"
              "- the RELOCATE percentage successful applications (RELOCATE \\% appl) \n"
              "- the SWAP percentage improvement (SWAP \\% impr)\n"
              "- the SWAP percentage successful applications (SWAP \\% appl)\n"
              "- the TWOPT percentage improvement (TWOPT \\% impr)\n"
              "- the TWOPT percentage successful applications (TWOPT \\% appl)\n"
              "- the ROOTREF percentage improvement (ROOTREF \\% impr)\n"
              "- the ROOTREF percentage successful applications (ROOTREF \\% appl)\n"
              "- the SEGSWAP percentage improvement (SEGSWAP \\% impr)\n"
              "- the SEGSWAP percentage successful applications (SEGSWAP \\% appl)\n"
              "\n\n";

    stream << "instance, "
              "RELOCATE \\% impr, "
              "RELOCATE \\% appl, "
              "SWAP \\% impr, "
              "SWAP \\% appl, "
              "TWOPT \\% impr, "
              "TWOPT \\% appl, "
              "ROOTREF \\% impr, "
              "ROOTREF \\% appl, "
              "SEGSWAP \\% impr, "
              "SEGSWAP \\% appl, "
              "\n";

    #endif


    auto total_percentage_relocate_improvement = 0.0f;
    auto total_percentage_swap_improvement = 0.0f;
    auto total_percentage_twopt_improvement = 0.0f;
    auto total_percentage_rootref_improvement = 0.0f;
    auto total_percentage_segswap_improvement = 0.0f;

    auto total_percentage_relocate_applications = 0.0f;
    auto total_percentage_swap_applications = 0.0f;
    auto total_percentage_twopt_applications = 0.0f;
    auto total_percentage_rootref_applications = 0.0f;
    auto total_percentage_segswap_applications = 0.0f;


    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        auto instance_relocate_improvement = 0.0f;
        auto instance_swap_improvement = 0.0f;
        auto instance_twopt_improvement = 0.0f;
        auto instance_rootref_improvement = 0.0f;
        auto instance_segswap_improvement = 0.0f;

        auto instance_relocate_applications = 0.0f;
        auto instance_swap_applications = 0.0f;
        auto instance_twopt_applications = 0.0f;
        auto instance_rootref_applications = 0.0f;
        auto instance_segswap_applications = 0.0f;

        for (const auto &result : results) {

            instance_relocate_improvement += result.get_relocate_improvement();
            instance_swap_improvement += result.get_swap_improvement();
            instance_twopt_improvement += result.get_twopt_improvement();
            instance_rootref_improvement += result.get_rootref_improvement();
            instance_segswap_improvement += result.get_segswap_improvement();

            instance_relocate_applications += result.get_relocate_successful_applications();
            instance_swap_applications += result.get_swap_successful_applications();
            instance_twopt_applications += result.get_twopt_successful_applications();
            instance_rootref_applications += result.get_rootref_successful_applications();
            instance_segswap_applications += result.get_segswap_successful_applications();

        }

        instance_relocate_improvement /= results.size();
        instance_swap_improvement /= results.size();
        instance_twopt_improvement /=results.size();
        instance_rootref_improvement /=results.size();
        instance_segswap_improvement /=results.size();

        instance_relocate_applications /=results.size();
        instance_swap_applications /=results.size();
        instance_twopt_applications /=results.size();
        instance_rootref_applications /=results.size();
        instance_segswap_applications /=results.size();

        const auto instance_total_improvement = instance_relocate_improvement + instance_swap_improvement +
                instance_twopt_improvement + instance_segswap_improvement + instance_rootref_improvement;

        const auto instance_percentage_relocate_improvement = 100.0 * instance_relocate_improvement / instance_total_improvement;
        const auto instance_percentage_swap_improvement = 100.0 * instance_swap_improvement / instance_total_improvement;
        const auto instance_percentage_twopt_improvement = 100.0 * instance_twopt_improvement / instance_total_improvement;
        const auto instance_percentage_rootref_improvement = 100.0 * instance_rootref_improvement / instance_total_improvement;
        const auto instance_percentage_segswap_improvement = 100.0 * instance_segswap_improvement / instance_total_improvement;


        const auto instance_total_applications = instance_relocate_applications + instance_swap_applications +
                                                instance_twopt_applications + instance_segswap_applications +
                                                instance_rootref_applications;

        const auto instance_percentage_relocate_applications = 100.0 * instance_relocate_applications / instance_total_applications;
        const auto instance_percentage_swap_applications = 100.0 * instance_swap_applications / instance_total_applications;
        const auto instance_percentage_twopt_applications = 100.0 * instance_twopt_applications / instance_total_applications;
        const auto instance_percentage_rootref_applications = 100.0 * instance_rootref_applications / instance_total_applications;
        const auto instance_percentage_segswap_applications = 100.0 * instance_segswap_applications / instance_total_applications;

        total_percentage_relocate_improvement += instance_percentage_relocate_improvement;
        total_percentage_swap_improvement += instance_percentage_swap_improvement;
        total_percentage_twopt_improvement += instance_percentage_twopt_improvement;
        total_percentage_rootref_improvement += instance_percentage_rootref_improvement;
        total_percentage_segswap_improvement += instance_percentage_segswap_improvement;

        total_percentage_relocate_applications += instance_percentage_relocate_applications;
        total_percentage_swap_applications += instance_percentage_swap_applications;
        total_percentage_twopt_applications += instance_percentage_twopt_applications;
        total_percentage_rootref_applications += instance_percentage_rootref_applications;
        total_percentage_segswap_applications += instance_percentage_segswap_applications;

        #ifdef VERBOSE
        stream << instance.path << ", "
               << instance_percentage_relocate_improvement << ", " << instance_percentage_relocate_applications << ", "
               << instance_percentage_swap_improvement << ", " << instance_percentage_swap_applications << ", "
               << instance_percentage_twopt_improvement << ", " << instance_percentage_twopt_applications << ", "
               << instance_percentage_rootref_improvement << ", " << instance_percentage_rootref_applications << ", "
               << instance_percentage_segswap_improvement << ", " << instance_percentage_segswap_applications << "\n";
        #endif

    }

    total_percentage_relocate_improvement /= info_list.size();
    total_percentage_swap_improvement /= info_list.size();
    total_percentage_twopt_improvement /= info_list.size();
    total_percentage_rootref_improvement /= info_list.size();
    total_percentage_segswap_improvement /= info_list.size();

    total_percentage_relocate_applications /= info_list.size();
    total_percentage_swap_applications /= info_list.size();
    total_percentage_twopt_applications /= info_list.size();
    total_percentage_rootref_applications /= info_list.size();
    total_percentage_segswap_applications /= info_list.size();

    const auto absolute_rnei_relocate = total_percentage_relocate_applications != 0 ? (total_percentage_relocate_improvement / total_percentage_relocate_applications) : 0;
    const auto absolute_rnei_swap =  total_percentage_swap_applications != 0 ? (total_percentage_swap_improvement / total_percentage_swap_applications) : 0;
    const auto absolute_rnei_twopt = total_percentage_twopt_applications != 0 ? (total_percentage_twopt_improvement / total_percentage_twopt_applications) : 0;
    const auto absolute_rnei_rootref =  total_percentage_rootref_applications != 0 ? (total_percentage_rootref_improvement / total_percentage_rootref_applications) : 0;
    const auto absolute_rnei_segswap = total_percentage_segswap_applications != 0 ?(total_percentage_segswap_improvement / total_percentage_segswap_applications) : 0;
    const auto absolute_rnei_sum = absolute_rnei_relocate + absolute_rnei_swap + absolute_rnei_twopt + absolute_rnei_rootref + absolute_rnei_segswap;

    stream << "Aggregate RNEI results\n";
    stream << "RELOCATE\n";
    stream << "\tPercentage improvement\t" << total_percentage_relocate_improvement << "\n";
    stream << "\tPercentage applications\t" << total_percentage_relocate_applications  << "\n";
    stream << "\tPercentage RNEI\t" << 100.0f * absolute_rnei_relocate / absolute_rnei_sum << "\n";

    stream << "SWAP\n";
    stream << "\tPercentage improvement\t" << total_percentage_swap_improvement << "\n";
    stream << "\tPercentage applications\t" << total_percentage_swap_applications  << "\n";
    stream << "\tPercentage RNEI\t" << 100.0f * absolute_rnei_swap / absolute_rnei_sum << "\n";

    stream << "TWOPT\n";
    stream << "\tPercentage improvement\t" << total_percentage_twopt_improvement << "\n";
    stream << "\tPercentage applications\t" << total_percentage_twopt_applications  << "\n";
    stream << "\tPercentage RNEI\t" << 100.0f * absolute_rnei_twopt / absolute_rnei_sum << "\n";

    stream << "ROOTREF\n";
    stream << "\tPercentage improvement\t" << total_percentage_rootref_improvement << "\n";
    stream << "\tPercentage applications\t" << total_percentage_rootref_applications  << "\n";
    stream << "\tPercentage RNEI\t" << 100.0f * absolute_rnei_rootref / absolute_rnei_sum << "\n";

    stream << "SEGSWAP\n";
    stream << "\tPercentage improvement\t" << total_percentage_segswap_improvement << "\n";
    stream << "\tPercentage applications\t" << total_percentage_segswap_applications  << "\n";
    stream << "\tPercentage RNEI\t" << 100.0f * absolute_rnei_segswap / absolute_rnei_sum << "\n";

    stream << "\n";


}

void print_shake_statistics(std::ofstream &stream,
                            std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list) {

    #ifdef VERBOSE
    stream << "Shake percentage improvement (csv format)\n";


    stream << "For each instance, the table contains the following columns\n"
              "- instance identifier (instance)\n"
              "- the SUNLOAD induced percentage improvement (SUNLOAD \\% impr) \n"
              "- the SREM induced percentage improvement (SREM \\% appl) \n"
              "\n\n";

    stream << "instance, "
              "SUNLOAD \\% impr, "
              "SREM \\% appl"
              "\n";
    #endif


    auto total_percentage_sunload_improvement = 0.0f;
    auto total_percentage_srem_improvement = 0.0f;

    for (const auto &info : info_list) {

        auto instance = info.first;
        auto results = info.second;

        auto instance_sunload_improvement = 0.0f;
        auto instance_srem_improvement = 0.0f;

        for (const auto &result : results) {

            instance_sunload_improvement += result.get_sunload_improvement();
            instance_srem_improvement += result.get_srem_improvement();

        }

        instance_sunload_improvement /= results.size();
        instance_srem_improvement /= results.size();
        auto instance_total = instance_sunload_improvement + instance_srem_improvement;

        if(instance_total == 0) {
            instance_total = 1;
        }

        const auto instance_percentage_sunload_improvement = 100.0 * (instance_sunload_improvement / instance_total);
        const auto instance_percentage_srem_improvement = 100.0 * (instance_srem_improvement / instance_total);

        total_percentage_sunload_improvement += instance_percentage_sunload_improvement;
        total_percentage_srem_improvement += instance_percentage_srem_improvement;

        #ifdef VERBOSE
        stream << instance.path << ", "
               << instance_percentage_sunload_improvement << ", " << instance_percentage_srem_improvement << "\n";
        #endif

    }

    total_percentage_sunload_improvement /= info_list.size();
    total_percentage_srem_improvement /= info_list.size();

    stream << "== Summary ==\n";
    stream << "SUNLOAD percentage improvement = " << total_percentage_sunload_improvement << "\n";
    stream << "SREM percentage improvement = " << total_percentage_srem_improvement << "\n";



}

void print_avg_solutions_evolution(std::ofstream &stream, std::map<std::string, float> &bks_map,
                                   std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> &info_list) {

    stream << "Average solutions evolution during time and iterations\n\n";

    const auto points_num = RESTARTS_NUM * ILS_ITERATIONS;

    std::vector<unsigned long> points_iter(points_num, INT_MAX);
    std::vector<float> points_time(points_num, -FLT_MAX);
    std::vector<float> points_value(points_num, -FLT_MAX);

    auto average_solution_quality = 0.0f;

    for (auto n = 0ul; n < points_num; n++) {

        auto time_sum = 0.0f;
        auto obj_sum = 0.0f;
        auto ignored_instance = 0ul;

        for (const auto &info : info_list) {
            auto bks = bks_map[info.first.path];
            auto time_partial = 0.0f;
            auto obj_partial = 0.0f;
            auto ignored_run = 0ul;
            const auto results = info.second;
            for (const auto &result : results) {
                auto point = result.get_sample(n);
                if(point.objective_value > MDVRP_ROUTE_EXCESS_PENALTY) {
                    ignored_run++;
                } else {
                    time_partial += point.time;
                    obj_partial += 100.0 * (point.objective_value - bks) / bks;
                }
            }
            if(ignored_run == results.size()) {
                ignored_instance++;
            } else {
                time_sum += (time_partial / (results.size() - ignored_run));
                obj_sum += (obj_partial / (results.size() - ignored_run));
            }

        }

        if(info_list.size() != ignored_instance) {
            time_sum /= (info_list.size() - ignored_instance);
            obj_sum /= (info_list.size() - ignored_instance);
        } else {
            stream << "** warning:  skipped infeasible point **\n";
        }

        points_iter[n] = n;
        points_time[n] = time_sum;
        points_value[n] = obj_sum;
        average_solution_quality += obj_sum;

    }

    average_solution_quality /= points_value.size();

    stream << "Average solution quality during the first " << points_num << " iterations" <<" = " << average_solution_quality << "\n\n";

    stream << "The following set of (x, y) points represent"
              "- x: iteration"
              "- y: percentage gap of the current solution\n\n";
    for(auto n = 0ul; n < points_num; n++) {
        stream << "(" << points_iter[n] << ", " << points_value[n] << ") ";
    }


    stream << "The following set of (x, y) points represent"
              "- x: iteration"
              "- y: percentage gap of the global best solution\n\n";
    auto min = FLT_MAX;
    for(auto n = 0ul; n < points_num; n++) {
        if(points_value[n] < min) {
            stream << "(" << points_iter[n] << ", " << points_value[n] << ") ";
            min = points_value[n];
        }
    }

    stream << "\n\n";
    stream << "The following set of (x, y) points represent"
              "- x: time instant"
              "- y: percentage gap of the current solution\n\n";
    for(auto n = 0ul; n < points_num; n++) {
        stream << "(" << points_iter[n] << ", " << points_value[n] << ") ";
    }


    stream << "\n\n";
    stream << "The following set of (x, y) points represent"
              "- x: time instant"
              "- y: percentage gap of the global best solution\n\n";
    min = FLT_MAX;
    for(auto n = 0ul; n < points_num; n++) {
        if(points_value[n] < min) {
            stream << "(" << points_iter[n] << ", " << points_value[n] << ") ";
            min = points_value[n];
        }
    }


}

#endif
