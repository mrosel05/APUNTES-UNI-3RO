"""
algoritmos.py — VUESTRO fichero: las reglas de aceptación
=========================================================
Inteligencia Artificial I · Práctica 3 · Optimización

Grupo: ____    Integrantes: ______________________ / ______________________

El esqueleto común (vecino -> evaluar -> ¿aceptar?) ya está en bloques.py.
Aquí solo tenéis que programar:

  1. busqueda_aleatoria()   — completa (no usa vecinos)
  2. Cinco reglas de aceptación: Escalador, Recocido, Umbral, RRT, GranDiluvio

Cada regla es una clase con tres métodos:

  __init__(self, parametros, problema, rng)
      Lee los parámetros del diccionario y prepara el estado interno.
      Podéis consultar problema.n, problema.k y problema.presupuesto.

  aceptar(self, f_actual, f_vecino, f_mejor) -> bool
      True si se acepta el vecino. (Se MAXIMIZA F.)
      f_mejor es el mejor valor encontrado hasta ahora (el "récord").

  tras_iteracion(self, aceptado)
      Se llama después de cada iteración: aquí se actualiza T, U, NIVEL...

Parámetros OBLIGATORIOS (verificar.py los usa con estos nombres):
  escalador : ninguno                 (opcional: "aceptar_iguales", por defecto True)
  recocido  : "T0"                    (temperatura inicial)
  umbral    : "U0"                    (umbral inicial)
  rrt       : "D"                     (desviación permitida respecto al récord)
  diluvio   : "L0", "lluvia"          (nivel inicial y subida por iteración)

Podéis añadir todos los parámetros extra que queráis (alfa, esquema de
enfriamiento, ritmo de bajada del umbral, etc.). Elegirlos y justificarlos
es parte de la práctica.

Reglas:
  - Usad SIEMPRE self.rng para el azar (nunca el módulo random directamente):
    si no, las ejecuciones no son reproducibles con la semilla.
  - No llaméis a problema.evaluar() desde las reglas: el esqueleto ya evalúa.
"""

import math

from bloques import PresupuestoAgotado, Resultado


# ======================================================================
# 1. BÚSQUEDA ALEATORIA (completa)
# ======================================================================
def busqueda_aleatoria(problema, parametros, rng):
    """
    Genera cadenas aleatorias hasta agotar el presupuesto (o encontrar el
    óptimo) y devuelve un Resultado con la mejor.
    Pistas: problema.aleatoria(rng), problema.evaluar(x),
            res.registrar(x, fx, problema), y capturar PresupuestoAgotado.
    """
    
    res = Resultado()
    try:
        x = problema.aleatoria(rng)
        fx = problema.evaluar(x)
        res.registrar(x, fx, problema)
        while res.mejor_f < problema.optimo:
            y = problema.vecino(x, rng)
            fy = problema.evaluar(y)
            x, fx = y, fy
            res.registrar(x, fx, problema)
    except PresupuestoAgotado:
        pass
    return res



# ======================================================================
# 2. REGLAS DE ACEPTACIÓN
# ======================================================================
class Escalador:
    """Acepta si el vecino no empeora (o si mejora estrictamente)."""

    def __init__(self, parametros, problema, rng):
        self.rng = rng
        self.aceptar_iguales = parametros.get("aceptar_iguales", True)

    def aceptar(self, f_actual, f_vecino, f_mejor):
        return f_vecino > f_actual or (f_vecino == f_actual and self.aceptar_iguales)

    def tras_iteracion(self, aceptado):
        pass


class Recocido:
    """Acepta si mejora; si empeora, con probabilidad exp(-|Δ| / T)."""

    def __init__(self, parametros, problema, rng):
        self.rng = rng
        self.T = parametros["T0"]
        
        # TODO: leer el resto de parámetros que decidáis (alfa, esquema...)

    def aceptar(self, f_actual, f_vecino, f_mejor):
        return (f_vecino > f_actual or
            self.rng.random() < math.exp((f_vecino - f_actual)/self.T))

    def tras_iteracion(self, aceptado):
        # TODO: actualizar la temperatura
        pass


class Umbral:
    """Threshold Accepting: acepta si F(s') >= F(s) - U. U baja con el tiempo."""

    def __init__(self, parametros, problema, rng):
        self.rng = rng
        self.U = parametros["U0"]
        # TODO: leer el resto de parámetros (cómo y a qué ritmo baja U)

    def aceptar(self, f_actual, f_vecino, f_mejor):
        return f_vecino >= f_actual - self.U

    def tras_iteracion(self, aceptado):
        # TODO: actualizar el umbral
        pass


class RRT:
    """Record-to-Record Travel: acepta si F(s') >= RÉCORD - D."""

    def __init__(self, parametros, problema, rng):
        self.rng = rng
        self.D = parametros["D"]

    def aceptar(self, f_actual, f_vecino, f_mejor):
        return f_vecino >= f_mejor - self.D

    def tras_iteracion(self, aceptado):
        pass


class GranDiluvio:
    """Great Deluge: acepta si F(s') >= NIVEL. El NIVEL sube con la LLUVIA."""

    def __init__(self, parametros, problema, rng):
        self.rng = rng
        self.nivel = parametros["L0"]
        self.lluvia = parametros["lluvia"]

    def aceptar(self, f_actual, f_vecino, f_mejor):
        return f_vecino >= self.nivel

    def tras_iteracion(self, aceptado):
        if(aceptado):
            self.nivel += self.lluvia


# No tocar: lo usa bloques.ejecutar()
REGLAS = {
    "escalador": Escalador,
    "recocido": Recocido,
    "umbral": Umbral,
    "rrt": RRT,
    "diluvio": GranDiluvio,
}
