#ifndef _BHCD_MERGE_H_
#define _BHCD_MERGE_H_

#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include "statistics.h"
#include "forest.h"
#include "model.h"

namespace BHCD
{
    struct MergeCache;

    struct Merge 
    {
        enum class Operation { ABSORB, REV_ABSORB, JOIN };
        
        Operation operation;
        double gain;  

        int t1;
        int t2;

        Merge(Operation _op = Operation::ABSORB, double _gain = 0, int _t1 = 0, int _t2 = 0) :
            operation(_op),
            gain(_gain),
            t1(_t1), 
            t2(_t2)
        {}
          
        std::string get_op_name() const;
        bool valid(const Forest& forest) const;
        const Node& perform(Forest& forest);
    };


    std::ostream& operator<< (std::ostream& stream, const Merge& merge);


    struct MergeComparator
    {
        bool operator() (const Merge& a, const Merge& b) const
        { 
            return a.gain < b.gain;
        }
    };


    class MergeQueue
    {
        private: 
            const int max_size;
            Merge* ms;
            MergeComparator cmp;
            int num_elements;

        public:
            MergeQueue(int _max_size);
            MergeQueue(const MergeQueue& other) = delete;
            MergeQueue& operator= (const MergeQueue& other) = delete;
            ~MergeQueue();

            void push(const Merge& merge);
            Merge pop();

            bool empty() const;
            int size() const;

            void purge(const Forest& f);
    };

    Merge find_best_merge(Forest& forest, int tree, const Edge& edge);
}

#endif
