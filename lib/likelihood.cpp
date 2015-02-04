#include <algorithm>
#include <cmath>
#include <cassert>

#include "likelihood.h"
#include "special_functions.h"

namespace BHCD 
{
    BetaLikelihood::BetaLikelihood(int _cache_size, double _alpha, double _beta) : 
        scale(log_beta(_alpha, _beta)), 
        alpha(_cache_size, _alpha), 
        beta(_cache_size, _beta), 
        sum(_cache_size, _alpha + _beta)
    {
    }


    double BetaLikelihood::log(const EdgeStatistics& stats) const
    {  
        const double gamma = 
            alpha(stats.num_present) + 
            beta(stats.num_absent) -
            sum(stats.num_present + stats.num_absent);

        return gamma - scale;
    }


    GammaLikelihood::GammaLikelihood(int _cache_size, double _alpha, double _beta)
        : scale(_alpha * ::log(_beta) - log_gamma(alpha)),
          alpha(_alpha),
          beta(_beta),
          gamma_alpha(_cache_size, _alpha),
          gamma_zero(_cache_size, 0)
    {
    }


    double GammaLikelihood::log(const EdgeStatistics& stats) const
    {
        const double n = stats.num_present;

        return scale + 
            gamma_alpha(n) - gamma_zero(n + 1) - 
            (n + alpha) * ::log(beta + 1);
    }
}
