from pyspark import SparkConf, SparkContext
import re, sys

conf = SparkConf().setAppName('TopK')
sc = SparkContext.getOrCreate(conf)

lines = sc.textFile(sys.argv[1])

def getResource(line):
    tokens = line.split()
    for i in range(len(tokens)):
        if tokens[i] == "\"GET": 
            return tokens[i+1].split('?')[0]

top = (lines
    .map(getResource)
    .map(lambda word: (word, 1))
    .reduceByKey(lambda a, b: a + b)
    .top(100, lambda x: x[1])
    )

sc.parallelize(top, 1).saveAsTextFile(sys.argv[2])
