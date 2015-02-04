import collections
import itertools


def parse_communities(filename):
    communities = []
    with open(filename) as f:
        for line in f.readlines():
            name, labels = line.split(':')
            communities.append((name.strip(), labels.split()))
    return communities


def dump_communities(filename, communities):
    with open(filename, 'w') as f:
        for n, c in communities:
            f.write('{0} : {1}\n'.format(n, ' '.join(c)))


def extract_tree_communities(tree):
    community = {} 
    for t in tree:
        if t in community:
            continue

        stack = [t]
        while stack:
            c = stack.pop()

            if c in community:
                continue

            if not c in tree:
                community[c] = [c]
                continue
            
            unprocessed = filter(lambda x: not x in community, tree[c])
            
            if unprocessed:
                stack.append(c)
                stack += unprocessed
            else:
                cs = map(lambda x: community[x], tree[c])
                community[c] = list(itertools.chain(*cs))
    
    return community.items()


class Stats:
    def __init__(self):
        self.num_present = 0
        self.num_absent = 0

    def add(self, num_present, num_absent):
        self.num_present += num_present
        self.num_absent += num_absent

    def net(self):
        return self.num_present - self.num_absent


class CommunityStats:
    def __init__(self, size):
        self.size = size
        self.internal = Stats()
        self.external = Stats()


def get_stats(communities, edges, min_size, max_size):
    cs = filter(lambda x: min_size <= len(x[1]) <= max_size, communities)
    label_communities = collections.defaultdict(list)
    stats = {}
    for (name, members) in cs:
        stats[name] = CommunityStats(len(members))
        for label in members:
            label_communities[label].append(name)
    
    for (l1, l2, num_present, num_absent) in edges:
        cs1, cs2 = label_communities[l1], label_communities[l2] 
        for c1, c2 in itertools.product(cs1, cs2):
            if c1 == c2:
                stats[c1].internal.add(num_present, num_absent)
            else:
                stats[c1].external.add(num_present, num_absent)
                stats[c2].external.add(num_present, num_absent)
    
    return stats.values()


def inv_conductance(stats):
    internal = stats.internal.net()
    external = stats.external.net()

    total = abs(internal) + abs(external)
    
    if total == 0:
        return 0
    else:
        return float(internal) / float(total)
