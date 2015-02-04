#!/usr/bin/python

import numpy as np
import scipy.stats as st
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('alpha', type=float)
parser.add_argument('beta', type=float)

args = parser.parse_args()

xs = np.linspace(0, 1, 100)
ys = st.beta.pdf(xs, args.alpha, args.beta)
plt.plot(xs, ys)
plt.show()
