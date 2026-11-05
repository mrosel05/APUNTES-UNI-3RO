#!/usr/bin/python

import sys

is_header = True

for line in sys.stdin: #acumulado, num, id
    if is_header: 
        is_header = False
        continue
    words = line.strip().split(',')
    print(f"{words[2]}\t{1}\t{words[1]}")   
