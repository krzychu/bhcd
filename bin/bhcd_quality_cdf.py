#!/usr/bin/python

import argparse
import graph
import community
import tree
import pdb
import numpy as np
import matplotlib.pyplot as plt


def evaluate_communities(communities, edges, size_range):
    min_size, max_size = size_range
    stats = community.get_stats(communities, edges, min_size, max_size)
    scores = sorted(map(community.inv_conductance, stats))
    ps = np.arange(0, len(scores)) / float(len(scores))
    return (scores, ps)


def plot_scores(communities, edges, label, style, size_range):
    xs, ys = evaluate_communities(communities, edges, size_range)
    plt.plot(xs, ys, style, label=label)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file')
    parser.add_argument('given_communities_file')
    parser.add_argument('inferred_communities_file')
    parser.add_argument('--min_size', type=int, default=2)
    parser.add_argument('--max_size', type=int, default=300)

    args = parser.parse_args()

    edges = graph.parse_edges(args.graph_file)
    given_communities = community.parse_communities(args.given_communities_file)
    inferred_communities = community.parse_communities(args.inferred_communities_file)
    
    size_range = args.min_size, args.max_size
    plt.title('Community Quality CDF')
    plot_scores(inferred_communities, edges, 'inferred', 'b-', size_range)
    plot_scores(given_communities, edges, 'given', 'r-', size_range)
    plt.xlabel('probability')
    plt.ylabel('quality')
    plt.legend()
    plt.show()
