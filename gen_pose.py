#!/bin/env/python3

import numpy as np

a = np.random.rand(20, 6)

file = open("a", "w")

for i in range(a.shape[0]):
    row = a[i, :]
    row_str = " ".join(np.vectorize(str)(row))
    file.write(row_str)
    file.write('\n')

