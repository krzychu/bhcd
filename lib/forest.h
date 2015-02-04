#ifndef _BHCD_FOREST_H_
#define _BHCD_FOREST_H_

#include <vector>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "model.h"
#include "statistics.h"

namespace BHCD
{
    struct Edge
    {
        int to;
        EdgeStatistics stats; 

        Edge(int _to, const EdgeStatistics& _stats) :
            to(_to),
            stats(_stats)
        {
        }

        Edge() = default;

        bool operator < (const Edge& other) const
        {
            return to < other.to;
        }
    };


    struct Node
    {
        int id;
        int repr;
        bool alive;

        NodeStatistics stats;

        std::vector<int> tree_children;
        std::list<Edge> graph_edges;

        Node(int _id = 0) :
            id(_id),
            repr(_id),
            alive(true),
            stats(NodeStatistics())
        {}

        bool is_root() const 
        {
            return repr == id;
        }

        bool is_alive() const
        {
            return alive;
        }

        void kill() 
        {
            alive = false;
        }
    };

    
    class Forest
    {
        private:
            std::vector<Node> nodes;
            Node& n(int id);
            std::vector<Edge> prepare_edges(std::list<Edge>& edge_list, int except);

        public:
            const Model& model;

            Forest(const Model& _model, int initial_capacity = 0);
            Forest(const Forest& other) = delete;
            Forest& operator = (const Forest& other) = delete;

            const Node& get_node(int id) const;
            int get_root(int id);

            int add_root();
            Edge add_graph_edge(int a_id, int b_id, const EdgeStatistics& stats);

            const Node& join(int a_id, int b_id);
            const Node& absorb(int tree_id, int subtree_id);

            EdgeStatistics get_edge_statistics(const Node& a, const Node& b) const;
            NodeStatistics get_join_stats(int a, int b, const EdgeStatistics& between) const;
            NodeStatistics get_absorb_stats(int tree, int subtree, const EdgeStatistics& between) const;

            void merge_graph_edges_into(Node& target, Node& a, Node& b);

            std::vector<Node>::const_iterator begin() const;
            std::vector<Node>::const_iterator end() const;
    };
}

#endif
