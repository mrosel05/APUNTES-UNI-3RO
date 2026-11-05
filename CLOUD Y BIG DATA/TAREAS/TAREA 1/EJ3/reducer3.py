#!/usr/bin/python

import sys

prev = ""
min, max, total = float('inf'), 0, 0

for line in sys.stdin:
    id, value = line.strip().split('\t')
    value = value.strip().strip('()')
    aux1, aux2, aux3 = value.strip().split(',')
    minDate, maxDate, count = int(aux1), int(aux2), int(aux3)

    if id == prev: 
        if minDate < min: min = minDate
        if maxDate > max: max = maxDate
        total += count
    else: 
        if prev != "": 
            print(f"{prev}\t({min}, {max}, {total})")
        prev = id
        min, max, total = minDate, maxDate, count

if prev != "": 
            print(f"{prev}\t({min}, {max}, {total})")




