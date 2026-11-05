"""
verificar.py — Comprobaciones de la Fase 0 (NO MODIFICAR)
=========================================================
Ejecutad:   python verificar.py

Comprueba que las reglas de aceptación hacen lo que se vio en clase,
reproduce las trazas de las diapositivas, y que vuestro código respeta
el presupuesto y es reproducible con la semilla.
Todo debe salir en verde (OK) antes de empezar los experimentos.
"""

import random
import sys
import traceback

import algoritmos
import configuracion
from bloques import Problema, ejecutar

OK, FALLO = "  OK  ", " FALLO"
fallos = 0


def comprobar(nombre, cond, detalle=""):
    global fallos
    print(f"[{OK if cond else FALLO}] {nombre}" + ("" if cond else f"   -> {detalle}"))
    if not cond:
        fallos += 1


def seguro(nombre, funcion):
    try:
        funcion()
    except NotImplementedError as e:
        comprobar(nombre, False, f"sin implementar ({e})")
    except Exception as e:  # noqa
        comprobar(nombre, False, f"excepción: {type(e).__name__}: {e}")
        traceback.print_exc(limit=1)


def params(alg, **sobrescribe):
    p = dict(configuracion.configurar(alg, 8, 4, 20000))
    p.update(sobrescribe)
    return p


def regla(alg, **sobrescribe):
    prob = Problema("deceptive", 8, 4, 20000)
    return algoritmos.REGLAS[alg](params(alg, **sobrescribe), prob, random.Random(0))


print("\n=== 1. Escalador ===")
def t_escalador():
    r = regla("escalador", aceptar_iguales=True)
    comprobar("acepta si mejora (5 -> 6)", r.aceptar(5, 6, 6) is True)
    comprobar("rechaza si empeora (6 -> 5)", r.aceptar(6, 5, 6) is False)
    comprobar("aceptar_iguales=True acepta 5 -> 5", r.aceptar(5, 5, 6) is True)
    r = regla("escalador", aceptar_iguales=False)
    comprobar("aceptar_iguales=False rechaza 5 -> 5", r.aceptar(5, 5, 6) is False)
seguro("escalador", t_escalador)

print("\n=== 2. Recocido simulado (T = 2) ===")
def t_recocido():
    r = regla("recocido", T0=2.0)
    comprobar("acepta siempre si mejora (6 -> 7)", all(r.aceptar(6, 7, 7) for _ in range(200)))
    comprobar("acepta siempre si es igual (6 -> 6)", all(r.aceptar(6, 6, 6) for _ in range(200)))
    N = 20000
    frec = sum(r.aceptar(6, 5, 6) for _ in range(N)) / N
    comprobar(f"P(aceptar 6 -> 5) ≈ e^(-1/2) = 0,61   (medido: {frec:.3f})", abs(frec - 0.6065) < 0.02,
              "revisa el signo de Δ o la fórmula")
    frec4 = sum(r.aceptar(7, 3, 7) for _ in range(N)) / N
    comprobar(f"P(aceptar 7 -> 3) ≈ e^(-4/2) = 0,14   (medido: {frec4:.3f})", abs(frec4 - 0.1353) < 0.02)
seguro("recocido", t_recocido)

print("\n=== 3. Umbral (TA), traza de clase con U = 3 ===")
def t_umbral():
    r = regla("umbral", U0=3)
    traza = [(6, 5, True), (5, 4, True), (4, 3, True), (3, 7, True)]
    comprobar("0000 0000 -> 1111 0000: 6 -> 5 -> 4 -> 3 -> 7",
              all(r.aceptar(a, b, 7) is acc for a, b, acc in traza))
    comprobar("rechaza empeorar más de U (6 -> 2)", r.aceptar(6, 2, 6) is False)
    comprobar("acepta empeorar exactamente U (6 -> 3)", r.aceptar(6, 3, 6) is True)
seguro("umbral", t_umbral)

print("\n=== 4. RRT, traza de clase con RÉCORD = 6, D = 3 ===")
def t_rrt():
    r = regla("rrt", D=3)
    comprobar("acepta 5, 4 y 3 con récord 6",
              r.aceptar(6, 5, 6) and r.aceptar(5, 4, 6) and r.aceptar(4, 3, 6))
    comprobar("rechaza 2 con récord 6 (2 < 6 - 3)", r.aceptar(3, 2, 6) is False)
    comprobar("compara con el RÉCORD, no con el actual (3 -> 3, récord 7)", r.aceptar(3, 3, 7) is False)
    r = regla("rrt", D=2)
    comprobar("con D = 2 rechaza el paso a 3 (récord 6)", r.aceptar(4, 3, 6) is False)
seguro("rrt", t_rrt)

print("\n=== 5. Gran Diluvio, traza de clase con NIVEL = 2,5 ===")
def t_diluvio():
    r = regla("diluvio", L0=2.5, lluvia=0.0)
    comprobar("acepta 5, 4, 3 y 7 con nivel 2,5",
              r.aceptar(6, 5, 6) and r.aceptar(5, 4, 6) and r.aceptar(4, 3, 6) and r.aceptar(3, 7, 6))
    comprobar("rechaza 2 (por debajo del nivel)", r.aceptar(3, 2, 6) is False)
    r = regla("diluvio", L0=2.5, lluvia=1.0)
    r.tras_iteracion(True)
    comprobar("tras una iteración con lluvia 1 el nivel es 3,5: rechaza 3", r.aceptar(4, 3, 6) is False)
seguro("diluvio", t_diluvio)

print("\n=== 6. Ejecuciones completas: presupuesto y reproducibilidad ===")
def t_ejecuciones():
    for alg in ["aleatoria", "escalador", "recocido", "umbral", "rrt", "diluvio"]:
        p = configuracion.configurar(alg, 32, 4, 2000)
        prob = Problema("deceptive", 32, 4, 2000)
        random.seed(123)
        r1 = ejecutar(alg, p, prob, semilla=7)
        gastadas = prob.evaluaciones
        prob2 = Problema("deceptive", 32, 4, 2000)
        random.seed(999)  # si se usa el módulo random global, el resultado cambiará
        r2 = ejecutar(alg, p, prob2, semilla=7)
        comprobar(f"{alg:9s}: respeta el presupuesto ({gastadas} ≤ 2000)", gastadas <= 2000)
        comprobar(f"{alg:9s}: reproducible con la misma semilla",
                  (r1.mejor_f, r1.historia) == (r2.mejor_f, r2.historia),
                  "¿usáis random.xxx en vez de self.rng / rng?")
        comprobar(f"{alg:9s}: mejor_f = F(mejor_x)",
                  r1.mejor_x is not None and Problema("deceptive", 32, 4, 10).evaluar(r1.mejor_x) == r1.mejor_f)
seguro("ejecuciones", t_ejecuciones)

print("\n=== 7. configuracion.py ===")
def t_config():
    obligatorios = {"recocido": ["T0"], "umbral": ["U0"], "rrt": ["D"], "diluvio": ["L0", "lluvia"]}
    for (n, k, B) in [(32, 4, 20000), (60, 5, 30000), (64, 4, 50000)]:
        for alg, claves in obligatorios.items():
            p = configuracion.configurar(alg, n, k, B)
            comprobar(f"configurar('{alg}', n={n}, k={k}, B={B}) tiene {claves}",
                      isinstance(p, dict) and all(c in p for c in claves))
seguro("configuracion", t_config)

print("\n" + ("TODO CORRECTO: podéis empezar los experimentos." if fallos == 0
              else f"{fallos} comprobación(es) fallida(s)."))
sys.exit(1 if fallos else 0)
