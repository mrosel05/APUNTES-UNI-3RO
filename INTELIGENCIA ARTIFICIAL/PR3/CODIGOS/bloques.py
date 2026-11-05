"""
bloques.py — Problema de los bloques y armazón común (NO MODIFICAR)
====================================================================
Inteligencia Artificial I · Práctica 3 · Optimización

Este fichero lo da el profesor y es el mismo para todos los grupos.
El concurso final se ejecuta con ESTA versión, así que cualquier cambio
que hagáis aquí no tendrá efecto en la evaluación.

Contenido
---------
- Problema:           una instancia del problema de los bloques.
- PresupuestoAgotado: excepción que se lanza al gastar todas las evaluaciones.
- busqueda_trayectoria(): el ESQUELETO COMÚN visto en clase
      vecino -> evaluar -> ¿aceptar? -> actualizar la mejor
  Solo cambia la regla de aceptación, que la programáis vosotros.
- ejecutar():         una ejecución completa de un algoritmo con una semilla.
"""

import random

FUNCIONES = ("onemax", "plateau", "royalroad", "deceptive")


# ----------------------------------------------------------------------
# Funciones por bloque f(u), con u = número de unos del bloque (0..k)
# Para k = 4 coinciden con las de clase:
#   OneMax     0 1 2 3 4
#   Plateau    0 0 0 3 4
#   Royal Road 0 0 0 0 4
#   Deceptive  3 2 1 0 4
# ----------------------------------------------------------------------
def tabla_f(funcion, k):
    """Devuelve la lista [f(0), f(1), ..., f(k)] para bloques de k bits."""
    if funcion == "onemax":
        return [u for u in range(k + 1)]
    if funcion == "plateau":
        return [u if u >= k - 1 else 0 for u in range(k + 1)]
    if funcion == "royalroad":
        return [k if u == k else 0 for u in range(k + 1)]
    if funcion == "deceptive":
        return [k - 1 - u if u < k else k for u in range(k + 1)]
    raise ValueError(f"Función desconocida: {funcion}. Usa una de {FUNCIONES}")


class PresupuestoAgotado(Exception):
    """Se lanza cuando se intenta evaluar más veces de las permitidas."""


class Problema:
    """
    Instancia del problema de los bloques.

    Parámetros
    ----------
    funcion     : "onemax" | "plateau" | "royalroad" | "deceptive"
    n           : longitud de la cadena (múltiplo de k)
    k           : bits por bloque (en clase, k = 4)
    presupuesto : número máximo de evaluaciones de F

    Atributos útiles (solo lectura)
    -------------------------------
    optimo         : valor de F en el óptimo global (= n)
    evaluaciones   : evaluaciones gastadas hasta ahora
    """

    def __init__(self, funcion, n, k=4, presupuesto=20000):
        if n % k != 0:
            raise ValueError("n debe ser múltiplo de k")
        self.funcion = funcion
        self.n = n
        self.k = k
        self.presupuesto = presupuesto
        self.f = tabla_f(funcion, k)
        self.optimo = n  # todo unos: n/k bloques que valen k
        self.evaluaciones = 0

    # -- información ----------------------------------------------------
    def quedan(self):
        """Evaluaciones que quedan por gastar."""
        return self.presupuesto - self.evaluaciones

    def __repr__(self):
        return (f"Problema({self.funcion}, n={self.n}, k={self.k}, "
                f"presupuesto={self.presupuesto})")

    # -- evaluación (la ÚNICA forma de conocer F) -----------------------
    def evaluar(self, x):
        """Devuelve F(x) y gasta UNA evaluación del presupuesto."""
        if self.evaluaciones >= self.presupuesto:
            raise PresupuestoAgotado()
        self.evaluaciones += 1
        k, f = self.k, self.f
        return sum(f[sum(x[i:i + k])] for i in range(0, self.n, k))

    # -- generación de soluciones (no gastan evaluaciones) ---------------
    def aleatoria(self, rng):
        """Cadena aleatoria de n bits."""
        return [rng.randint(0, 1) for _ in range(self.n)]

    def vecino(self, x, rng):
        """Vecino de x: copia de x con UN bit invertido al azar."""
        y = x[:]
        i = rng.randrange(self.n)
        y[i] ^= 1
        return y


class Resultado:
    """Lo que devuelve una ejecución."""

    def __init__(self):
        self.mejor_x = None
        self.mejor_f = float("-inf")
        self.evals_hasta_optimo = None   # None si no alcanzó el óptimo
        self.historia = []               # lista de (evaluación, mejor F) cada vez que mejora

    def registrar(self, x, fx, problema):
        if fx > self.mejor_f:
            self.mejor_x, self.mejor_f = x[:], fx
            self.historia.append((problema.evaluaciones, fx))
            if fx == problema.optimo and self.evals_hasta_optimo is None:
                self.evals_hasta_optimo = problema.evaluaciones

    def __repr__(self):
        return (f"Resultado(mejor_f={self.mejor_f}, "
                f"evals_hasta_optimo={self.evals_hasta_optimo})")


# ----------------------------------------------------------------------
# ESQUELETO COMÚN de los algoritmos de trayectoria (clase, diapositiva 10)
# ----------------------------------------------------------------------
def busqueda_trayectoria(problema, regla, rng, x0=None):
    """
    s <- inicial; repetir: s' <- vecino(s); evaluar; si regla.aceptar(...): s <- s'
    'regla' es un objeto con los métodos aceptar() y tras_iteracion()
    (ver algoritmos.py). Termina al agotar el presupuesto o al llegar al óptimo.
    """
    res = Resultado()
    try:
        x = problema.aleatoria(rng) if x0 is None else x0[:]
        fx = problema.evaluar(x)
        res.registrar(x, fx, problema)
        while res.mejor_f < problema.optimo:
            y = problema.vecino(x, rng)
            fy = problema.evaluar(y)
            aceptado = regla.aceptar(fx, fy, res.mejor_f)
            if aceptado:
                x, fx = y, fy
            res.registrar(x, fx, problema)
            regla.tras_iteracion(aceptado)
    except PresupuestoAgotado:
        pass
    return res


# ----------------------------------------------------------------------
# Una ejecución completa
# ----------------------------------------------------------------------
def ejecutar(algoritmo, parametros, problema, semilla):
    """
    Ejecuta UNA vez 'algoritmo' con 'parametros' sobre 'problema' (que debe
    estar recién creado) usando la semilla dada. Devuelve un Resultado.

    algoritmo  : "aleatoria" | "escalador" | "recocido" | "umbral" | "rrt" | "diluvio"
    parametros : diccionario con los parámetros de ese algoritmo
    """
    import algoritmos  # vuestro fichero
    if problema.evaluaciones != 0:
        raise RuntimeError("Crea un Problema nuevo para cada ejecución.")
    rng = random.Random(semilla)
    if algoritmo == "aleatoria":
        return algoritmos.busqueda_aleatoria(problema, parametros, rng)
    clase = algoritmos.REGLAS[algoritmo]
    regla = clase(parametros, problema, rng)
    return busqueda_trayectoria(problema, regla, rng)
