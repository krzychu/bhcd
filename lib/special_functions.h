#ifndef _BHCD_SPECIAL_FUNCTIONS_H_
#define _BHCD_SPECIAL_FUNCTIONS_H_

#include <utility>
#include <vector>

namespace BHCD
{
    double log_gamma(double z);
    double log_beta(double alpha, double beta);

    // = log(alpha * exp(a) + (1-alpha) * exp(b))
    double log_combine(double alpha, double a, double b);

    size_t hash_combine(size_t a, size_t b);

    class CachedLogGamma
    {
        private:
            std::vector<double> cache;
            double delta;

        public:
            CachedLogGamma(int cache_size, double delta);
            double operator() (int n) const;
    };
}

#endif
