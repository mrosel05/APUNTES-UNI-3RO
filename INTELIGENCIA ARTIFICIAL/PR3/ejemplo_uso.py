"""
ejemplo_uso.py — Cómo lanzar ejecuciones y guardar resultados
=============================================================
Esto NO es el experimento: solo muestra la mecánica. Qué probar, cuántas
réplicas, qué semillas y cómo analizarlo lo decidís vosotros.

Regla de oro: un Problema NUEVO por ejecución (lleva la cuenta del presupuesto).
"""

import csv

from bloques import Problema, ejecutar

# --- una ejecución --------------------------------------------------------
prob = Problema("deceptive", n=32, k=4, presupuesto=20000)
res = ejecutar("escalador", {"aceptar_iguales": True}, prob, semilla=1)
print("Una ejecución:", res, "| evaluaciones gastadas:", prob.evaluaciones)
print("Evolución del mejor F (evaluación, F):", res.historia)

# --- varias réplicas a un CSV ----------------------------------------------
with open("resultados_ejemplo.csv", "w", newline="") as fh:
    w = csv.writer(fh)
    w.writerow(["algoritmo", "parametros", "funcion", "n", "k", "presupuesto",
                "semilla", "mejor_f", "optimo", "evals_hasta_optimo"])
    for semilla in range(1, 6):            # <- ¿cuántas réplicas? ¿qué semillas?
        prob = Problema("deceptive", n=32, k=4, presupuesto=20000)
        params = {"aceptar_iguales": True}
        r = ejecutar("escalador", params, prob, semilla)
        w.writerow(["escalador", params, prob.funcion, prob.n, prob.k, prob.presupuesto,
                    semilla, r.mejor_f, prob.optimo, r.evals_hasta_optimo])
print("Guardado resultados_ejemplo.csv")
