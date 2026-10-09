#include <iostream>
#include "PriorityQueue.h"
#include <set>
#include <queue>

/*
Cola de clientes. 
Cuando haya hueco, cogemos tantos como huecos haya. 
Los metemos en el priority queue con la posicion de la caja en la que estan
Para saber las cajas libres, llevamos un set de cajas, ordenado para que nos de la mas baja
Metemos al cliente en la pqueue con la pos de cola y momento en que llega. 
Sacamos al primero de la pqueue y para el resto, ha pasado tanto tiempo
como hubiera necesitado el cliente para todos los que estuvieran en la cola. 
Rebajamos el tiempo de todos? Eso es lineal, no se puede. 
Necesitamos un ordenador que ordene segun una cola de tiempos que han pasado
al sacar cada cliente y el tiempo que les quedaba. De esta manera si se puede. 
*/

struct data{
    bool operator()(data const& d1, data const& d2){
        return d1.tiempo + d1.momentoEntrada < d2.tiempo + d2.momentoEntrada;
    }
    int tiempo, momentoEntrada, caja;
};



int resolver(int n, int c){
    PriorityQueue<data, data> cajas;
    std::queue<int> cola;
    std::set<int> cajasLibres;
    int tiempo_pasado = 0;
    while(c--){ //creamos la cola
        int aux;
        std::cin >> aux;
        cola.push(aux);
    }

    for(int i = 1; i <= n; i++){ //llenamos las cajas.
        if(cola.empty()) cajasLibres.insert(i);
        else {
            cajas.push({cola.front(), tiempo_pasado, i});
            cola.pop();
        }
    }

    while(!cola.empty()){ //mientras haya gente
        data d = cajas.top(); cajas.pop();
        while(!cajas.empty() && 
        cajas.top().tiempo + cajas.top().momentoEntrada == d.tiempo + d.momentoEntrada){
            cajasLibres.insert(d.caja);
            d = cajas.top(); cajas.pop();
        }
        cajasLibres.insert(d.caja);
        tiempo_pasado += d.tiempo;
        while(cajas.size() < n && !cola.empty()){
            cajas.push({cola.front(), tiempo_pasado, *(cajasLibres.begin())});
            cola.pop(); cajasLibres.erase(cajasLibres.begin());
        }
    }

    if(cajas.size() < n){
        return cajas.top().caja;
    }
    else return *(cajasLibres.begin());

}

int main(){
    int n, c;
    std::cin >> n >> c;
    while(n != 0 && c != 0){
        std::cout << resolver(n, c) << "\n";
        std::cin >> n >> c;
    }
    return 0;
}