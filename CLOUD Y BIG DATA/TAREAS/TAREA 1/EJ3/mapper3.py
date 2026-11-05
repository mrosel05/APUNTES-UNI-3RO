#!/usr/bin/python

import sys

is_header = True

for line in sys.stdin:
    if is_header: 
        is_header = False
        continue
    words = line.strip().split(',')
    print(f"{words[1]}\t({words[3]}, {words[3]}, {1})")   
    
        
