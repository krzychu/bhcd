#!/usr/bin/python

import tree
import community
import argparse
import os.path

parser = argparse.ArgumentParser()
parser.add_argument('tree_file')
args = parser.parse_args()

base, ext = os.path.splitext(args.tree_file)

tree = tree.parse_tree(args.tree_file)
communities = community.extract_tree_communities(tree, 0)
community.dump_communities(base + '.inferred', communities)
