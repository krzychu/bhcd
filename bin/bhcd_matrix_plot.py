#!/usr/bin/python

import argparse
import re
import numpy as np
import matplotlib.pyplot as plt
import graph


def extract_matrix(edges, labels, matrix_name):
    label_id = {}
    for (i, l) in enumerate(labels):
        label_id[l] = i

    n = len(labels)
    m = np.zeros((n, n))

    for (l1, l2, p, a) in edges:
        i1 = label_id[l1]
        i2 = label_id[l2]
        k = p if matrix_name == 'present' else a
        m[i1, i2] += k
        m[i2, i1] += k

    return m


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file', type=str)
    parser.add_argument('matrix', choices=['present', 'absent'])

    args = parser.parse_args()

    edges = graph.parse_edges(args.graph_file)
    labels = graph.extract_labels(edges)
    matrix = extract_matrix(edges, labels, args.matrix)
    
    ticks = range(len(labels))
    plt.title('Number of {0} edges'.format(args.matrix))
    plt.imshow(matrix, interpolation='nearest')
    plt.xticks(ticks, labels, rotation='vertical')
    plt.yticks(ticks, labels)
    plt.show()
