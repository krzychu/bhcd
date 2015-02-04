#include "statistics.h"

using std::ostream;

namespace BHCD
{
    EdgeStatistics::EdgeStatistics(int _num_present, int _num_absent)
        : num_present(_num_present), num_absent(_num_absent)
    {}


    const EdgeStatistics EdgeStatistics::operator+(const EdgeStatistics & other) const
    {
        return EdgeStatistics(
                num_present + other.num_present, 
                num_absent + other.num_absent); 
    }


    const EdgeStatistics EdgeStatistics::operator-(const EdgeStatistics & other) const
    {
        return EdgeStatistics(
                num_present - other.num_present, 
                num_absent - other.num_absent); 
    }


    const EdgeStatistics& EdgeStatistics::operator+=(const EdgeStatistics & other)
    {
        num_present += other.num_present;
        num_absent += other.num_absent;
        return *this;
    }
    
    bool EdgeStatistics::operator == (const EdgeStatistics& other) const
    {
        return num_present == other.num_present && num_absent == other.num_absent;
    }


    ostream& operator<<(ostream& stream, const EdgeStatistics& stats)
    {
        stream 
            << "num_present = " << stats.num_present 
            << " num_absent = " << stats.num_absent
            << "\n";

        return stream;
    }


    }
