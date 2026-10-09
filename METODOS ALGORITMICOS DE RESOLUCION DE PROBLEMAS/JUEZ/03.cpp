#include <iostream>
#include "PriorityQueue.h"

/*
EXPLICACION DE SOL. 
En este ejercicio debemos utilizar una priority queue para coger los dos 
sumandos más pequeños y sumarlos entre sí, para luego volverlo a meter
y que se convierta en un nuevo sumandos. 
Vamos a ir cogiendo los sumandos más pequeños para minimizar el 
"coste" de sumar que nos da el ejercicio. 
Por esto, en la función sol hacemos lo siguiente: 
    En otro caso, metemos cada sumando a la pqueue. 
    Luego, llevamos dos enteros: un auxiliar sobre el que realizar las sumas
    y el acumulado del coste de sumar.
    Sacamos los dos primeros, los sumamos, lo añadimos al acum y metemos
    el nuevo sumando producido a la pqueue. 


Coste de sol: 
    El coste se puede justificar como O(n*log n). 
    Un bucle que se ejecuta n veces y cada iteración tiene coste 
    log n por el push. O(n*logn).
    Por último, tenemos un bucle que se ejecuta n-1 veces y tiene coste 
    O(1) (top y sumas) + O(log n) (pop y push). O(n*log n).

    Por ello, O(n*log n) + O(n*log n) = O(n*log n)
*/

long long int sol(int n){

    PriorityQueue <long long int> p;
    while(n--){ //carga de los datos
        long long int aux;
        std::cin >> aux;
        p.push(aux); 
    }

    long long int acum = 0; //suma y vuelta
    while(p.size() > 1){
        long long int suma = p.top(); p.pop();
        suma += p.top(); p.pop();
        acum += suma;
        p.push(suma);
    }
    return acum;
}


int main(){
    int n;
    std::cin >> n;
    while(n != 0){
        std::cout << sol(n) << "\n";
        std::cin >> n;
    }
    return 0;
}

