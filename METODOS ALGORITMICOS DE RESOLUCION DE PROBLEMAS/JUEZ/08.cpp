#include <iostream>
#include "PriorityQueue.h"

/*
EXPLICACION: 
Guardaremos las pilas de cada tipo en un pqueue, pues queremos que nos de la pila
más alta de cada uno. Repetiremos así hasta que nos quedemos sin pilas en uno de los 
dos "montones" de pilas. 
Dentro de cada uno de los dias vamos a tomar una pila de cada monton, hacer la diferencia
de ellas y guardar en una lista temporal, ya que no las podemos devolver al monton directamente. 
Almacenamos tamb el tiempo de vuelo y al final del dia, escribimos por consola el tiempo. 
Volvemos a meter también las pilas que quedaron con energia y repetimos mientras queden pilas.

COSTE: 
El bucle interior se ejecuta como máximo O(A+B) veces en total, ya que en 
cada iteración se extrae una pila de cada tipo y, como mucho, se vuelve a insertar
 una pila residual. Por tanto, en cada iteración se elimina definitivamente al 
 menos una pila. Si ambas pilas tienen la misma energía, las dos se agotan y no 
 se vuelve a insertar ninguna.

Cada operación de inserción o extracción de las colas de prioridad tiene un coste 
O(log(A+B)), ya que estas contienen como máximo A+B pilas. Por tanto, el coste 
total de las operaciones de las colas es O((A+B)log(A+B)). La inserción inicial 
de las pilas tiene ese mismo orden de coste.

En conclusión, el coste temporal de sol es O(n log n), siendo n=A+B.
*/

void sol(int N, int A, int B){
    int aux;
    PriorityQueue<int, std::greater<int>> pA, pB;
    while(A--){
        std::cin >> aux;
        pA.push(aux);
    }
    while(B--){
        std::cin >> aux;
        pB.push(aux);
    }

    
    while(!pA.empty() && !pB.empty()){
        int dia = 0, cont = 0;
        std::vector<int> usadasA, usadasB;
        while(!pA.empty() && !pB.empty() && cont < N){
            int pilaA = pA.top(); pA.pop();
            int pilaB = pB.top(); pB.pop();
            if(pilaA > pilaB) {
                pilaA -= pilaB;
                dia += pilaB;
                usadasA.push_back(pilaA);
            }
            else if(pilaB > pilaA){
                pilaB -= pilaA;
                dia += pilaA;
                usadasB.push_back(pilaB);
            }
            else dia += pilaA;
            cont++;
        }
        for(int i: usadasA) pA.push(i);
        for(int i: usadasB) pB.push(i);
        std::cout << dia << " ";
    }
    std::cout << "\n"; 
}

int main(){
    int N, A, B, aux;
    while(std::cin >> N >> A >> B) 
        sol(N, A, B);
    return 0;
}