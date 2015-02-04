#ifndef _BHCD_NETWORK_H_
#define _BHCD_NETWORK_H_

#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include "statistics.h"
#include "special_functions.h"
#include "forest.h"

namespace BHCD
{
    struct Relation
    {
        std::string v1;
        std::string v2;

        Relation(const std::string& _v1, const std::string& _v2) :
            v1(std::min(_v1, _v2)), v2(std::max(_v1, _v2))
        {}
    };


    inline bool operator==(const Relation& a, const Relation& b) 
    {
        return a.v1 == b.v1 && a.v2 == b.v2;
    }


    struct RelationHash
    {
        size_t operator() (const Relation& r) const
        {
            std::hash<std::string> h;
            return hash_combine(h(r.v1), h(r.v2));
        }
    };


    class Network
    {
        private:
            int num_vertices;
            std::unordered_map<Relation, EdgeStatistics, RelationHash> edges;
            std::unordered_set<std::string> vertices;

        public:
            Network(const Network& other) = delete;
            Network& operator=(const Network& other) = delete;
            Network() {}

            void add_edges(const std::string& label1, const std::string& label2, int num_present, int num_absent);
            int get_num_vertices() const;
            int get_num_edges() const;

            typedef std::unordered_map<Relation, EdgeStatistics, RelationHash>::const_iterator edge_const_iterator;
            edge_const_iterator edge_begin() const;
            edge_const_iterator edge_end() const;

            typedef std::unordered_set<std::string>::const_iterator vertex_const_iterator;
            vertex_const_iterator vertex_begin() const;
            vertex_const_iterator vertex_end() const;
    };
}

#endif
