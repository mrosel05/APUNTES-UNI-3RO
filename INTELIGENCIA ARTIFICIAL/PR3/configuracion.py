"""
configuracion.py — VUESTRA configuración final (se entrega y se CONGELA)
=======================================================================
Inteligencia Artificial I · Práctica 3 · Optimización

Grupo: ____

Este fichero es lo que el profesor ejecutará en el CONCURSO, sobre instancias
que NO habéis visto: pueden cambiar
   - la función (onemax, plateau, royalroad, deceptive),
   - la longitud n,
   - el tamaño de bloque k (en clase siempre fue 4),
   - el presupuesto de evaluaciones.

Por eso NO se entrega un número fijo por algoritmo, sino una REGLA:
una función que, dada la instancia, devuelve los parámetros.

    configurar("rrt", n=32, k=4, presupuesto=20000)  ->  {"D": ...}

Lo que NO sabe vuestra función es qué tipo de función f se va a usar:
la configuración debe ser buena "en general", no solo en Deceptive.
Si un parámetro depende de n, k o del presupuesto, explicad POR QUÉ en la
memoria, con vuestros datos.

Los valores de abajo son de RELLENO: no están ajustados.
"""


def configurar(algoritmo, n, k, presupuesto):
    if algoritmo == "aleatoria":
        return {}

    if algoritmo == "escalador":
        return {"aceptar_iguales": True}          # TODO: decidir y justificar

    if algoritmo == "recocido":
        return {"T0": 1.0}                         # TODO (+ los parámetros extra que uséis)

    if algoritmo == "umbral":
        return {"U0": 1.0}                         # TODO

    if algoritmo == "rrt":
        return {"D": 1}                            # TODO

    if algoritmo == "diluvio":
        return {"L0": 0.0, "lluvia": 0.0}          # TODO

    raise ValueError(f"Algoritmo desconocido: {algoritmo}")
