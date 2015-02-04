def parse_edges(filename):
    with open(filename) as f:
        f.readline()
        edges = []
        for line in f.readlines():
            _, l1, l2, p, a = line.split()
            p = int(p)
            a = int(a)
            edges.append((l1, l2, p, a))

        return edges


def extract_labels(edges):
    labels = set()
    for (l1, l2, _, _) in edges:
        labels.add(l1)
        labels.add(l2)

    return list(labels)
