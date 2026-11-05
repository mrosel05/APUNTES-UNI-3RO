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
    
    # Aceptar iguales es vital para salir de mesetas (Plateau y Royal Road)
    if algoritmo == "escalador": 
        return {"aceptar_iguales": True}         

    # T0 moderado y esquema basado en presupuesto para asegurar enfriamiento gradual
    if algoritmo == "recocido":
        return {"T0": 50.0, "patron": "presupuesto"}                         

    # Umbral inicial U0 dependiente del tamaño k o fijo en 2-3 con presupuesto
    if algoritmo == "umbral":
        return {"U0": 3.0, "patron": "presupuesto"}                         

    # D se ajusta en función de k para soportar funciones engañosas como Deceptive
    if algoritmo == "rrt":
        return {"D": k - 1}                           

    # Nivel inicial bajo (L0=0) y lluvia muy suave adaptada al presupuesto o fija pequeña
    if algoritmo == "diluvio":
        return {"L0": 0.0, "lluvia": 0.5}         

    raise ValueError(f"Algoritmo desconocido: {algoritmo}")
