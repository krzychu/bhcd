import argparse
import collections
import itertools


authors = collections.defaultdict(list)
with open('nips.csv') as f:
    for line in f.readlines():
        paper, author = line.split(',')
        authors[paper.strip()].append(author.strip())

with open('nips.comm', 'w') as f:
    for (n, a) in authors.items():
        f.write('{0}: {1}\n'.format(n, ' '.join(a)))


num_edges = collections.defaultdict(lambda: 0)
for a in authors.values():
    for x, y in itertools.product(a, a):
        if x >= y:
            continue
        num_edges[(x, y)] += 1


with open('nips.graph', 'w') as f:
    f.write('num_edge_defs {0}\n'.format(len(num_edges)))
    for ((x, y), n) in num_edges.items():
        f.write('edges {0} {1} {2} 0\n'.format(x, y, n))
