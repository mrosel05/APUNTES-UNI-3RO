"""
Dibuja las funciones de pertenencia de un sistema y su superficie de control
(salida frente a las dos entradas). Se da hecho: solo hay que importar el sistema.

    python superficie.py emparejador     # usa SISTEMA de emparejador.py

Genera pertenencia_<modulo>.png y superficie_<modulo>.png.
"""
import sys
import importlib
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from difusa import triangular, evaluar


def dibujar_pertenencia(sistema, nombre):
    variables = {**sistema["entradas"], **sistema["salidas"]}
    fig, ejes = plt.subplots(1, len(variables), figsize=(4 * len(variables), 3))
    for eje, (nom, var) in zip(np.atleast_1d(ejes), variables.items()):
        lo, hi = var["universo"]
        xs = np.linspace(lo, hi, 400)
        for etq, abc in var["etiquetas"].items():
            eje.plot(xs, [triangular(x, *abc) for x in xs], label=etq)
        eje.set_title(nom); eje.set_ylim(0, 1.05); eje.legend(fontsize=8)
    fig.tight_layout()
    fig.savefig(f"pertenencia_{nombre}.png", dpi=120)


def dibujar_superficie(sistema, nombre, n=41):
    (e1, v1), (e2, v2) = list(sistema["entradas"].items())[:2]
    (s, vs), = list(sistema["salidas"].items())[:1]
    xs = np.linspace(*v1["universo"], n)
    ys = np.linspace(*v2["universo"], n)
    Z = np.array([[evaluar(sistema, {e1: x, e2: y}, paso=(vs["universo"][1] - vs["universo"][0]) / 200)[s]
                   for x in xs] for y in ys])
    fig = plt.figure(figsize=(6, 4.5))
    ax = fig.add_subplot(projection="3d")
    X, Y = np.meshgrid(xs, ys)
    ax.plot_surface(X, Y, Z, cmap="viridis")
    ax.set_xlabel(e1); ax.set_ylabel(e2); ax.set_zlabel(s)
    fig.tight_layout()
    fig.savefig(f"superficie_{nombre}.png", dpi=120)


if __name__ == "__main__":
    modulo = sys.argv[1] if len(sys.argv) > 1 else "emparejador"
    sistema = importlib.import_module(modulo).SISTEMA
    dibujar_pertenencia(sistema, modulo)
    dibujar_superficie(sistema, modulo)
    print(f"Generados pertenencia_{modulo}.png y superficie_{modulo}.png")
