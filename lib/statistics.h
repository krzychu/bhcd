#ifndef _BHCD_STATISTICS_H_
#define _BHCD_STATISTICS_H_

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <iostream>
#include "special_functions.h"

namespace BHCD
{
    struct EdgeStatistics 
    {
        int num_present;
        int num_absent;

        EdgeStatistics(int _num_present = 0, int _num_absent = 0);
        const EdgeStatistics operator+(const EdgeStatistics & other) const;
        const EdgeStatistics& operator+=(const EdgeStatistics & other);
        const EdgeStatistics operator-(const EdgeStatistics & other) const;
        bool operator == (const EdgeStatistics& other) const;
    };

    
    std::ostream& operator<<(std::ostream& stream, const EdgeStatistics& stats);


    struct NodeStatistics
    {
        EdgeStatistics total;
        EdgeStatistics cross;
        double log_likelihood;
        double children_log_likelihood;

        NodeStatistics(
                EdgeStatistics _total = EdgeStatistics(), 
                EdgeStatistics _cross = EdgeStatistics(),
                double _log_likelihood = 0, 
                double _children_log_likelihood = 0) :
            total(_total),
            cross(_cross),
            log_likelihood(_log_likelihood),
            children_log_likelihood(_children_log_likelihood)
        {}
    };

}

#endif
