from pyspark.sql import SparkSession
import pyspark.sql.functions as F
import sys

spark = SparkSession.builder.appName("GrepDistribuido").getOrCreate()

lines = spark.read.text(sys.argv[1]).coalesce(100)
word = sys.argv[3].lower()

matches = (lines
    .filter(F.lower(F.col('value')).contains(word))
    .coalesce(1)
    )

matches.write.text(sys.argv[2])