#ifndef _BHCD_LIKELIHOOD_H_
#define _BHCD_LIKELIHOOD_H_

#include <vector>
#include "special_functions.h"
#include "statistics.h"

namespace BHCD 
{
    struct Likelihood
    {
        Likelihood() = default;
        Likelihood(const Likelihood& other) = delete;
        Likelihood& operator = (const Likelihood& other) = delete;

        virtual double log(const EdgeStatistics& stats) const = 0;
        virtual ~Likelihood() {}
    };


    class BetaLikelihood : public Likelihood
    {
        private:
            const double scale;
            
            CachedLogGamma alpha;
            CachedLogGamma beta;
            CachedLogGamma sum;

        public:
            BetaLikelihood(int _cache_size, double _alpha, double _beta);
            virtual double log(const EdgeStatistics& stats) const;
    };


    class GammaLikelihood: public Likelihood
    {
        private:
            const double scale;
            const double alpha;
            const double beta;

            CachedLogGamma gamma_alpha;
            CachedLogGamma gamma_zero;

        public:
            GammaLikelihood(int _cache_size, double _alpha, double _beta);
            virtual double log(const EdgeStatistics& stats) const;
    };
}

#endif
