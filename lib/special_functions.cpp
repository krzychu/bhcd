#include <cmath>
#include <algorithm>

#include "special_functions.h"

namespace BHCD 
{
    double log_gamma(double z)
    {
        if(z < 0.5)
            return log(M_PI) - log(sin(M_PI * z)) - log_gamma(1 - z);

        const double c[] = 
        {
            0.99999999999980993, 676.5203681218851, -1259.1392167224028,
            771.32342877765313, -176.61502916214059, 12.507343278686905,
            -0.13857109526572012, 9.9843695780195716e-6, 1.5056327351493116e-7
        };

        z -= 1; 

        double sum = c[0];
        for(int i = 1; i < 9; i++)
            sum += c[i] / (z + i);

        double t = z + 7.5;
        return (z + 0.5) * log(t) - t + 0.5 * log(2 * M_PI) + log(sum);
    }


    double log_beta(double alpha, double beta)
    {
        return log_gamma(alpha) + log_gamma(beta) - log_gamma(alpha + beta);
    }

    
    double log_combine(double alpha, double a, double b)
    {
        const double m = std::max(a, b);

        const double tail = log(alpha * exp(a - m) + (1 - alpha) * exp(b - m));

        if(isinf(tail))
            return m;
        else
            return m + tail;
    }


    size_t hash_combine(size_t a, size_t b)
    {
        return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
    }


    CachedLogGamma::CachedLogGamma(int _cache_size, double _delta)
        : cache(_cache_size), delta(_delta)
    {
        for(unsigned int i = 0; i < cache.size(); i++)
            cache[i] = log_gamma(i + delta);
    }


    double CachedLogGamma::operator() (int x) const
    {
        if(x < static_cast<int>(cache.size()))
            return cache[x];
        else
            return log_gamma(x + delta);
    }
}
