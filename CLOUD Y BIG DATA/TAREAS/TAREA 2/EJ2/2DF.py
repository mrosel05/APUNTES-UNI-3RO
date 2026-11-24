from pyspark.sql import SparkSession
import pyspark.sql.functions as F
import sys

spark = SparkSession.builder.appName("TopK").getOrCreate()

lines = spark.read.text(sys.argv[1])

top = (lines
    .withColumn("tokens", F.split(F.col("value"), " "))
    .select(F.col("tokens"), F.posexplode(F.col("tokens")).alias("pos", "token"))
    .filter(F.col("token").eqNullSafe('"GET'))
    .withColumn("recurso_raw", F.element_at(F.col("tokens"), F.col("pos") + 2))
    .withColumn("recurso", F.split(F.col("recurso_raw"), r"\?").getItem(0))
    .groupBy("recurso")
    .count()
    .orderBy(F.desc("count"))
    .limit(100)
    )

top.select(F.concat_ws(" ", F.col("recurso"), F.col("count"))
           ).coalesce(1).write.text(sys.argv[2])