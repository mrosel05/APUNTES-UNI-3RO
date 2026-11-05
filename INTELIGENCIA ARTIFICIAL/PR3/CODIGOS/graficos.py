import pandas as pd
import matplotlib.pyplot as plt

FUNCIONES = ["onemax", "plateau", "royalroad", "deceptive"]


def grafica(csv_entrada, png_salida):

    df = pd.read_csv(csv_entrada)

    fig, axs = plt.subplots(4, 4, figsize=(20, 14))

    for col, funcion in enumerate(FUNCIONES):

        datos = df[df["funcion"] == funcion]

        # =====================================================
        # PORCENTAJE DE EXITO
        # =====================================================

        exito = (
            datos.groupby("parametros")["exito"]
            .mean()
            * 100
        )

        ax = axs[0, col]

        ax.bar(range(len(exito)), exito.values)

        ax.set_title(funcion)

        if col == 0:
            ax.set_ylabel("% exito")

        ax.set_ylim(0, 100)

        ax.set_xticks(range(len(exito)))
        ax.set_xticklabels(
            exito.index,
            rotation=90,
            fontsize=6
        )

        # =====================================================
        # MEDIA mejor_f
        # =====================================================

        mejor_f = (
            datos.groupby("parametros")["mejor_f"]
            .mean()
        )

        ax = axs[1, col]

        ax.bar(range(len(mejor_f)), mejor_f.values)

        if col == 0:
            ax.set_ylabel("media mejor_f")

        ax.set_xticks(range(len(mejor_f)))
        ax.set_xticklabels(
            mejor_f.index,
            rotation=90,
            fontsize=6
        )

        # =====================================================
        # DESVIACION TIPICA mejor_f
        # =====================================================

        desv = (
            datos.groupby("parametros")["mejor_f"]
            .std()
            .fillna(0)
        )

        ax = axs[2, col]

        ax.bar(range(len(desv)), desv.values)

        if col == 0:
            ax.set_ylabel("desv mejor_f")

        ax.set_xticks(range(len(desv)))
        ax.set_xticklabels(
            desv.index,
            rotation=90,
            fontsize=6
        )

        # =====================================================
        # EVALUACIONES HASTA EL OPTIMO
        # =====================================================

        evals = (
            datos.groupby("parametros")["evals_hasta_optimo"]
            .mean()
        )

        ax = axs[3, col]

        ax.bar(range(len(evals)), evals.values)

        if col == 0:
            ax.set_ylabel("evals")

        ax.set_xticks(range(len(evals)))
        ax.set_xticklabels(
            evals.index,
            rotation=90,
            fontsize=6
        )

    plt.tight_layout()

    plt.savefig(
        png_salida,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close()


grafica(
    "aleatoria.csv",
    "aleatoria.png"
)

grafica(
    "escalador.csv",
    "escalador.png"
)

grafica(
    "recocido.csv",
    "recocido.png"
)

grafica(
    "umbral.csv",
    "umbral.png"
)

grafica(
    "rrt.csv",
    "rrt.png"
)

grafica(
    "diluvio.csv",
    "diluvio.png"
)

print("Graficas generadas correctamente")
