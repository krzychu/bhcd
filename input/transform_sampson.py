import re
import numpy as np
import itertools

all_labels = [
    'SAMPLK1',
    'SAMPLK2',
    'SAMPLK3',
    'SAMPDLK',
    'SAMPES',
    'SAMPDES',
    'SAMPIN',
    'SAMPNIN',
    'SAMPPR',
    'SAMNPR'
]

positive = [
    'SAMPLK3',
    'SAMPES',
    'SAMPIN',
    'SAMPPR',
]

negative = [
    'SAMPDLK',
    'SAMPDES',
    'SAMPNIN',
    'SAMNPR'
]


def read_parse(f, pattern):
    line = f.readline().strip()
    m = re.match(pattern, line)
    assert m
    return m.groups()


def count(matrices, labels, i, j):
    return sum(map(lambda m: int(matrices[m][i, j] + matrices[m][j, i]), labels))


with open('sampson.dat') as f:
    read_parse(f, 'DL')
    num_monks, num_matrices = map(int, read_parse(f, 'N=(\d+) NM=(\d+)'))
    read_parse(f, 'FORMAT = FULLMATRIX DIAGONAL PRESENT')

    read_parse(f, 'ROW LABELS:')
    row_labels = []
    for _ in xrange(num_monks):
        row_labels.append(read_parse(f, '([A-Z\d_]+)')[0])
        
    read_parse(f, 'COLUMN LABELS:')
    column_labels = []
    for _ in xrange(num_monks):
        column_labels.append(read_parse(f, '([A-Z\d_]+)')[0])

    assert column_labels == row_labels

    read_parse(f, 'LEVEL LABELS:')
    matrix_labels = []
    for _ in xrange(num_matrices):
        matrix_labels.append(read_parse(f, '([A-Z\d_]+)')[0])
   
    read_parse(f, 'DATA:')
    matrices = {}
    for label in matrix_labels:
        m = np.zeros((num_monks, num_monks))
        for r in xrange(num_monks):
            m[r] = map(int, f.readline().split())

        matrices[label] = m
  
    edges = []
    for i in range(num_monks):
        for j in range(i):
            a, b = row_labels[i], row_labels[j]

            p = count(matrices, positive, i, j)
            n = count(matrices, negative, i, j)

            if p or n:
                edges.append((a, b, p, n))

    with open('sampson_graph.txt', 'w') as out:
        out.write('num_edge_defs {0}\n'.format(len(edges)))
        for e in edges:
            out.write('edges {0} {1} {2} {3}\n'.format(*e))
