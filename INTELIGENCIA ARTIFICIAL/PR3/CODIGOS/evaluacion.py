import csv
import math
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from bloques import Problema, ejecutar
from configuracion import configurar


# Diseño de evaluación final. Las semillas 1001–1030 son distintas de las
# semillas 1–20 usadas en el ajuste y quedan fijadas antes de ejecutar.
FUNCIONES = ("onemax", "plateau", "royalroad", "deceptive")
ALGORITMOS = ("aleatoria", "escalador", "recocido", "umbral", "rrt", "diluvio")
INSTANCIAS = (
    {"n": 32, "k": 4, "presupuesto": 20_000},
    {"n": 64, "k": 4, "presupuesto": 40_000},
)
SEMILLAS_EVALUACION = tuple(range(1001, 1031))

CARPETA_SALIDA = Path(".")


def ejecutar_evaluacion():
    filas = []
    total = len(INSTANCIAS) * len(FUNCIONES) * len(ALGORITMOS) * len(SEMILLAS_EVALUACION)
    actual = 0

    print(f"Evaluación final: {total} ejecuciones")
    print(f"Semillas fijadas: {SEMILLAS_EVALUACION[0]}–{SEMILLAS_EVALUACION[-1]}\n")

    for instancia in INSTANCIAS:
        n = instancia["n"]
        k = instancia["k"]
        presupuesto = instancia["presupuesto"]

        for funcion in FUNCIONES:
            for algoritmo in ALGORITMOS:
                # La configuración final se obtiene de configurar.py para cada
                # algoritmo e instancia. No se ajustan parámetros en esta fase.
                parametros = configurar(algoritmo, n, k, presupuesto)

                for replica, semilla in enumerate(SEMILLAS_EVALUACION, start=1):
                    # IMPORTANTE: cada ejecución necesita un Problema nuevo,
                    # porque el objeto lleva la cuenta de evaluaciones gastadas.
                    problema = Problema(
                        funcion, n=n, k=k, presupuesto=presupuesto
                    )
                    resultado = ejecutar(
                        algoritmo, parametros, problema, semilla=semilla
                    )

                    mejor_f = resultado.mejor_f
                    optimo = problema.optimo
                    exito = (mejor_f == optimo)

                    # Comprobaciones para detectar resultados inválidos.
                    if problema.evaluaciones > presupuesto:
                        raise RuntimeError(
                            f"{algoritmo} superó el presupuesto en {funcion}, n={n}, "
                            f"semilla={semilla}."
                        )
                    if resultado.mejor_x is None:
                        raise RuntimeError(
                            f"{algoritmo} no devolvió solución en {funcion}, n={n}, "
                            f"semilla={semilla}."
                        )

                    filas.append({
                        "algoritmo": algoritmo,
                        "parametros": repr(parametros),
                        "funcion": funcion,
                        "n": n,
                        "k": k,
                        "presupuesto": presupuesto,
                        "replica": replica,
                        "semilla": semilla,
                        "evaluaciones_gastadas": problema.evaluaciones,
                        "mejor_f": mejor_f,
                        "optimo": optimo,
                        "exito": exito,
                        "evals_hasta_optimo": (
                            resultado.evals_hasta_optimo if exito else np.nan
                        ),
                    })

                    actual += 1
                    if actual % 60 == 0 or actual == total:
                        print(f"  Progreso: {actual}/{total}")

    df = pd.DataFrame(filas)
    ruta = CARPETA_SALIDA / "resultados_fase5.csv"
    df.to_csv(ruta, index=False, encoding="utf-8-sig")
    print(f"\nDatos completos guardados en: {ruta.resolve()}")
    return df


def rango_intercuartilico(serie):
    """Calcula Q3 - Q1; devuelve NaN si no hay datos válidos."""
    valores = serie.dropna()
    if valores.empty:
        return np.nan
    return float(valores.quantile(0.75) - valores.quantile(0.25))


def crear_resumen(df):
    """Calcula mediana, IQR, tasa de éxito y evaluaciones hasta el óptimo."""
    claves = ["n", "k", "presupuesto", "funcion", "algoritmo"]
    resumen = (
        df.groupby(claves, as_index=False)
        .agg(
            mediana_mejor_f=("mejor_f", "median"),
            iqr_mejor_f=("mejor_f", rango_intercuartilico),
            tasa_exito=("exito", "mean"),
            exitos=("exito", "sum"),
            replicas=("exito", "count"),
            mediana_evals_hasta_optimo=("evals_hasta_optimo", "median"),
            iqr_evals_hasta_optimo=("evals_hasta_optimo", rango_intercuartilico),
        )
    )
    resumen["tasa_exito_pct"] = 100 * resumen["tasa_exito"]
    resumen.to_csv(
        CARPETA_SALIDA / "resumen_fase5.csv", index=False, encoding="utf-8-sig"
    )
    print("\nTABLA RESUMEN (IQR = Q3 - Q1; tasa de éxito en porcentaje)\n")
    columnas = claves + [
        "mediana_mejor_f", "iqr_mejor_f", "tasa_exito_pct",
        "mediana_evals_hasta_optimo", "iqr_evals_hasta_optimo"
    ]
    print(resumen[columnas].round(3).to_string(index=False))
    print("\nResumen guardado en: resumen_fase5.csv")
    return resumen


def crear_boxplot_fitness(df, n):
    """Genera un diagrama de caja del mejor F por algoritmo y función."""
    datos = df[df["n"] == n]
    grupos = []
    etiquetas = []
    posiciones = []
    posicion = 1

    for funcion in FUNCIONES:
        for algoritmo in ALGORITMOS:
            valores = datos.loc[
                (datos["funcion"] == funcion) &
                (datos["algoritmo"] == algoritmo), "mejor_f"
            ].dropna().to_numpy()
            if len(valores):
                grupos.append(valores)
                etiquetas.append(f"{algoritmo}\n{funcion}")
                posiciones.append(posicion)
            posicion += 1
        posicion += 1

    fig, ax = plt.subplots(figsize=(19, 7))
    ax.boxplot(grupos, positions=posiciones, showfliers=False)
    ax.set_title(f"Fase 5: mejor valor de F (n={n})")
    ax.set_ylabel("Mejor F alcanzado")
    ax.set_xticks(posiciones)
    ax.set_xticklabels(etiquetas, rotation=55, ha="right", fontsize=8)
    ax.grid(axis="y", alpha=0.3)
    fig.tight_layout()
    nombre = CARPETA_SALIDA / f"boxplot_mejor_f_n{n}.png"
    fig.savefig(nombre, dpi=200, bbox_inches="tight")
    plt.close(fig)
    print(f"Gráfico guardado en: {nombre}")


def crear_boxplot_evaluaciones(df, n):
    """Compara evaluaciones hasta el óptimo, usando solo réplicas exitosas."""
    datos = df[(df["n"] == n) & (df["exito"] == True)]
    grupos = []
    etiquetas = []
    posiciones = []
    posicion = 1

    for funcion in FUNCIONES:
        for algoritmo in ALGORITMOS:
            valores = datos.loc[
                (datos["funcion"] == funcion) &
                (datos["algoritmo"] == algoritmo), "evals_hasta_optimo"
            ].dropna().to_numpy()
            if len(valores):
                grupos.append(valores)
                etiquetas.append(f"{algoritmo}\n{funcion}")
                posiciones.append(posicion)
            posicion += 1
        posicion += 1

    fig, ax = plt.subplots(figsize=(19, 7))
    if grupos:
        ax.boxplot(grupos, positions=posiciones, showfliers=False)
        ax.set_xticks(posiciones)
        ax.set_xticklabels(etiquetas, rotation=55, ha="right", fontsize=8)
    else:
        ax.text(0.5, 0.5, "Ninguna réplica alcanzó el óptimo", ha="center", va="center")
        ax.set_xticks([])
    ax.set_title(f"Evaluaciones hasta el óptimo (solo éxitos, n={n})")
    ax.set_ylabel("Número de evaluaciones")
    ax.grid(axis="y", alpha=0.3)
    fig.tight_layout()
    nombre = CARPETA_SALIDA / f"boxplot_evals_n{n}.png"
    fig.savefig(nombre, dpi=200, bbox_inches="tight")
    plt.close(fig)
    print(f"Gráfico guardado en: {nombre}")


def main():
    df = ejecutar_evaluacion()
    crear_resumen(df)
    for instancia in INSTANCIAS:
        n = instancia["n"]
        crear_boxplot_fitness(df, n)
        crear_boxplot_evaluaciones(df, n)
    print("\nFase 5 terminada correctamente.")


if __name__ == "__main__":
    main()
