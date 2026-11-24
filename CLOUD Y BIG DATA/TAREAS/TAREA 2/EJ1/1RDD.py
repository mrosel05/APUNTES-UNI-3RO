from pyspark import SparkConf, SparkContext
import re, sys

conf = SparkConf().setAppName('GrepDistribuido')
sc = SparkContext.getOrCreate(conf)

lines = sc.textFile(sys.argv[1]).coalesce(100)
word = sys.argv[3].lower()

matches = (lines
    .filter(lambda x: word in x.lower())
    .coalesce(1)
    )

matches.saveAsTextFile(sys.argv[2])
