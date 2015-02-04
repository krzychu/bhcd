import collections

def parse_tree(filename):
    tree = collections.defaultdict(list)
    with open(filename) as f:
        n = int(f.readline().split()[1])
        for _ in xrange(n):
            child, parent = f.readline().split()
            if parent != child:
                tree[parent].append(child)

    return tree 
