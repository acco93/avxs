//
// Created by acco on 11/1/18.
//
// A very simple command line arguments parser.
//

#include "cmdline_arguments_parser.hpp"
#include <iostream>


#define TOKEN_RUNS ("--runs")
#define TOKEN_SEED ("--seed")
#define TOKEN_FILES ("--files")
#define TOKEN_HELP ("--help")
#define TOKEN_BKS ("--bks")
#define TOKEN_NEAR ("--near")
#define TOKEN_RANK ("--rank")
#define TOKEN_LOG ("--log")
#define TOKEN_PRECISION ("--precision")
#define TOKEN_ROUND_RESULTS ("--round-results")
#define TOKEN_TEX_PICTURE ("--tex-picture")
#define TOKEN_ROUND_ARC_COSTS ("--round-arc-costs")


cmdline_arguments_parser::cmdline_arguments_parser(int argc, char **argv) {

    if(argc == 1 || (argc == 2 && std::string(argv[1]) == TOKEN_HELP)) {
        print_usage();
        exit(0);
    }

    for(auto n = 1; n < argc; n++) {

        auto token = std::string(argv[n]);

        if(token == TOKEN_SEED) {

            options.fixed_seed = true;
            options.seed = std::stoul(std::string(argv[++n]));

        } else if(token == TOKEN_FILES) {

            for(n = n + 1; n < argc; n++) {
                token = std::string(argv[n]);

                if(token.substr(0, 2) == "--") {
                    n = n - 1;
                    break;
                }

                options.paths.push_back(token);
            }

        } else if(token == TOKEN_RUNS) {
            options.runs = std::stoul(std::string(argv[++n]));
        } else if(token == TOKEN_BKS) {
            options.bks_path = std::string(argv[++n]);
        } else if(token == TOKEN_NEAR) {
            parameters.near_factor = std::stol(std::string(argv[++n]));
            parameters.use_rank_fitness = false;
            parameters.use_near_fitness = true;
        } else if(token == TOKEN_RANK) {
            parameters.rank_factor = std::stol(std::string(argv[++n]));
            parameters.use_rank_fitness = true;
            parameters.use_near_fitness = false;
        }else if(token == TOKEN_LOG){
            options.random_log_file = false;
            options.log_file = std::string(argv[++n]);
        } else if(token == TOKEN_PRECISION) {
            options.costs_decimals = std::stoul(std::string(argv[++n]));
        } else if(token == TOKEN_ROUND_RESULTS) {
            options.round_results = std::stoul(std::string(argv[++n]));
        } else if(token == TOKEN_TEX_PICTURE) {
            options.tex_picture = true;
        } else if(token == TOKEN_ROUND_ARC_COSTS) {
            options.round_arc_costs = true;
        } else {
            std::cout << "Error: unknown argument '" << token <<"'. Try --help for support.\n";
            exit(1);
        }

    }

    if(options.paths.empty()) {
        print_usage();
        exit(1);
    }

    print_avxs_parameters();
    print_program_options();

}

void cmdline_arguments_parser::print_usage() {
    std::cout << "Usage: avxs [OPTIONS] --files path1 [path2 ... pathN]\n";
    std::cout << "Available options are:\n";
    std::cout << " --seed NUMBER\t\tSet the seed, default: number of milliseconds from 1970\n";
    std::cout << " --runs NUMBER\t\tSet the number of repetitions for each instance, default: 1\n";
    std::cout << " --bks STRING\t\tSet the bks file, default: ../bks.txt\n";
    std::cout << " --near NUMBER\t\tUse the d-near assignment fitness function\n";
    std::cout << " --rank NUMBER\t\tUse the b-rank assignment fitness function\n";
    std::cout << " --log STRING\t\tStore results in the given path\n";
    std::cout << " --precision NUMBER\tUse a defined number of decimal places when computing arc costs.\n";
    std::cout << "                   \tWarning: using too few decimal places may cause undefined behaviour.\n";
    std::cout << " --round-arc-costs\tRound arc costs to the nearest integer value.\n";
    std::cout << " --round-results NUMBER\tRound the final results to the defined number of decimals\n";
    std::cout << " --latex-tables\t\tPrint results in a latex-compatible way.\n";
    std::cout << " --tex-picture\t\tProduce a .tex picture that could be compiled in .pdf using\n";
    std::cout << "              \t\tpdflatex --enable-write18 -interaction=nonstopmode file.tex\n";
    std::cout << "              \t\tThe .tex files will be created in the same directory as the considered instance files\n";
    std::cout << "\n";
    std::cout << "Reports bug to <luca.accorsi4 and the domain is unibo.it>\n";
}

void cmdline_arguments_parser::print_avxs_parameters() {
    std::cout << "AVXS ENCODED HYPERPARAMETERS\n";
    std::cout << " Number of restarts \u0394 = " << RESTARTS_NUM << "\n";
    std::cout << " Maximum number of non-improving iterations \u03B4  = " << ILS_ITERATIONS << "\n";
    std::cout << " Sparsification parameter \u03C0 base = " << PI_BASE << "\n";
    std::cout << " Probability of removing a customer from the main-route in MREM \u03B7 = " << ETA_MREM << "\n";
    std::cout << " Unloading range of SUNLOAD \u03B6 = [" << ZETA_SUNLOAD_MIN  / 100.0f << ", " << ZETA_SUNLOAD_MIN / 100.0f + ZETA_SUNLOAD_MAX / 100.0f<< "]" << "\n";
    std::cout << " Maximum number of CPLEX ticks = " << CPLEX_TICKS_PER_SECOND * CPLEX_TIME_LIMIT << "\n";
    std::cout << "\n";
}

program_options cmdline_arguments_parser::get_program_options() {
    return options;
}

avxs_parameters cmdline_arguments_parser::get_avxs_parameters() {
    return parameters;
}

void cmdline_arguments_parser::print_program_options() {
    std::cout << "PROGRAM OPTIONS\n";
    std::cout << " Instances to process = " << options.paths.size() << "\n";
    std::cout << " Number of runs per instance = " << options.runs << "\n";
    if(options.fixed_seed) {
        std::cout << " Seed = " << options.seed << "\n";
    } else {
        std::cout << " Seed = auto\n";
    }
    if(options.random_log_file) {
        std::cout << " Log file = random name\n";
    } else {
        std::cout << " Log file = " << options.log_file << "\n";
    }
    if(options.costs_decimals < 32) {
        std::cout << " Arc cost decimal places = " << options.costs_decimals << "\n";
    }
    if(options.round_results < 8) {
        std::cout << " Results decimal places = " << options.round_results << "\n";
    }
    if(options.round_arc_costs) {
        std::cout << " Round arc costs = true\n";
    }
    std::cout << " BKS path = " << options.bks_path << "\n";
    std::cout << "\n";
}
