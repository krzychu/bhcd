#!/usr/bin/python

import subprocess
import argparse
import re
import itertools


def get_git_log(limit):
    args = ['git', 'log', '--all', '--name-status', '--pretty=format:%H %ae']
    if limit:
        args.append('-{0}'.format(limit))

    p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    (out, err) = p.communicate()

    if err:
        raise RuntimeError(err)

    return out


def get_file_authors(log):
    header_re = re.compile('^(?P<hash>[a-z0-9]+) (?P<email>.+)$')
    file_re = re.compile('^[A-Z]\s+(?P<path>.*)$')
    
    authors = {}
    current_author = None
    for line in log.splitlines():
        if line.strip() == '':
            continue

        fm = file_re.match(line)
        if fm:
            if not current_author:
                continue

            path = fm.group('path')
            if not path in authors:
                authors[path] = set()

            authors[path].add(current_author)
        else:
            hm = header_re.match(line)
            if not hm:
                continue

            current_author = hm.group('email')

    return authors 


def count_common_paths(authors):
    num_common = {}

    for (_, modifiers) in authors.items():
        for (a, b) in itertools.product(modifiers, modifiers):
            if a >= b: 
                continue

            key = (a, b)
            if not key in num_common:
                num_common[key] = 0

            num_common[key] += 1

    return num_common


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', type=int, default=-1)

    args = parser.parse_args()
    if args.n == -1:
        args.n = None

    log = get_git_log(args.n)
    authors = get_file_authors(log)
    num_common = count_common_paths(authors)

    print 'num_edge_defs', len(num_common)

    for ((a, b), n) in num_common.items():
        print  'edges {0} {1} {2} 0'.format(a, b, n)
