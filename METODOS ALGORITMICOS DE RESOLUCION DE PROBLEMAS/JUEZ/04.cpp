#include <iostream>
#include "PriorityQueue.h"

/*
Explicación: 
Para este problema debemos utilizar una estructura que nos de siempre
aquel elemento para el cual su tiempo de espera sea menor para notificarle. 
Es por ello necesaria una priority queue ordenada por el tiempo de aviso,
y en caso de igualdad por el id. 
La clave del ejercicio son los campos periodo y aviso. El periodo es cada
cuanto tiempo se ha determinado que debemos avisar al usuario, por ello
el aviso es el momento en el que se le debe avisar. Cada vez que avisemos
al usuario, el aviso ocurrira un periodo más tarde, motivo por el que hacemos
aviso += periodo y lo volvemos a meter al pqueue. 

Coste: 
La funcion sol consiste en de dos bucles: 
    - En el primero cargamos todos los datos. Se ejecuta n veces, siendo 
    n el número de usuarios que nos dan y tiene coste O(log n) por el push.
    - En el segundo resolvemos el ejercicio, notificando por orden. Se ejecuta
    k veces y tiene coste O(log n). 

Por ello, el coste de sol sera O((n + k) log n).
*/

struct tDatos{
    bool operator() (tDatos const& d1, tDatos const& d2){
        return d1.aviso < d2.aviso || (d1.aviso == d2.aviso && d1.id < d2.id ); 
    }

    tDatos(): periodo(0), id(0), aviso(0){}
    tDatos(int i, int p) : periodo(p), id(i), aviso(p) {}
    int periodo, id, aviso;
};

void sol(int n){
    PriorityQueue <tDatos, tDatos> p;
    while(n--){
        int aux1, aux2;
        std::cin >> aux1 >> aux2;
        p.push(tDatos(aux1, aux2));
    }

    std::cin >> n;
    while(n--){
        tDatos aux = p.top(); p.pop();
        std::cout << aux.id << "\n";
        aux.aviso += aux.periodo;
        p.push(aux);
    }
    std::cout << "---\n";
}

int main(){
    int n;
    std::cin >> n;
    while(n != 0){
        sol(n);
        std::cin >> n;
    }
    return 0;
}