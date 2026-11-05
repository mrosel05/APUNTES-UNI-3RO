#!/usr/bin/python

import sys
import re

for line in sys.stdin:
    words = re.findall('[a-z]+', line.lower())
    
    for word in words:
        print(word + "\t1")
