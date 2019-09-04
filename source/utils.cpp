#include <utility>

//
// Created by acco on 11/24/18.
//

#include "utils.hpp"

#include <iostream>
#include <map>
#include <fstream>

void check_bks_availability(std::vector<std::string> instances, const std::string &bks_file_path) {

    std::cout << "UPPER BOUNDS AVAILABILITY CHECK\n";

    std::ifstream infile(bks_file_path);

    std::string key;
    float value;

    std::map<std::string, float> bks_map;
    while (infile >> key >> value) {
        bks_map[key] = value;
    }

    std::ofstream outfile;
    outfile.open(bks_file_path, std::ios_base::app);

    std::vector<std::string> new_instances;

    for(const auto &instance : instances) {
        if(!bks_map.count(instance)) {
            new_instances.push_back(instance);
        }
    }

    if(!new_instances.empty()) {
        std::cout << " Wait!\n";
        std::cout << " It seems that there are " << new_instances.size() << " instances that we never processed.\n";
        std::cout << " Please provide an upper bound for them!\n";
        std::cout << " Note: the upper bounds will only be used to compute the gaps.\n";
        for(const auto &instance : new_instances) {
            std::cout <<" " << instance << ": ";
            auto bks = 0.0f;
            std::cin >> bks;
            outfile << instance << "\t" << bks << "\n";
        }
        outfile.close();
        std::cout << " Update completed.\n";
    }else {
        std::cout << " Everything seems fine.\n";
    }

    std::cout << " Note, to view and edit upper bounds refer to '"<<bks_file_path<<"'.\n\n";


}

void print_progress(unsigned long current, unsigned long total, std::string message) {

    const static int barLen = 50;
    static unsigned long pastMessageLen = 0;

    auto progress = 0;

    if(current - total != 0){
        progress = static_cast<int>(barLen * current / static_cast<float>(total));
    } else {
        progress = barLen;
    }



    std::cout << "\r";
    for(auto n = 0u; n < barLen + 9 +2+ 1 + pastMessageLen; n++) {
        std::cout << " ";
    }

    std::cout << "\rProgress ";
    std::cout << "[";
    for(auto p = 0; p < progress; p++) {
        std::cout << ":";
    }
    for(auto p = progress; p < barLen; p++) {
        std::cout << " ";
    }

    std::cout << "] " << message;

    std::cout << std::flush;

    pastMessageLen = message.length();

}

void print_progress_end(std::string message) {
    print_progress(0,0, std::move(message));
}