#define LABEL_MAX_LEN 15 // Max chars sin contar el '\0'

typedef struct {
    int id;                // Identificador entero
    double value;          // Un valor de punto flotante
    char label[LABEL_MAX_LEN + 1]; // Etiqueta de texto (tamaño fijo)
} SimpleRecord;