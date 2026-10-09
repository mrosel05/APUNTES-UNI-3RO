#include <iostream>
#include "PriorityQueue.h"

/*
Justificación: 
Como los pacientes dependen de la prioridad, ordenamos con ese valor. Ante igualdad, 
toma el que tenga mayor tiempo de espera (porque en la funcion hacemos n--).

Coste: 
Insertar tiene coste log n, por el push. 
Atender tiene coste log n, por el pop. 
Para n operaciones, esto es n log n. 

*/

struct tDatos{
    bool operator()(tDatos const& d1, tDatos const& d2) {
        return d1.prioridad > d2.prioridad || 
            (d1.prioridad == d2.prioridad && d1.timestamp > d2.timestamp);
    }
    tDatos(): prioridad(0), timestamp(0), nombre(""){}
    tDatos(int i, int t, std::string s): prioridad(i), timestamp(t), nombre(s){}
    int prioridad, timestamp;
    std::string nombre;
};

void res(int n){
    PriorityQueue<tDatos, tDatos> p;
    while(n--){
        char op;
        std::cin >> op;
        if(op == 'I') {
            std::string nombre;
            int prioridad;
            std::cin >> nombre >> prioridad;
            p.push(tDatos(prioridad, n, nombre));
        }
        else {
            tDatos d = p.top(); p.pop();
            std::cout << d.nombre << "\n";
        }
    }
    std::cout << "---\n";
}


int main(){
    int n;
    std::cin >> n;
    while(n != 0){
        res(n);
        std::cin >> n;
    }
    return 0;
}