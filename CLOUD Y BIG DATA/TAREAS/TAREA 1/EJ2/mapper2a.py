#!/usr/bin/python

import sys

for line in sys.stdin:
    tokens = line.split()
    
    for i in range(len(tokens)):
        if tokens[i] == "\"GET": 
            print(tokens[i+1].split('?')[0] + "\t1")
