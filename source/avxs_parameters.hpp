//
// Created by acco on 10/19/18.
//

#ifndef XSTTRP_AVXS_PARAMETERS_HPP
#define XSTTRP_AVXS_PARAMETERS_HPP

#ifdef MORE_RESTARTS
#define RESTARTS_NUM (1000u)
#else
#define RESTARTS_NUM (100u)
#endif

#ifdef LONGER_IMPROVEMENT
#define ILS_ITERATIONS (1000u)
#else
#define ILS_ITERATIONS (100u)
#endif


#ifdef NGRANULAR
#define PI_BASE (9999.0f)
#else
#define PI_BASE (1.25f)
#endif

#define ETA_MREM (0.5f)
#define ZETA_SUNLOAD_MIN (30)
#define ZETA_SUNLOAD_MAX (60)
#define CPLEX_TICKS_PER_SECOND (1900.0f)
#define CPLEX_TIME_LIMIT (60u)
#define LRP_NEAR_FACTOR (1)
#define XSTTRP_NEAR_FACTOR (25)

struct avxs_parameters {

    unsigned long near_factor = 1;
    unsigned long rank_factor = 2;
    bool use_rank_fitness = false;
    bool use_near_fitness = false;


};


#endif //XSTTRP_AVXS_PARAMETERS_HPP
