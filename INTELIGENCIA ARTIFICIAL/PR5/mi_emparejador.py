"""
Parte B · Vuestro controlador. Un SEGUNDO controlador difuso del emparejador,
con dos variables de entrada que elijáis (tiempo en cola, racha de derrotas
seguidas, ping, horas jugadas esta semana, ...) y una salida (amplitud de
búsqueda en puntos de MMR, prioridad en cola, tiempo máximo de espera, ...).
Seguid el formato de emparejador.py: tres etiquetas por variable, 4-6 reglas.
Después:   python superficie.py mi_emparejador
"""
from difusa import evaluar

SISTEMA = {
    "entradas": {
        "muertes/h":    {"universo": (0, 100),
                                "etiquetas": {"precavido": (0, 0, 40), "valiente": (30, 50, 70), "suicida": (60, 100, 100)}},
        "asesinatos/h": {"universo": (0, 300),
                        "etiquetas": {"pacifista": (0, 0, 120), "estratega": (90, 150, 210), "belicista": (180, 300, 300)}},
    },
    "salidas": {
        "PeligroMapa":  {"universo": (0, 1000),
                        "etiquetas": {"trinchera": (0, 0, 350), "batalla": (150, 500, 850), "masacre": (650, 1000, 1000)}},
    },
    "reglas": [
        {"si": {"muertes/h": "suicida"},                                "entonces": ("PeligroMapa", "trinchera")},
        {"si": {"muertes/h": "valiente"},                               "entonces": ("PeligroMapa", "batalla")},
        {"si": {"muertes/h": "precavido"},                              "entonces": ("PeligroMapa", "masacre")},
        {"si": {"muertes/h": "suicida", "asesinatos/h": "belicista"},   "entonces": ("PeligroMapa", "masacre")},
        {"si": {"muertes/h": "valiente", "asesinatos/h": "estratega"},  "entonces": ("PeligroMapa", "batalla")},
        {"si": {"muertes/h": "precavido", "asesinatos/h": "pacifista"}, "entonces": ("PeligroMapa", "trinchera")},
    ],
}

def tipo_de_partida(peligro):
    if peligro < 300:
        return "trinchera"
    if peligro > 700:
        return "masacre"
    return "batalla"

if __name__ == "__main__":
    for muertes, asesinatos in [(98, 160), (5, 140), (54, 135), (98, 279), (6, 10), (50, 290)]:
            a = evaluar(SISTEMA, {"muertes/h": muertes, "asesinatos/h": asesinatos})["PeligroMapa"]
            print(f"muertes/h: {muertes:>4} - asesinatos/h {asesinatos:>3} →  PeligroMapa {a:+6.1f}  ({tipo_de_partida(a)})")
