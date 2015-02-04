#ifndef _BHCD_SPLIT_H_
#define _BHCD_SPLIT_H_

namespace BHCD
{
    struct Split
    {
        virtual double probability(int num_children) const = 0;
        virtual ~Split() {}
    };


    struct ThresholdSplit : public Split
    {
        const double probability_when_has_children;

        ThresholdSplit(double _probability) :
            probability_when_has_children(_probability)
        {}

        virtual double probability(int num_children) const;
    };

    
    struct SoftSplit : public Split
    {
        const double gamma;

        SoftSplit(double _gamma)
            : gamma(_gamma)
        {}

        virtual double probability(int num_children) const;
    };
}

#endif
