#!/usr/bin/python

import sys

list = []

for line in sys.stdin:
    info = line.split('\t')
    list.append((info[0], int(info[1])))
    list.sort(key= lambda x : x[1], reverse= True)
    list = list[:100]

for element in list: 
    print(f"{element[0]}\t{element[1]}")

    