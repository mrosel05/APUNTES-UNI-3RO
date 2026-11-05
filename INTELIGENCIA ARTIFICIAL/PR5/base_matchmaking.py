"""
Base de conocimiento de ejemplo: emparejamiento (matchmaking) en un juego
competitivo por equipos, al estilo del League of Legends (Práctica 5, Parte A).

El sistema recibe lo que se sabe del jugador y decide a qué tipo de partida
lo manda. Los FC de las reglas son estimaciones de un "experto" (un diseñador
del juego); las reglas R10 y R11 son deliberadamente discutibles (ver A6).

Hechos observables (los da el cliente del juego, con la certeza con que los observa):
  racha_positiva   ha ganado la mayoría de las últimas 10 partidas
  racha_negativa   ha perdido la mayoría de las últimas 10 partidas
  ranking_alto     está en las ligas altas (Diamante o más)
  ranking_bajo     está en las ligas bajas (Hierro, Bronce)
  rol_fijo         juega siempre el mismo rol
  compra           gasta dinero en la tienda del juego
  chatea           interactúa por el chat con su equipo
  cola_larga       lleva mucho tiempo esperando en la cola

Hipótesis intermedias: en_forma, comprometido, riesgo_abandono
Decisión final (hipótesis):  partida_pareja, contra_mejores, contra_peores
"""

REGLAS = [
    # --- nivel 1: observaciones → perfil del jugador
    {"nombre": "R1", "si": ["racha_positiva", "rol_fijo"],
     "entonces": "en_forma", "fc": 0.8},
    {"nombre": "R2", "si": ["racha_positiva", "no rol_fijo"],
     "entonces": "en_forma", "fc": 0.5},
    {"nombre": "R3", "si": ["racha_negativa"],
     "entonces": "en_forma", "fc": -0.7},
    {"nombre": "R4", "si": ["compra", "chatea"],
     "entonces": "comprometido", "fc": 0.7},
    {"nombre": "R5", "si": ["compra", "no chatea"],
     "entonces": "comprometido", "fc": 0.4},
    {"nombre": "R6", "si": ["racha_negativa", "no chatea"],
     "entonces": "riesgo_abandono", "fc": 0.6},
    {"nombre": "R7", "si": ["racha_negativa", "no compra"],
     "entonces": "riesgo_abandono", "fc": 0.5},

        # --- reglas anadidas por nosotros. De nivel 1
        {"nombre": "R16", "si": ["juega_de_noche", "racha_negativa"],
         "entonces": "riesgo_abandono", "fc": 0.6,},  # A favor: jugar de noche con racha mala dispara el riesgo de dejarlo
        {"nombre": "R17", "si": ["juega_de_noche", "rol_fijo"],
         "entonces": "riesgo_abandono", "fc": -0.4,},  # En contra: quien juega su rol fijo de noche suelen ser jugadores que farmean experiencia
    # --- nivel 2: perfil → tipo de partida
    {"nombre": "R8", "si": ["en_forma"],
     "entonces": "contra_mejores", "fc": 0.7},
    {"nombre": "R9", "si": ["en_forma", "ranking_alto"],
     "entonces": "contra_mejores", "fc": -0.5},     # arriba del todo no quedan "mejores"
    {"nombre": "R10", "si": ["riesgo_abandono"],
     "entonces": "contra_peores", "fc": 0.8},       # discutible: ¿regalar victorias?
    {"nombre": "R11", "si": ["comprometido", "riesgo_abandono"],
     "entonces": "contra_peores", "fc": 0.4},       # discutible: ¿mimar a quien paga?
    {"nombre": "R12", "si": ["riesgo_abandono", "ranking_bajo"],
     "entonces": "contra_peores", "fc": -0.6},      # abajo del todo no quedan "peores"
    {"nombre": "R13", "si": ["rol_fijo", "chatea"],
     "entonces": "partida_pareja", "fc": 0.5},
    {"nombre": "R14", "si": ["cola_larga"],
     "entonces": "partida_pareja", "fc": 0.6},      # si lleva mucho esperando, lo que haya
    {"nombre": "R15", "si": ["cola_larga", "ranking_alto"],
     "entonces": "contra_peores", "fc": 0.4},       # en las ligas altas hay poca gente
]

HIPOTESIS = ["partida_pareja", "contra_mejores", "contra_peores"]

# Tres jugadores de consulta. Cada uno da la certeza con que se observa cada hecho.
CASOS = {
    "jugador_1_en_racha": {
        "racha_positiva": 0.9, "racha_negativa": -0.9, "ranking_alto": -0.6, "ranking_bajo": -0.8,
        "rol_fijo": 0.8, "compra": 0.3, "chatea": 0.7, "cola_larga": -1.0},
    "jugador_2_frustrado": {
        "racha_positiva": -1.0, "racha_negativa": 0.8, "ranking_alto": -1.0, "ranking_bajo": 0.5,
        "rol_fijo": -0.4, "compra": 0.9, "chatea": -0.6, "cola_larga": -0.7},
    "jugador_3_diamante_esperando": {
        "racha_positiva": 0.5, "racha_negativa": -0.5, "ranking_alto": 1.0, "ranking_bajo": -1.0,
        "rol_fijo": 1.0, "compra": -0.2, "chatea": 0.9, "cola_larga": 0.9},
    "jugador_4_nocturno": {
        "racha_positiva": -1.0,"racha_negativa": 0.8, "ranking_alto": -1.0, "ranking_bajo": 0.5,
        "rol_fijo": 0.7, "compra": 1.0, "chatea": 1.0,"cola_larga": -1.0,"juega_de_noche": 0.9,},
    "jugador_2_modificado": {
            "racha_positiva": -1.0, "racha_negativa": 0.8, "ranking_alto": -1.0, "ranking_bajo": 0.5,
            "rol_fijo": -0.4, "compra": -0.5, "chatea": -0.6, "cola_larga": -0.7},
}

if __name__ == "__main__":
    from fc import encadenar_adelante, hipotesis_ordenadas
    for nombre, hechos in CASOS.items():
        print(f"\n== {nombre} ==")
        r = encadenar_adelante(hechos, REGLAS, traza=True)
        for h, v in hipotesis_ordenadas(r, HIPOTESIS):
            print(f"  {h:<16} {v:+.3f}")
