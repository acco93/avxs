//
// Created by acco on 10/16/18.
//

#ifndef XSTTRP_TIMER_HPP
#define XSTTRP_TIMER_HPP

#include <chrono>

class timer {

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::high_resolution_clock::now();
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::high_resolution_clock::now();

public:

    timer() = default;

    void start() {
        begin = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        end = std::chrono::high_resolution_clock::now();
    }

    long elapsed(){
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    long lap() {
        const auto value = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - begin).count();
        return value;
    }


};


#endif //XSTTRP_TIMER_HPP
