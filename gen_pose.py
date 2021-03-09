#!/bin/env/python3

import numpy as np

a = np.random.rand(20, 2)

file = open("input_poses", "w")

for i in range(a.shape[0]):
    row = np.concatenate((a[i, :], [0, 0, 0, 0]))
    row_str = " ".join(np.vectorize(str)(row))
    file.write(row_str)
    file.write('\n')

