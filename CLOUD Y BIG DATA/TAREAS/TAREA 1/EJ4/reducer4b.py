#!/usr/bin/python

import sys

prev = ""
ratingList = []

for line in sys.stdin:
    mean, count, id = line.strip().split('\t')

    if mean == prev: 
        ratingList.append(id)
    else: 
        if prev != "": 
            print(f"{prev}\t{ratingList}")
        prev = mean
        ratingList = []

if prev != "": 
            print(f"{prev}\t{ratingList}")




