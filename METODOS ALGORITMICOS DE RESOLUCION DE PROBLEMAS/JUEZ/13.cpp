/*
 Nombre y apellidos de los componentes del grupo:

 Xiang Lin - Mario Rosell
 */

#include <iostream>
#include <string>
#include <cassert>
#include "IndexPQ_gen.h"
using namespace std;

/* [3 puntos]
Explicacion detallada de como y por que vas a resolver el problema utilizando colas de prioridad.

Vamos a usar una cola de prioridad con prioridad variable parametrizada (que llamaremos desde ahora cola de prioridad) porque en el problema vienen dos tipos de consultas:
    - 'pais num': evento que viene puede añadir un nuevo elemento con una prioridad o actualizar/ modificar la prioridad de un elemento existente.
    - ?: evento que consulta el elemento más prioritario. 

Para evitar tener que recorrer todos los elementos cada vez que recibimos una consulta de tipo '?' (que costaría O(n) por consulta), usamos una cola de prioridad para que
cada consulta cueste O(log n). n = numero de elementos distintos recibidos hasta el momento.

Para resolverlo, primero intentamos leer el numero de eventos de la entrada. 
Si no es posible se acaba el problema.
Si es posible, declaramos la cola de prioridad con prioridad variable (IndexPQ) y, por cada evento, leemos el nombre del evento y si el nombre es:
 - ?: consultamos el elemento con mayor prioridad y lo escribimos por consola junto a su puntuación actual.
 - En otro caso ('pais num'): miramos si el elemento estaba en la cola para ver su puntación hasta ahora.
                              Si no estaba su puntuacion sera cero.
                              Si estaba, cogemos su puntuacion.
                              Ahora que tenemos la puntacion, actualizamos la prioridad del elemento sumándole su puntuacion anterior.
                              Es importante destacar que si el elemento no estaba, 'update' lo añade a la cola de prioridad.

Definicion de los tipos implicados y del comparador.

Dentro de la cola de prioridad, cada elemento esta identificado por un string (nombre del pais) y su prioridad. Su prioridad la representamos con el struct prioridad
que consta de los siguientes valores:
    - pais: el nombre del pais
    - punt: la puntuacion acumulada del pais

Para comparar, definimos un comparador propio en el struct y para ello usaremos el operador parentesis. Siguiendo el enunciado, como cuando el comparador devuelve true
el elemento es considerado mas prioritario y, por tanto, se eleva, damos mas prioridad al elemento con mayor puntuacion o, ante igualdad de puntuacion, al menor
string (cadena lexicograficamente mas pequeña)

 */

struct prioridad {
    string pais;
    int punt;

    bool operator() (prioridad const& d1, prioridad const& d2) const {
        return d1.punt > d2.punt || (d1.punt == d2.punt && d1.pais < d2.pais);
    }
};

 /* [5 puntos]
  Completa y comenta el codigo para resolver el problema.
 */
bool resuelveCaso() {
    int numEventos;

    cin >> numEventos;
    if (!cin) return false;
    IndexPQ<string, prioridad, prioridad> euro; // Cola de prioridad con parioridad variable parametrizada
    for (int i = 0; i < numEventos; ++i) {
        string nombre;

        // Lectura y determinacion del evento
        cin >> nombre;
        if (nombre == "?") { 
            prioridad prim = euro.top().prioridad; // Consulta de la cima del heap

            cout << prim.pais << " " << prim.punt << "\n";
        }
        else {
            int pts;

            cin >> pts;
            int ant = !euro.contiene(nombre) ? 0 : euro.priority(nombre).punt; // Consulta de puntuacion de si existia
            euro.update(nombre, { nombre, ant + pts }); // Actualizacion de la prioridad del elemento
        }
    }
    cout << "---\n";
    return true;
}

/* [2 puntos]
 Analiza y justifica el coste en el caso peor de resuelveCaso

 El bucle for principal se ejecute n = numEventos veces, para cada vuelta consultamos la cima del heap o la prioridad de un elemento y luego actualizamos dicha prioridad.
 Como a lo sumo puede haber n elementos en el heap en el peor caso, cada consulta de la cima es constate O(1) y cada actualizacion tiene un coste logaritmico O(log n).

 Luego el coste es O(n log(n)) con n = numEventos en el peor de los casos.  
*/

int main() {
    while (resuelveCaso());
    return 0;
}       