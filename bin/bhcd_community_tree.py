#!/usr/bin/python

import argparse
import graphviz
import itertools
import tree
import os.path

def render_community_tree(tree, output_file_name):
    g = graphviz.Graph()
    for v in tree.keys():
        g.node(v, shape='point')

    for p in tree:
        for c in tree[p]:
            g.edge(c, p)

    g.render(output_file_name)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('tree_file_name', type=str)

    args = parser.parse_args()
    (output, ext) = os.path.splitext(args.tree_file_name)

    if not ext:
        raise RuntimeError('no extension!')

    tree = tree.parse_tree(args.tree_file_name)
    render_community_tree(tree, output)
