#!/usr/bin/python

import sys

prev = ""
sum, count = 0, 0

for line in sys.stdin:
    aux1, aux2, id = line.strip().split('\t')
    rating, number = float(aux1), float(aux2)

    if id == prev: 
        sum += rating * number
        count += number
    else: 
        if prev != "": 
            print(f"{sum/count:.1f}\t{count}\t{prev}")
        prev = id
        sum, count = rating * number, number

if prev != "": 
    print(f"{sum/count:.1f}\t{count}\t{prev}")




