#include <algorithm>
#include <limits>
#include <cassert>
#include <iostream>
#include "merge.h"
#include "special_functions.h"

using namespace std;

namespace BHCD
{
    std::string Merge::get_op_name() const
    {
        switch(operation)
        {
            case Operation::ABSORB:     return "absorb";
            case Operation::REV_ABSORB: return "reverse absorb";
            case Operation::JOIN:       return "join";
            default:                    throw runtime_error("merge type not supported");
        }
    }


    const Node& Merge::perform(Forest& f)
    {
        assert(valid(f));

        switch(operation)
        {
            case Operation::ABSORB:     return f.absorb(t1, t2);
            case Operation::REV_ABSORB: return f.absorb(t2, t1);
            case Operation::JOIN:       return f.join(t1, t2); 
            default:                    throw runtime_error("merge type not supported");
        }
    }


    Merge find_best_merge(Forest& forest, int a_id, const Edge& edge)
    {
        const Node& a = forest.get_node(a_id);
        assert(a.is_root());

        const int b_id = forest.get_root(edge.to);
        const Node& b = forest.get_node(b_id);
        assert(b.is_root());

        EdgeStatistics e = edge.stats;

        // probability of current state 
        const double current_log_likelihood = 
            a.stats.log_likelihood + 
            b.stats.log_likelihood + 
            forest.model.external.log(e);
    
        Merge::Operation op = Merge::Operation::JOIN;
        double ll = forest.get_join_stats(a.id, b.id, e).log_likelihood;

        double absorb = forest.get_absorb_stats(a.id, b.id, e).log_likelihood;
        if(absorb > ll && !a.tree_children.empty())
        {
            ll = absorb;
            op = Merge::Operation::ABSORB;
        }

        double rev_absorb = forest.get_absorb_stats(b.id, a.id, e).log_likelihood;
        if(rev_absorb > ll && !b.tree_children.empty())
        {
            ll = rev_absorb;
            op = Merge::Operation::REV_ABSORB;
        }

        return Merge(op, ll - current_log_likelihood, a.id, b.id);
    }


    bool Merge::valid(const Forest& forest) const
    {
        const Node& a = forest.get_node(t1);
        const Node& b = forest.get_node(t2);

        switch(operation)
        {
            case Operation::JOIN:       return a.is_root() && b.is_root();
            case Operation::ABSORB:     return a.is_root() && b.is_root() && !a.tree_children.empty();
            case Operation::REV_ABSORB: return a.is_root() && b.is_root() && !b.tree_children.empty();
            default:                    throw runtime_error("merge type not supported");
        }
    }


    ostream& operator<< (ostream& stream, const Merge& merge)
    {
        switch(merge.operation)
        {
            case Merge::Operation::ABSORB: stream << "ABSORB ";
            case Merge::Operation::REV_ABSORB: stream << "REV_ABSORB ";
            case Merge::Operation::JOIN:   stream << "JOIN ";
        }
        stream << merge.t1 << " " << merge.t2 << " " << merge.gain;
        return stream;
    }


    MergeQueue::MergeQueue(int _max_size)
        : max_size(_max_size), ms(new Merge[_max_size]), num_elements(0)
    {
    }


    void MergeQueue::push(const Merge& merge)
    {
        assert(num_elements < max_size);
        ms[num_elements++] = merge;
        push_heap(ms, ms + num_elements, cmp);
    }


    Merge MergeQueue::pop()
    {
        assert(num_elements > 0);

        pop_heap(ms, ms + num_elements, cmp);
        return ms[--num_elements];
    }


    bool MergeQueue::empty() const
    {
        return num_elements == 0;
    }


    int MergeQueue::size() const
    {
        return num_elements;
    }


    void MergeQueue::purge(const Forest& forest)
    {
        auto last = remove_if(ms, ms + num_elements, 
                [&](const Merge& m) -> bool { return !m.valid(forest); });
        
        num_elements = last - ms;
        make_heap(ms, ms + num_elements, cmp);
    }


    MergeQueue::~MergeQueue()
    {
        delete ms;
    }
}
