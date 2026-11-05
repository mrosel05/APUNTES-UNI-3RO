"""
Controlador difuso del emparejador (Práctica 5, Parte B). Sirve de modelo para
el segundo controlador que diseñe cada pareja.

Entradas:  ratio     % de victorias en las últimas 10 partidas (0-100)
           ranking   puntuación oculta del jugador (MMR), 0-3000
Salida:    ajuste    diferencia de MMR del rival buscado respecto al jugador (-500 … +500):
                     negativo → contra peores, ≈0 → partida pareja, positivo → contra mejores
"""
from difusa import evaluar

SISTEMA = {
    "entradas": {
        "ratio":   {"universo": (0, 100),
                    "etiquetas": {"bajo": (0, 0, 50), "medio": (30, 50, 70), "alto": (50, 100, 100)}},
        "ranking": {"universo": (0, 3000),
                    "etiquetas": {"bajo": (0, 0, 1500), "medio": (1000, 1500, 2000), "alto": (1500, 3000, 3000)}},
    },
    "salidas": {
        "ajuste":  {"universo": (-500, 500),
                    "etiquetas": {"peores": (-500, -500, 0), "parejos": (-250, 0, 250), "mejores": (0, 500, 500)}},
    },
    "reglas": [
        {"si": {"ratio": "alto"},                     "entonces": ("ajuste", "mejores")},
        {"si": {"ratio": "medio"},                    "entonces": ("ajuste", "parejos")},
        {"si": {"ratio": "bajo"},                     "entonces": ("ajuste", "peores")},
        {"si": {"ratio": "alto", "ranking": "alto"},  "entonces": ("ajuste", "parejos")},  # arriba no quedan mejores
        {"si": {"ratio": "bajo", "ranking": "bajo"},  "entonces": ("ajuste", "parejos")},  # abajo no quedan peores
    ],
}


def tipo_de_partida(ajuste):
    """Traduce el ajuste numérico a las tres clases del enunciado."""
    if ajuste < -100:
        return "contra peores"
    if ajuste > 100:
        return "contra mejores"
    return "partida pareja"


if __name__ == "__main__":
    for ratio, ranking in [(80, 1500), (50, 1500), (20, 1500), (80, 2800), (20, 300), (65, 1800)]:
        a = evaluar(SISTEMA, {"ratio": ratio, "ranking": ranking})["ajuste"]
        print(f"ratio {ratio:>3} %  ranking {ranking:>4}  →  ajuste {a:+6.1f}  ({tipo_de_partida(a)})")
