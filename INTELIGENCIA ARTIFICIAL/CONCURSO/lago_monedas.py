"""
Lago con monedas · Concurso «El lago desconocido» · Inteligencia Artificial I (UCM)

Uso:
    from lago_monedas import MAPAS, crear_lago, dibujar
    entorno = crear_lago(MAPAS[0], semilla=7)   # la semilla decide salida, meta y monedas
    print(dibujar(entorno))                     # solo para practicar

- Lagos de 12 x 12 casillas. Resbalan como FrozenLake (va donde quiere 1 de cada 3 veces).
- La salida, la meta y las 2 monedas se colocan al azar sobre hielo, según la semilla,
  y no cambian durante todo el entrenamiento. La meta está como mínimo a 15 pasos de la salida.
- Estado (observación): número de casilla (0-143) + 144 si el agente ya lleva una moneda.
  Es decir, 288 estados posibles.
- Recompensa: 2 al llegar a la meta con al menos una moneda; 1 al llegar sin ninguna; 0 en otro caso.
- El episodio termina al llegar a la meta, al caer en un agujero o al dar 300 pasos.
"""

from collections import deque

import numpy as np
import gymnasium as gym
from gymnasium import spaces

# Los cinco lagos del concurso (F = hielo, H = agujero)
MAPAS = [
    ["FFFFFHFFHFFF",
     "FFFFFFHFHFFF",
     "FFFHHFFFFHFF",
     "FFHFFFFFHFFF",
     "FHFFFFHFFFFF",
     "FHFFFFFFHFHF",
     "FFFFFHFFFHHF",
     "FFFFFFFHFFFF",
     "FHFFFFFHFFFF",
     "HFFFFHFFFFFF",
     "FFFFFFFFFFHF",
     "FFHHFFHFFFFF"],   # lago 1
    ["FFFFFFFFFFFF",
     "FFHFHFHFFHFH",
     "FHFFFFFHFFFF",
     "FFFFFFFFFFFF",
     "FFFFHFFFFFFF",
     "HFFFHHHFFFFF",
     "FFFFFFFFFFFF",
     "FFFFFFFFFFFF",
     "FFFFFHFFFHFH",
     "FHFFFHHFFFFF",
     "FFFFFHFHFFFF",
     "FFHFFFFFFFFF"],   # lago 2
    ["FFFHFFFFFFHF",
     "FFFFHFFFFFFH",
     "FFFFFFFFFFFF",
     "FFFHFFFFFFFH",
     "FFFFFFFFHFFF",
     "FFFFFFFFFFFF",
     "HHFFFFHFFFFH",
     "FHFHFFHFFFFH",
     "HFFFFFFFFFFF",
     "FFFFFHFHFFFF",
     "FFFFFFFFHFFF",
     "FHFHHFHFFHHF"],   # lago 3
    ["FFFFFFFHFFFF",
     "FFFHFFFFHFHF",
     "HHFFFFHFFFFF",
     "FFFFFFFHFFFF",
     "FFHFHFFFFFFF",
     "FFFFFFFFFFFF",
     "FFFFFFFFFFFF",
     "FFFFFFFFFFFF",
     "FFFFHFFHFHHF",
     "FFFFFFFHHFFH",
     "FHFFHFFFFFFH",
     "FFFFHFHFFFFF"],   # lago 4
    ["FFFFFHHFHHFF",
     "HFFFHFFFFFFF",
     "FFFFHFHHFHFF",
     "FHFFFFFFFFFF",
     "FFFFFFFFHFFH",
     "FFFFFFFFHFFF",
     "FFFFFFFFFFFF",
     "FHHFHFHFFFFF",
     "FHFFFFHFFFHF",
     "FFFFFFFFFFFF",
     "HFFFFFFFFFFF",
     "FFHFFHFFHFHF"],   # lago 5
]

TAMANO = 12
MAX_PASOS = 300
DISTANCIA_MINIMA = 15      # pasos mínimos (sin resbalar) entre salida y meta


def _vecinos(f, c):
    for df, dc in ((0, -1), (1, 0), (0, 1), (-1, 0)):
        nf, nc = f + df, c + dc
        if 0 <= nf < TAMANO and 0 <= nc < TAMANO:
            yield nf, nc


def _distancias(mapa, origen):
    """Distancia en pasos (sin resbalar) desde `origen` a cada casilla de hielo alcanzable."""
    dist = {origen: 0}
    cola = deque([origen])
    while cola:
        f, c = cola.popleft()
        for nf, nc in _vecinos(f, c):
            if mapa[nf][nc] != "H" and (nf, nc) not in dist:
                dist[(nf, nc)] = dist[(f, c)] + 1
                cola.append((nf, nc))
    return dist


def colocar(mapa, semilla):
    """Elige salida, meta y dos monedas sobre hielo, con camino entre todas ellas."""
    rng = np.random.default_rng(semilla)
    hielo = [(f, c) for f in range(TAMANO) for c in range(TAMANO) if mapa[f][c] == "F"]
    for _ in range(10_000):
        salida = hielo[rng.integers(len(hielo))]
        dist = _distancias(mapa, salida)
        lejanas = [p for p in dist if dist[p] >= DISTANCIA_MINIMA]
        if not lejanas:
            continue
        meta = lejanas[rng.integers(len(lejanas))]
        libres = [p for p in dist if p not in (salida, meta) and dist[p] >= 2
                  and abs(p[0] - meta[0]) + abs(p[1] - meta[1]) >= 2]
        if len(libres) < 2:
            continue
        i, j = rng.choice(len(libres), size=2, replace=False)
        return salida, meta, (libres[i], libres[j])
    raise ValueError("No he podido colocar salida, meta y monedas en este mapa")


class LagoConMonedas(gym.Wrapper):
    def __init__(self, mapa, semilla):
        self.salida, self.meta, self.monedas = colocar(mapa, semilla)
        desc = [list(fila) for fila in mapa]
        desc[self.salida[0]][self.salida[1]] = "S"
        desc[self.meta[0]][self.meta[1]] = "G"
        base = gym.make("FrozenLake-v1", desc=["".join(f) for f in desc], is_slippery=True,
                        max_episode_steps=MAX_PASOS)
        super().__init__(base)
        self.casillas = TAMANO * TAMANO
        self.casillas_moneda = {f * TAMANO + c for f, c in self.monedas}
        self.observation_space = spaces.Discrete(2 * self.casillas)
        self.tiene_moneda = 0

    def reset(self, *, seed=None, options=None):
        casilla, info = self.env.reset(seed=seed, options=options)
        self.tiene_moneda = 0
        return casilla, info

    def step(self, accion):
        casilla, r, terminado, truncado, info = self.env.step(accion)
        if casilla in self.casillas_moneda:
            self.tiene_moneda = 1
        if r > 0:
            r = 2.0 if self.tiene_moneda else 1.0
        return casilla + self.casillas * self.tiene_moneda, r, terminado, truncado, info


def crear_lago(mapa, semilla):
    """Crea el lago `mapa` con salida, meta y monedas colocadas según `semilla`."""
    return LagoConMonedas(mapa, semilla)


def dibujar(entorno):
    """Devuelve el lago como texto: S salida, G meta, $ moneda, H agujero, F hielo."""
    filas = []
    for f in range(TAMANO):
        fila = ""
        for c in range(TAMANO):
            if (f, c) == entorno.salida: fila += "S"
            elif (f, c) == entorno.meta: fila += "G"
            elif (f, c) in entorno.monedas: fila += "$"
            else: fila += entorno.env.unwrapped.desc[f][c].decode()
        filas.append(" ".join(fila))
    return "\n".join(filas)
