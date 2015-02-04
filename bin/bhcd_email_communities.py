#!/usr/bin/python

import re
import argparse
import collections
import graph


email_re = re.compile(r'[^@]+@(?P<domain>[^@]+)')
def get_domain(email):
    m = email_re.match(email)
    return None if not m else m.group('domain')


def group_emails_by_domain(emails):
    by_domain = collections.defaultdict(list)
    for email in emails:
        domain = get_domain(email)
        by_domain[domain].append(email)
    return by_domain


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename')

    args = parser.parse_args()
    edges = graph.parse_edges(args.filename)
    emails = graph.extract_labels(edges)
    
    for domain, emails in group_emails_by_domain(emails).items():
        print '{0} : {1}'.format(domain, ' '.join(emails))
