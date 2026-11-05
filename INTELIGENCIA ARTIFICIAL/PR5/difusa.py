"""
Práctica 5 · Parte B — Inferencia difusa tipo Mamdani (mín-máx, centroide)
Inteligencia Artificial I · Curso 2026-27

Representación:
  variable = {"universo": (140, 200),                       # rango numérico
              "etiquetas": {"pequeño": (140, 150, 160),     # triángulos (a, b, c): vale 1 en b
                            "mediano": (155, 167.5, 180),
                            "alto":    (175, 187.5, 200)}}
  regla    = {"si": {"altura": "alto", "peso": "bajo"},     # conjunción de (variable, etiqueta)
              "entonces": ("talla", "L")}                   # (variable de salida, etiqueta)
"""


def triangular(x, a, b, c):
    """Función de pertenencia triangular: 0 fuera de [a, c], 1 en b, lineal entre medias.
    Se admite a == b (hombro izquierdo) o b == c (hombro derecho): por eso las
    comparaciones de los extremos son estrictas (x < a, x > c) y no x ≤ a, x ≥ c."""
    if x < a or x > c: return 0
    if x == b: return 1
    if x < b: return (x - a) / (b - a)
    return (c - x) / (c - b)

def fuzzificar(x, variable):
    """Grado de pertenencia de x a cada etiqueta de la variable.
    Devuelve {etiqueta: grado}."""
    pertenencia = {}
    for etiqueta, tri in variable["etiquetas"].items():
        pertenencia[etiqueta] = triangular(x, tri[0], tri[1], tri[2])
    return pertenencia


def inferir(reglas, grados_entrada):
    """Inferencia mín-máx.
    grados_entrada = {variable: {etiqueta: grado}} (salida de fuzzificar por cada entrada).
    Devuelve {variable_salida: {etiqueta: grado}}: para cada etiqueta de salida, el MÁXIMO
    sobre las reglas que la concluyen del MÍNIMO de los grados de su antecedente."""
    variable_salida  = {}
    for regla in reglas: 
        condiciones, cons0, cons1 = regla["si"], regla["entonces"][0], regla["entonces"][1]
        grad_cond = min([grados_entrada[var][etiq] for var, etiq in condiciones.items()])
        if cons0 not in variable_salida: variable_salida[cons0] = {}
        variable_salida[cons0][cons1] = max(variable_salida[cons0].get(cons1, 0), grad_cond)
    return variable_salida

        

def centroide(variable, grados, paso=0.1):
    """Desfuzzificación por centro de gravedad.
    Cada etiqueta se trunca a su grado inferido, se agregan con el máximo y se calcula
    Σ μ(y)·y / Σ μ(y) sobre el universo discretizado con el paso dado.
    Si ningún grado es > 0 devuelve el centro del universo."""

    min_u, max_u = variable["universo"][0], variable["universo"][1]
    
    if not grados or all(g <= 0 for g in grados.values()):
        return (min_u + max_u) / 2
        
    numerador = 0.0
    denominador = 0.0
    
    y = min_u
    while y <= max_u:
        mu_y = 0.0
        for etiqueta, grado in grados.items():
            if grado > 0:
                tri = variable["etiquetas"][etiqueta]
                mu_y = max(mu_y, min(grado, triangular(y, tri[0], tri[1], tri[2])))
                
        numerador += mu_y * y
        denominador += mu_y
        y += paso
        
    if denominador == 0: return (min_u + max_u) / 2  
    return numerador / denominador

def evaluar(sistema, entradas, paso=0.1):
    """Ciclo completo: fuzzificar cada entrada → inferir → centroide de cada salida.
    sistema = {"entradas": {nombre: variable}, "salidas": {nombre: variable}, "reglas": [...]}
    entradas = {nombre: valor numérico}.  Devuelve {salida: valor numérico}."""
    grados = {v: fuzzificar(entradas[v], var) for v, var in sistema["entradas"].items()}
    inferido = inferir(sistema["reglas"], grados)
    return {s: centroide(var, inferido.get(s, {}), paso) for s, var in sistema["salidas"].items()}


# ----------------------------------------------------------------------
# La variable "altura" de las transparencias
# ----------------------------------------------------------------------
ALTURA = {"universo": (140, 200),
          "etiquetas": {"pequeño": (140, 150, 160),
                        "mediano": (155, 167.5, 180),
                        "alto": (175, 187.5, 200)}}

if __name__ == "__main__":
    print("Laura (179 cm):", {k: round(v, 2) for k, v in fuzzificar(179, ALTURA).items()})
    print("Centroide con pequeño 0.5, mediano 0.3, alto 0:",
          round(centroide(ALTURA, {"pequeño": 0.5, "mediano": 0.3, "alto": 0.0}), 1), "cm")
