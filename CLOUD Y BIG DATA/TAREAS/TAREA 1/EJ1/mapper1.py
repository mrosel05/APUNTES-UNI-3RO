#!/usr/bin/python

import sys
import re

word = sys.argv[1].lower()

for line in sys.stdin:
    words = re.findall('[a-z]+', line.lower())

    if word in words:
        print(line)
