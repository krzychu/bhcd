#include <unordered_map>
#include <cmath>
#include <cassert>
#include <iostream>
#include <string>
#include <algorithm>
#include "merge.h"
#include "engine.h"
#include "forest.h"

using std::vector;
using std::unordered_map;
using std::ostream;
using std::endl;
using std::map;
using std::string;
using std::set_union;
using std::back_inserter;
using std::copy_if;
using std::string;
using std::to_string;


namespace BHCD
{
    static string get_label(int id, const unordered_map<int, string>& label)
    {
        auto itr = label.find(id);  

        if(itr == label.end())
            return "__bhcd_" + to_string(id);
        else
            return itr->second;
    }

   
    map<string, string> Engine::infer_hierarchy(ostream& log, const Network& network) const
    {
        MergeQueue queue(4.5 * network.get_num_edges());
        Forest forest(model, network.get_num_vertices());

        unordered_map<string, int> node_of_label;
        unordered_map<int, string> label_of_node;
        for(auto itr = network.vertex_begin(); itr != network.vertex_end(); itr++)
        {
            int root = forest.add_root();
            node_of_label[*itr] = root;
            label_of_node[root] = *itr;
        }

        for(auto itr = network.edge_begin(); itr != network.edge_end(); itr++)
        {
            int t1 = node_of_label[itr->first.v1];
            int t2 = node_of_label[itr->first.v2];

            const EdgeStatistics e = itr->second;
            forest.add_graph_edge(t1, t2, e);

            queue.push(find_best_merge(forest, t1, Edge(t2, e)));
        }

        log << "done" << endl;

        int _n = 0;
        const int queue_purge_threshold = 4 * queue.size();
        while(!queue.empty())
        {
            if(queue.size() > queue_purge_threshold)
                queue.purge(forest);

            Merge m = queue.pop();

            if(!m.valid(forest))
                continue;
          
            log << "iter: " << (_n++) 
                << " size: " << queue.size() 
                << " " << m.get_op_name() 
                << " " << get_label(m.t1, label_of_node)
                << " with " << get_label(m.t2, label_of_node) 
                << endl;

            const Node& new_node = m.perform(forest);
            log << "created " << get_label(new_node.id, label_of_node) << endl;

            for(auto itr = new_node.graph_edges.begin(); itr != new_node.graph_edges.end(); itr++)
                queue.push(find_best_merge(forest, new_node.id, *itr));
        }

        map<string, string> output;
        for(auto itr = forest.begin(); itr != forest.end(); itr++)
        {
            if(!itr->is_alive())
                continue;

            const string parent_label = get_label(itr->id, label_of_node);

            if(itr->is_root())
                output[parent_label] = parent_label;

            for(auto ch = itr->tree_children.begin(); ch != itr->tree_children.end(); ch++)
            {
                log << get_label(*ch, label_of_node) << " " << parent_label << endl;
                const string child_label = get_label(*ch, label_of_node); 
                output[child_label] = parent_label;
            }
        }

        return output;
    }
}
