#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "forest.h"

using std::vector;
using std::list;
using std::unordered_map;
using std::unordered_set;


namespace BHCD
{   
    NodeStatistics join_node_stats(
            const Model& model,
            const NodeStatistics& stats_a,
            const NodeStatistics& stats_b,
            const EdgeStatistics& edge_stats)
    {
        const EdgeStatistics total = 
            stats_a.total + 
            stats_b.total + 
            edge_stats;

        const double children = 
            stats_a.log_likelihood + 
            stats_b.log_likelihood;

        const double split = model.split.probability(2);

        const double internal = model.internal.log(total);
        const double external = model.external.log(edge_stats);
        const double log_likelihood = 
            log_combine(1 - split, internal, external + children);

        return NodeStatistics(total, edge_stats, log_likelihood, children);
    }


    NodeStatistics absorb_node_stats(
            const Model& model,
            int num_children,
            const NodeStatistics& stats_tree,
            const NodeStatistics& stats_subtree,
            const EdgeStatistics& edge_stats)
    {
        const EdgeStatistics total = 
            stats_tree.total + 
            stats_subtree.total + 
            edge_stats;

        const EdgeStatistics cross =  
            stats_tree.cross + 
            edge_stats;

        const double children = 
            stats_tree.children_log_likelihood +
            stats_subtree.log_likelihood;

        const double split = model.split.probability(num_children);
        const double internal = model.internal.log(total);
        const double external = model.external.log(cross);
        const double log_likelihood = 
            log_combine(1 - split, internal, external + children);

        return NodeStatistics(total, cross, log_likelihood, children);        
    }


    EdgeStatistics Forest::get_edge_statistics(const Node& a, const Node& b) const
    {
        const list<Edge>& e = a.graph_edges;

        for(auto itr = e.begin(); itr != e.end(); itr++)
            if(itr->to == b.id)
                return itr->stats;
   
        return EdgeStatistics();
    }



    Forest::Forest(const Model& _model, int initial_capacity)
        : model(_model)
    {
        nodes.reserve(initial_capacity); 
    }


    const Node& Forest::get_node(int id) const
    {
        assert(id < static_cast<int>(nodes.size()));
        return nodes[id];
    }


    int Forest::get_root(int id)
    {
        int root = id;
        while(n(root).repr != root)
            root = n(root).repr;
       
        int current = id;
        while(n(current).repr != root)
        {
            int next = n(current).repr;
            n(current).repr = root;
            current = next;
        }

        return root;
    }


    Node& Forest::n(int id)
    {
        assert(id < static_cast<int>(nodes.size()));
        return nodes[id];
    }

    
    int Forest::add_root()
    {
        int idx = static_cast<int>(nodes.size());
        nodes.push_back(Node(idx));

        return idx;
    }


    vector<Edge> Forest::prepare_edges(list<Edge>& edge_list, int except)
    {
        vector<Edge> v;
        for(auto itr = edge_list.begin(); itr != edge_list.end(); itr++)
        {
            int root = get_root(itr->to);
            if(root == except)
                continue;

            v.push_back(Edge(root, itr->stats));
        }

        edge_list.clear();
        std::sort(v.begin(), v.end());

        return v;
    }


    void Forest::merge_graph_edges_into(Node& target, Node& a, Node& b)
    {
        assert(target.graph_edges.empty());
        assert(a.is_root());
        assert(b.is_root());
        assert(target.is_root());

        auto ae = prepare_edges(a.graph_edges, b.id);
        auto be = prepare_edges(b.graph_edges, a.id);

        auto ai = ae.begin();
        auto bi = be.begin();

        while (ai != ae.end() && bi != be.end())
        {
            if(ai->to < bi->to)
            {
                target.graph_edges.push_back(*ai);
                ai++;
            }
            else if (ai->to > bi->to)
            {
                target.graph_edges.push_back(*bi);
                bi++;
            }
            else
            {
                target.graph_edges.push_back(Edge(ai->to, ai->stats + bi->stats));
                ai++;
                bi++;
            }
        }

        while(ai != ae.end())
        {
            target.graph_edges.push_back(*ai);
            ai++;
        }

        while(bi != be.end())
        {
            target.graph_edges.push_back(*bi);
            bi++;
        }
    }


    const Node& Forest::join(int a_id, int b_id)
    {
        int root_id = add_root();
        Node& root = n(root_id);
        Node& a = n(a_id);
        Node& b = n(b_id);

        assert(a.is_root());
        assert(b.is_root());
        assert(a.is_alive());
        assert(b.is_alive());

        EdgeStatistics between = get_edge_statistics(a, b);
        NodeStatistics stats = get_join_stats(a.id, b.id, between);

        assert(root.is_root());
        merge_graph_edges_into(root, a, b);
        root.tree_children.push_back(a.id);
        root.tree_children.push_back(b.id);
        root.stats = stats;
        
        a.repr = root_id;
        b.repr = root_id;

        assert(!a.is_root());
        assert(!b.is_root());

        return root;
    }


    const Node& Forest::absorb(int tree_id, int subtree_id)
    { 
        int root_id = add_root();
        Node& root = n(root_id);

        Node& tree = n(tree_id);
        Node& subtree = n(subtree_id);

        assert(tree.is_root());
        assert(subtree.is_root());
        assert(!tree.tree_children.empty());

        EdgeStatistics between = get_edge_statistics(tree_id, subtree_id);
        NodeStatistics stats = get_absorb_stats(tree_id, subtree_id, between);

        root.tree_children = tree.tree_children;
        root.tree_children.push_back(subtree.id);
        merge_graph_edges_into(root, tree, subtree);
        root.stats = stats;

        tree.repr = root_id;
        tree.kill();
        subtree.repr = root_id;
          
        return root;
    }


    Edge Forest::add_graph_edge(int a_id, int b_id, const EdgeStatistics& stats)
    {
        Node& a = n(a_id);
        Node& b = n(b_id);
        a.graph_edges.push_back(Edge(b.id, stats));
        b.graph_edges.push_back(Edge(a.id, stats));

        return Edge(b.id, stats);
    }


    NodeStatistics Forest::get_join_stats(int a_id, int b_id, const EdgeStatistics& between) const
    {
        const Node& a = get_node(a_id);
        const Node& b = get_node(b_id);

        return join_node_stats(model, a.stats, b.stats, between);
    }


    NodeStatistics Forest::get_absorb_stats(int tree_id, int subtree_id, const EdgeStatistics& between) const
    {
        const Node& tree = get_node(tree_id);
        const Node& subtree = get_node(subtree_id);
        int num_trees = static_cast<int>(tree.tree_children.size() + 1);

        return absorb_node_stats(model, num_trees, tree.stats, subtree.stats, between);
    }


    vector<Node>::const_iterator Forest::begin() const
    {
        return nodes.cbegin();
    }


    vector<Node>::const_iterator Forest::end() const
    {
        return nodes.end();
    }
}
