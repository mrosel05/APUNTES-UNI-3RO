"""
Comprobaciones de la Práctica 5. Ejecutar:   python verificar.py
Cada comprobación usa los ejemplos de las transparencias del Tema 5 o del
material de la práctica. Un test que falla imprime lo esperado y lo obtenido.
"""
import traceback

RESULTADOS = []


def test(nombre, obtenido_fn, esperado, tol=1e-3):
    try:
        obtenido = obtenido_fn()
        if isinstance(esperado, dict):
            ok = all(abs(obtenido.get(k, 0.0) - v) <= tol for k, v in esperado.items())
            obtenido = {k: round(obtenido.get(k, 0.0), 3) for k in esperado}
        elif isinstance(esperado, (int, float)):
            ok = abs(obtenido - esperado) <= tol
            obtenido = round(obtenido, 3)
        else:
            ok = obtenido == esperado
    except NotImplementedError:
        ok, obtenido = False, "sin implementar"
    except Exception as e:
        ok, obtenido = False, f"ERROR {type(e).__name__}: {e}"
    RESULTADOS.append(ok)
    print(f"[{'OK' if ok else 'FALLA'}] {nombre}" + ("" if ok else f"\n        esperado {esperado}, obtenido {obtenido}"))


print("=== Parte A · Reglas con factores de certeza ===")
import fc
test("A1 conjunción, transp. 64: min(0.5, 0.7)", lambda: fc.fc_conjuncion([0.5, 0.7]), 0.5)
test("A1 disyunción, transp. 65: max(0.5, 0.7)", lambda: fc.fc_disyuncion([0.5, 0.7]), 0.7)
test("A1 negación: FC(¬x) = -FC(x)", lambda: fc.fc_negacion(0.4), -0.4)
test("A2 propagación, transp. 68: 0.3 · 0.7", lambda: fc.fc_regla(0.3, 0.7), 0.21)
test("A2 propagación con premisa ≤ 0 → 0", lambda: fc.fc_regla(-0.2, 0.7), 0.0)
test("A3 combinar dos positivos: 0.15, 0.18", lambda: fc.combinar(0.15, 0.18), 0.303)
test("A3 combinar dos negativos: -0.3, -0.4", lambda: fc.combinar(-0.3, -0.4), -0.58)
test("A3 combinar signos distintos: 0.6, -0.4", lambda: fc.combinar(0.6, -0.4), 0.3333)

R71 = [{"si": ["A", "B", "C"], "entonces": "H", "fc": 0.5, "nombre": "R1"},
       {"si": ["D", "F"], "entonces": "H", "fc": 0.9, "nombre": "R2"}]
H71 = {"A": 1, "B": 0.5, "C": 0.3, "D": 0.7, "F": 0.2}
test("A4 encadenar, transp. 71: FC(H)", lambda: fc.encadenar_adelante(H71, R71)["H"], 0.303)
R75 = [{"si": ["A"], "entonces": "B", "fc": -0.6, "nombre": "R1"},
       {"si": ["no B"], "entonces": "C", "fc": -0.9, "nombre": "R2"}]
test("A4 encadenar con negación, transp. 75: FC(C)", lambda: fc.encadenar_adelante({"A": 0.8}, R75)["C"], -0.432)
test("A4 encadenar no modifica los hechos de entrada",
     lambda: (fc.encadenar_adelante(H71, R71), "H" not in H71)[1], True)

import base_matchmaking as bm
test("A5 emparejador, jugador 1 (en racha): contra_mejores y partida_pareja",
     lambda: fc.encadenar_adelante(bm.CASOS["jugador_1_en_racha"], bm.REGLAS), {"contra_mejores": 0.448, "partida_pareja": 0.350})
test("A5 emparejador, jugador 2 (frustrado): contra_peores (combina signos distintos)",
     lambda: fc.encadenar_adelante(bm.CASOS["jugador_2_frustrado"], bm.REGLAS), {"contra_peores": 0.179})
test("A5 emparejador, jugador 3 (diamante esperando): las tres hipótesis",
     lambda: fc.encadenar_adelante(bm.CASOS["jugador_3_diamante_esperando"], bm.REGLAS),
     {"partida_pareja": 0.747, "contra_peores": 0.360, "contra_mejores": 0.100})

print("\n=== Parte B · Lógica difusa ===")
import difusa as df
test("B1 triangular en el pico", lambda: df.triangular(150, 140, 150, 160), 1.0)
test("B1 triangular en la rampa", lambda: df.triangular(157, 155, 167.5, 180), 0.16)
test("B1 triangular fuera del soporte", lambda: df.triangular(135, 140, 150, 160), 0.0)
test("B1 triangular con hombro (a = b) vale 1 en el extremo", lambda: df.triangular(10, 10, 10, 22), 1.0)
test("B2 fuzzificar Laura, 179 cm (transp. 113)", lambda: df.fuzzificar(179, df.ALTURA),
     {"pequeño": 0.0, "mediano": 0.08, "alto": 0.32})
test("B3 inferir mín-máx (dos reglas sobre la misma etiqueta)",
     lambda: df.inferir([{"si": {"t": "alta", "h": "seca"}, "entonces": ("v", "media")},
                         {"si": {"t": "media"}, "entonces": ("v", "media")},
                         {"si": {"t": "alta", "h": "humeda"}, "entonces": ("v", "alta")}],
                        {"t": {"alta": 0.7, "media": 0.2}, "h": {"seca": 0.4, "humeda": 0.6}})["v"],
     {"media": 0.4, "alta": 0.6})
test("B4 centroide, transp. 120: pequeño 0.5, mediano 0.3, alto 0 → 158.1 cm",
     lambda: df.centroide(df.ALTURA, {"pequeño": 0.5, "mediano": 0.3, "alto": 0.0}), 158.1, tol=0.15)
test("B4 centroide sin ninguna etiqueta activa → centro del universo",
     lambda: df.centroide(df.ALTURA, {}), 170.0)
import emparejador as em
test("B5 ciclo completo, emparejador (ratio 50 %, ranking 1500) → ajuste 0",
     lambda: df.evaluar(em.SISTEMA, {"ratio": 50, "ranking": 1500})["ajuste"], 0.0, tol=0.5)
test("B5 ciclo completo, emparejador (ratio 80 %, ranking 2800): dos reglas activas → +159.2",
     lambda: df.evaluar(em.SISTEMA, {"ratio": 80, "ranking": 2800})["ajuste"], 159.2, tol=0.5)
test("B5 ciclo completo, emparejador (ratio 65 %, ranking 1800) → +156.8",
     lambda: df.evaluar(em.SISTEMA, {"ratio": 65, "ranking": 1800})["ajuste"], 156.8, tol=0.5)

print(f"\n{sum(RESULTADOS)} de {len(RESULTADOS)} comprobaciones superadas.")
