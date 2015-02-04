#include <stdexcept>
#include "network.h"

using std::unordered_map;
using std::pair;
using std::vector;
using std::make_pair;
using std::min;
using std::max;
using std::runtime_error;
using std::string;


namespace BHCD
{
    void Network::add_edges(const std::string& label1, const std::string& label2, int num_present, int num_absent)
    {
        vertices.insert(label1);
        vertices.insert(label2);

        Relation key(label1, label2);
        EdgeStatistics s(num_present, num_absent);
        edges[key] += s;
    }


    int Network::get_num_vertices() const
    {
        return static_cast<int>(vertices.size());
    }

    
    int Network::get_num_edges() const
    {
        return static_cast<int>(edges.size());
    }


    Network::edge_const_iterator Network::edge_begin() const
    {
        return edges.begin();
    }


    Network::edge_const_iterator Network::edge_end() const
    {
        return edges.end();
    }


    Network::vertex_const_iterator Network::vertex_begin() const
    {
        return vertices.begin();
    }


    Network::vertex_const_iterator Network::vertex_end() const
    {
        return vertices.end();
    }
}
