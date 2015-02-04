#include <cmath>
#include "split.h"


namespace BHCD
{
    double ThresholdSplit::probability(int num_children) const
    {
        return num_children > 0 ? probability_when_has_children : 0.0;
    }

    
    double SoftSplit::probability(int num_children) const
    {
        return 1.0 - exp(log(1.0 - gamma) * num_children);
    }
}
