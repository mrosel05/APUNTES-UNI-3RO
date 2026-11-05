"""
Práctica 5 · Parte A — Motor de reglas con factores de certeza (MYCIN)
Inteligencia Artificial I · Curso 2026-27

Representación:
  hechos  = {"A": 1.0, "B": 0.5}             # hecho -> factor de certeza en [-1, 1]
  reglas  = [{"si": ["A", "B", "no C"],      # antecedente: conjunción de literales
              "entonces": "H",               # consecuente: un único hecho
              "fc": 0.7,                     # FC de la regla, FC(H / A∧B∧¬C)
              "nombre": "R1"}]
Un literal "no X" se evalúa con FC(¬X) = -FC(X).
"""


# ----------------------------------------------------------------------
# Fórmulas de la teoría de la confirmación (transparencias 72-74)
# ----------------------------------------------------------------------

def fc_negacion(fc):
    """FC(¬x) = -FC(x)."""
    return -fc

def fc_conjuncion(fcs):
    """FC(A ∧ B ∧ ...) = min de los FC de las premisas."""
    return min(fcs)

def fc_disyuncion(fcs):
    """FC(A ∨ B ∨ ...) = max de los FC de las premisas."""
    return max(fcs)

def fc_regla(fc_premisa, fc_de_la_regla):
    """Propagación vertical: FC(h) = FC(e) · FC(h/e) si FC(e) > 0; 0 en otro caso."""
    return fc_premisa * fc_de_la_regla if fc_premisa > 0 else 0

def combinar(fc1, fc2):
    """Acumulación de evidencias de dos reglas sobre la misma hipótesis."""
    if fc1 > 0 and fc2 > 0:   return fc1 + fc2 - fc1 * fc2
    elif fc1 < 0 and fc2 < 0: return fc1 + fc2 + fc1 * fc2
    else: return (fc1 + fc2) / (1 - min(abs(fc1), abs(fc2)))

# ----------------------------------------------------------------------
# Motor de encadenamiento hacia adelante
# ----------------------------------------------------------------------

def fc_literal(literal, hechos):
    """FC de un literal ("X" o "no X") dado el diccionario de hechos.
    Devuelve None si el hecho es desconocido."""
    if literal.startswith("no "):
        nombre = literal[3:]
        return None if nombre not in hechos else fc_negacion(hechos[nombre])
    return hechos.get(literal)

"""
Representación:
  hechos  = {"A": 1.0, "B": 0.5}             # hecho -> factor de certeza en [-1, 1]
  reglas  = [{"si": ["A", "B", "no C"],      # antecedente: conjunción de literales
              "entonces": "H",               # consecuente: un único hecho
              "fc": 0.7,                     # FC de la regla, FC(H / A∧B∧¬C)
              "nombre": "R1"}]
Un literal "no X" se evalúa con FC(¬X) = -FC(X).
"""

def encadenar_adelante(hechos, reglas, traza=False):
    """Dispara cada regla una sola vez, en cuanto todas sus premisas son conocidas.
    Devuelve un diccionario nuevo con los hechos iniciales y los inferidos."""
    hechos_nuevos = dict(hechos)

    cambios = True
    reglas_usadas = set()
    while cambios: 
        cambios = False
        for regla in reglas: 
            premisas, hecho_nuevo, fc_reg, nombre = regla["si"], regla["entonces"], regla["fc"], regla["nombre"]
            if nombre in reglas_usadas: continue

            fc_premisas = [fc_literal(premisa, hechos_nuevos) for premisa in premisas]
            if None in fc_premisas: continue

            fc_premisa = fc_conjuncion(fc_premisas)
            fc_hecho_nuevo = fc_regla(fc_premisa, fc_reg)

            huboCombinacion = True
            if hecho_nuevo in hechos_nuevos: 
                hechos_nuevos[hecho_nuevo] = combinar(hechos_nuevos[hecho_nuevo], fc_hecho_nuevo)
            else: 
                hechos_nuevos[hecho_nuevo] = fc_hecho_nuevo
                huboCombinacion = False

            if traza: 
                s1= f"{nombre}: {premisas} -> {hecho_nuevo}"
                if huboCombinacion: 
                    s1 += f"\tHay combinacion. fc_hecho_nuevo: {fc_hecho_nuevo:.3f}"
                s2 = f"\tFC_prem: {fc_premisa:.3f} | FC_regla: {fc_reg:.3f} | FC_h: {hechos_nuevos[hecho_nuevo]:.3f}"
                print(s1)
                print(s2)
            reglas_usadas.add(nombre)
            cambios = True
    return hechos_nuevos

def hipotesis_ordenadas(hechos, candidatas):
    """Lista de (hipótesis, FC) ordenada de mayor a menor certeza."""
    return sorted(((h, hechos.get(h, 0.0)) for h in candidatas),
                  key=lambda par: par[1], reverse=True)


if __name__ == "__main__":
    # Ejercicio de la transparencia 71
    reglas = [{"si": ["A", "B", "C"], "entonces": "H", "fc": 0.5, "nombre": "R1"},
              {"si": ["D", "F"], "entonces": "H", "fc": 0.9, "nombre": "R2"}]
    hechos = {"A": 1, "B": 0.5, "C": 0.3, "D": 0.7, "F": 0.2}
    print("Transparencia 71:")
    r = encadenar_adelante(hechos, reglas, traza=True)
    print("  FC(H) =", round(r["H"], 3))

    # Ejercicio de la transparencia 75
    reglas = [{"si": ["A"], "entonces": "B", "fc": -0.6, "nombre": "R1"},
              {"si": ["no B"], "entonces": "C", "fc": -0.9, "nombre": "R2"}]
    print("Transparencia 75:")
    r = encadenar_adelante({"A": 0.8}, reglas, traza=True)
    print("  FC(C) =", round(r["C"], 3))
