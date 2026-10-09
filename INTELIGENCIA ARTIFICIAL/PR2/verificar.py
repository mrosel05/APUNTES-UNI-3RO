"""
Laboratorio de búsqueda informada — Inteligencia Artificial I
Herramienta de apoyo: carga vuestro mapa, comprueba si vuestra heurística es
admisible y consistente, y mide cuánto trabajo le ahorra a A*.

Uso:   python verificar.py mapa_01.csv Andrada Noviella

Para probar VUESTRA heurística, editad la función mi_heuristica() del final.
"""
import csv, sys, heapq
from collections import deque


# ─────────────────────────── carga del mapa ───────────────────────────
def cargar(ruta):
    """Devuelve G[a][b] = minutos  y  KM[a][b] = kilómetros."""
    G, KM, VIA = {}, {}, {}
    with open(ruta, encoding='utf-8') as f:
        for r in csv.DictReader(f):
            a, b = r['origen'], r['destino']
            mins, km = int(r['minutos']), int(r['km'])
            G.setdefault(a, {})[b] = mins
            G.setdefault(b, {})[a] = mins
            KM.setdefault(a, {})[b] = km
            KM.setdefault(b, {})[a] = km
            VIA.setdefault(a, {})[b] = r['tipo_via']
            VIA.setdefault(b, {})[a] = r['tipo_via']
    return G, KM, VIA


# ─────────────────── el oráculo: h*(n) para todo n ────────────────────
def dijkstra(G, origen):
    """Coste mínimo de 'origen' a cada ciudad. Como las conexiones son de doble
    sentido, lanzarlo desde el DESTINO da directamente h*(n) de todas."""
    dist = {origen: 0}
    cola = [(0, origen)]
    while cola:
        d, u = heapq.heappop(cola)
        if d > dist.get(u, float('inf')):
            continue
        for v, c in G[u].items():
            if d + c < dist.get(v, float('inf')):
                dist[v] = d + c
                heapq.heappush(cola, (d + c, v))
    return dist


# ─────────────────────────── A* instrumentado ─────────────────────────
def astar(G, origen, destino, h):
    orden = 0
    frontera = [(h(origen), 0, orden, origen)]
    mejor = {origen: 0}
    padre = {origen: None}
    cerrados = set()
    expandidos, generados = 0, 1

    while frontera:
        f, g, _, u = heapq.heappop(frontera)
        if u in cerrados:
            continue
        cerrados.add(u)
        expandidos += 1
        if u == destino:
            camino, c = [], u
            while c:
                camino.append(c)
                c = padre[c]
            return {'coste': g, 'camino': camino[::-1],
                    'expandidos': expandidos, 'generados': generados}
        for v, c in G[u].items():
            if g + c < mejor.get(v, float('inf')):
                mejor[v] = g + c
                padre[v] = u
                orden += 1
                generados += 1
                heapq.heappush(frontera, (g + c + h(v), g + c, orden, v))
    return None


def ramificacion_efectiva(N, d):
    """Resuelve N + 1 = 1 + b + b^2 + ... + b^d por bisección."""
    if d == 0:
        return float('nan')
    lo, hi = 1.0000001, 10.0
    for _ in range(200):
        m = (lo + hi) / 2
        if sum(m ** i for i in range(d + 1)) < N + 1:
            lo = m
        else:
            hi = m
    return (lo + hi) / 2


# ──────────────────────── comprobaciones de calidad ───────────────────
def comprobar(G, hstar, h, destino):
    fallos_adm, fallos_con = [], []
    for n in G:
        if h(n) < 0:
            fallos_adm.append((n, h(n), hstar[n], 'negativa'))
        elif h(n) > hstar[n]:
            fallos_adm.append((n, h(n), hstar[n], 'sobreestima'))
    if h(destino) != 0:
        fallos_adm.append((destino, h(destino), 0, 'h(objetivo) != 0'))
    for u in G:
        for v, c in G[u].items():
            if h(u) > c + h(v) + 1e-9:
                fallos_con.append((u, v, h(u), c, h(v)))
    return fallos_adm, fallos_con


def informe(G, origen, destino, h, nombre):
    hstar = dijkstra(G, destino)
    adm, con = comprobar(G, hstar, h, destino)

    print(f"\n{'='*64}\n  {nombre}\n{'='*64}")
    if adm:
        print(f"  ADMISIBLE: NO — falla en {len(adm)} ciudades. Ejemplos:")
        for n, hv, hs, por in adm[:5]:
            print(f"     {n}: h={hv:.0f} pero h*={hs:.0f}   ({por})")
        print("  Con una heurística no admisible A* puede devolver un camino peor.")
    else:
        print("  ADMISIBLE: sí — h(n) <= h*(n) en las", len(G), "ciudades.")
        print(f"  CONSISTENTE: {'sí' if not con else 'NO'}", end='')
        if con:
            u, v, hu, c, hv = con[0]
            print(f" — ejemplo: h({u})={hu:.0f} > {c} + h({v})={c+hv:.0f}")
        else:
            print()

    r = astar(G, origen, destino, h)
    if not r:
        print("  A* no encuentra camino."); return
    d = len(r['camino']) - 1
    prec = [h(n) / hstar[n] for n in G if hstar[n] > 0]
    print(f"  Camino: {' -> '.join(r['camino'])}")
    print(f"  Coste: {r['coste']} min   ({d} tramos)")
    print(f"  Nodos expandidos: {r['expandidos']}   generados: {r['generados']}")
    print(f"  b* = {ramificacion_efectiva(r['generados'], d):.3f}")
    print(f"  Precision media h/h* = {sum(prec)/len(prec):.2f}")
    return r


# ══════════════════════════════════════════════════════════════════════
#  AQUÍ ESCRIBÍS VOSOTROS
# ══════════════════════════════════════════════════════════════════════
def mi_heuristica(G, KM, VIA, destino):
    """Devolved una función h(ciudad) -> minutos estimados hasta 'destino'.

    Reglas:
      · Nunca puede sobreestimar:  h(n) <= h*(n)
      · h(destino) tiene que ser 0
      · No vale usar dijkstra(G, destino): eso es el oráculo, no una heurística

    Esta de ejemplo devuelve 0 (A* se convierte en coste uniforme).
    """
    return lambda n: 0


if __name__ == '__main__':
    ruta = sys.argv[1] if len(sys.argv) > 1 else 'mapa_06.csv'
    G, KM, VIA = cargar(ruta)
    origen = sys.argv[2] if len(sys.argv) > 2 else 'Tebares'
    destino = sys.argv[3] if len(sys.argv) > 3 else 'Zaraiba'
    if not origen or not destino:
        print("Uso: python verificar.py mapa_XX.csv ORIGEN DESTINO")
        print("Ciudades:", ', '.join(sorted(G)))
        sys.exit(1)

    hstar = dijkstra(G, destino)
    informe(G, origen, destino, lambda n: 0, "h = 0   (es coste uniforme: vuestra linea base)")
    informe(G, origen, destino, mi_heuristica(G, KM, VIA, destino), "VUESTRA HEURISTICA")
    informe(G, origen, destino, lambda n: hstar[n], "h = h*  (el oraculo: el limite que nadie puede batir)")
