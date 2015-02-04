#include <iostream>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <stdexcept>

#include "engine.h"
#include "likelihood.h"

using namespace std;
using namespace BHCD;

int line_number = 0;

void ensure_command(istream& stream, const string& expected)
{
    string command;
    stream >> command;

    if(command != expected)
        throw runtime_error("expected " + expected + ", got " + command);
}

Engine* read_engine(istream& stream)
{
    ensure_command(stream, "likelihood");
    string likelihood_type;
    stream >> likelihood_type;
    line_number++;

    ensure_command(stream, "split");
    string split_type;
    stream >> split_type;
    line_number++;

    ensure_command(stream, "gamma");
    double gamma;
    stream >> gamma;
    line_number++;

    ensure_command(stream, "internal");
    double internal_alpha, internal_beta;
    stream >> internal_alpha >> internal_beta; 
    line_number++;
    
    ensure_command(stream, "external");
    double external_alpha, external_beta;
    stream >> external_alpha >> external_beta; 
    line_number++;

    Split * split = nullptr;
    if(split_type == "soft")
    {
        split = new SoftSplit(gamma);
    }
    else if(split_type == "threshold")
    {
        split = new ThresholdSplit(gamma);
    }
    else
    {
        cerr << "unknown split: " << split_type << endl;
        exit(-1);
    }

    Likelihood * internal = nullptr;
    Likelihood * external = nullptr; 

    if(likelihood_type == "beta")
    {
        internal = new BetaLikelihood(5000000, internal_alpha, internal_beta);
        external = new BetaLikelihood(5000000, external_alpha, external_beta);
    }
    else if(likelihood_type == "gamma")
    {
        internal = new GammaLikelihood(5000000, internal_alpha, internal_beta);
        external = new GammaLikelihood(5000000, external_alpha, external_beta);
    }
    else
    {
        cerr << "unknown likelihood type: " << likelihood_type << endl;
        exit(-1);
    }

    return new Engine(internal, external, split);
}

Network* read_network(istream& stream)
{
    ensure_command(stream, "num_edge_defs");
    int num_edges;
    stream >> num_edges;
    line_number++;

    Network* net = new Network();
    for(int i = 0; i < num_edges; i++)
    {
        ensure_command(stream, "edges"); 
        int present, missing;
        string label1, label2;
        stream >> label1 >> label2 >> present >> missing;
        line_number++;
        net->add_edges(label1, label2, present, missing);

        if(line_number % 10000 == 0)
            cerr << line_number << endl;
    }

    return net;
}

int main(int argc, const char** argv)
{
    ios_base::sync_with_stdio(false);

    if(argc != 3)
    {
        cerr << "usage: " << argv[0] << " hyperparameters graph" << endl;
        return -1;
    }

    const char * hyperparameters_filename = argv[1];
    const char * graph_filename = argv[2];

    ifstream graph(graph_filename);
    if(!graph.is_open())
    {
        cerr << "unable to open " << graph_filename << endl;
        return -1;
    }

    ifstream hyperparameters(hyperparameters_filename);
    if(!hyperparameters.is_open())
    {
        cerr << "unable to open " << hyperparameters_filename << endl;
        return -1;
    }

    auto_ptr<Engine> engine(read_engine(hyperparameters));
    cerr << "hyperparameters read" << endl;

    auto_ptr<Network> network(read_network(graph));
    cerr << "graph read" << endl;

    auto result = engine->infer_hierarchy(cerr, *network);

    cout << "num_edges " << result.size() << "\n";
    for(auto itr = result.begin(); itr != result.end(); itr++)
        cout << itr->first << " " << itr->second << "\n";


    return 0;
}
