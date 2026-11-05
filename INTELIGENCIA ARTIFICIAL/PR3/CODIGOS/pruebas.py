import csv

from bloques import Problema, ejecutar

SEMILLAS = range(1, 21)
N = 32
K = 4
PRESUPUESTO = 20000
FUNCIONES = ["onemax", "plateau", "royalroad", "deceptive"]
CABECERA = ["algoritmo", "parametros", "funcion", "n", "k", "presupuesto", "repeticion",
            "semilla", "mejor_f", "optimo", "exito", "evals_hasta_optimo"]


def ejecutar_configuracion(w, algoritmo, parametros, funcion):

    for repeticion in range(1,21):
        for semilla in SEMILLAS:
            prob = Problema(funcion, n=N, k=K, presupuesto=PRESUPUESTO)
            r = ejecutar(algoritmo, parametros, prob, semilla)

            w.writerow([algoritmo, str(parametros), funcion, N, K, PRESUPUESTO, repeticion, semilla, r.mejor_f,
                prob.optimo, r.mejor_f == prob.optimo, r.evals_hasta_optimo])

def aleatoria():
    print("Busqueda aleatoria")

    with open("aleatoria.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            ejecutar_configuracion(
                w, "aleatoria", {}, funcion)


def escalador():
    print("Escalador")

    with open("escalador.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            for aceptar_iguales in [True, False]:
                params = {"aceptar_iguales": aceptar_iguales}
                ejecutar_configuracion(
                    w, "escalador", params, funcion)


def recocido():
    print("Recocido")

    T0_VALORES = [1, 50, 100]
    ALFAS = [0.95, 0.99, 0.995]

    with open("recocido.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            for T0 in T0_VALORES:
                params = {"T0": T0, "patron": "presupuesto"}

                ejecutar_configuracion(
                    w, "recocido", params, funcion)

                for alfa in ALFAS:
                    params = {"T0": T0, "patron": "geometrico", "alfa": alfa}

                    ejecutar_configuracion(
                        w, "recocido", params, funcion)


def umbral():
    print("Umbral")

    U0_VALORES = [3, 2, 0.999]
    ALFAS = [0.95, 0.99, 0.995, 0.999]

    with open("umbral.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            for U0 in U0_VALORES:
                params = {"U0": U0, "patron": "presupuesto"}
                ejecutar_configuracion(
                    w, "umbral", params, funcion)

                for alfa in ALFAS:
                    params = {"U0": U0, "patron": "geometrico", "alfa": alfa}
                    ejecutar_configuracion(
                        w, "umbral", params, funcion)


def rrt():
    print("RRT")

    with open("rrt.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            for D in range(1, 13):
                params = {"D": D}
                ejecutar_configuracion(
                    w, "rrt", params, funcion)


def diluvio():
    print("Gran Diluvio")

    NIVELES = [0, 1, 2, 3]
    LLUVIAS = [0.5, 1, 2]

    with open("diluvio.csv", "w", newline="") as fh:
        w = csv.writer(fh)
        w.writerow(CABECERA)

        for funcion in FUNCIONES:
            for L0 in NIVELES:
                for lluvia in LLUVIAS:
                    params = {"L0": L0, "lluvia": lluvia}
                    ejecutar_configuracion(
                        w, "diluvio", params, funcion)


if __name__ == "__main__":
    aleatoria()
    escalador()
    recocido()
    umbral()
    rrt()
    diluvio()
    print("\nTodos los experimentoshan finalizado correctamente.")