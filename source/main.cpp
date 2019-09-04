#include <iostream>
#include "xsttrp_instance.hpp"
#include "avxs.hpp"
#include "utils.hpp"
#include "cmdline_arguments_parser.hpp"
#include "timer.hpp"


void warning_compiler_directives() {
    #if defined STATS || defined VERBOSE || defined NGRANULAR || defined VND
    std::cout << "ENABLED COMPILER DIRECTIVES\n";
    #ifdef STATS
    std::cout << " STATS - runtime statistics (may slightly affect performances)\n";
    #endif
    #ifdef STATS
    std::cout << " VERBOSE - detailed results (needs STATS)\n";
    #endif
    #ifdef STATS
    std::cout << " NGRANULAR - full neighborhood exploration\n";
    #endif
    #ifdef STATS
    std::cout << " VND - standard VND without randomization\n";
    #endif
    std::cout << "\n";
    #endif
}

int main(int argc, char* argv[]) {

    warning_compiler_directives();

    auto args_parser = cmdline_arguments_parser(argc, argv);

    auto parameters = args_parser.get_avxs_parameters();
    auto options = args_parser.get_program_options();

    std::vector<std::pair<xsttrp_instance, std::vector<avxs_result>>> results;

    check_bks_availability(options.paths, options.bks_path);

    auto errorsNum = 0;
    auto errors = std::vector<bool>(options.paths.size(), false);

    for(auto n = 0u; n < options.paths.size(); n++) {

        auto stopwatch = timer();
        stopwatch.start();

        const auto &path = options.paths[n];
        const auto& instance = xsttrp_instance(path, options.costs_decimals, options.round_arc_costs);

        if(!instance.is_valid) {
            errors[n] = true;
            errorsNum++;
            continue;
        }

        stopwatch.stop();

        const auto instance_parsing_time = stopwatch.elapsed();

        std::vector<avxs_result> results_list;

        auto old_seed = 0ul;
        auto seed = 0ul;

        for(auto run = 0ul; run < options.runs; run++) {

            if(options.fixed_seed) {
                seed = options.seed + run;
            } else {
                do {
                    std::random_device rd;
                    seed = rd();
                } while (seed == old_seed);
                old_seed = seed;
            }

            #ifdef STATS
            std::cout << path << " (run "<< run <<", seed "<<seed<<")\n";
            #else

            print_progress(options.runs * n + run, options.paths.size() * options.runs, path + "(run " + std::to_string((run+1)) + ")");

            #endif

            auto result = avxs(instance, seed, parameters);
            result.set_instance_parsing_time(instance_parsing_time);

            results_list.emplace_back(result);

        }

        results.emplace_back(instance, results_list);

    }

    #ifndef STATS
    print_progress_end("Done!");
    std::cout << "\n\n";
    #else
    std::cout << "\n";
    #endif


    if(errorsNum) {
        std::cout << "There were errors while parsing the following " << errorsNum << " file(s):\n";
        for(auto n = 0u; n < options.paths.size(); n++) {
            if(errors[n]) {
                std::cout << options.paths[n] << "\n";
            }
        }
    }

    analyze(results, parameters, options);

    std::cout << "\n\n\n\n";

}

