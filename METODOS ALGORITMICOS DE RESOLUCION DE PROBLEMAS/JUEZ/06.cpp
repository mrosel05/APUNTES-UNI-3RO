#include <iostream>
#include "PriorityQueue.h"
#include <set>
#include <queue>

/*
EXPLICACIÓN
Para el siguiente problema, vamos a ordenar a los clientes sobre el tiempo en el que 
les toca salir de la caja y, en caso de igualdad, el de la caja mas baja. 
Tenemos dos casos, cuando el número de clientes es menor que el número 
de cajas, por lo que la solución es la caja c + 1 y limpiamos la entrada.

En otro caso, llenamos inicialmente las cajas (i <= n) y luego, 
sacamos un cliente y metemos a otro por el orden establecido. 
Su tiempo de salida será el tiempo que ha tardado el anterior más el suyo propio. 
Devolvemos la próxima caja en quedarse libre. 

COSTE: 
Depende de n y c. Si n > c, la función tiene coste c para limpiar la entrada. O(c)
En otro caso, el bucle se ejecuta c veces, con un coste log n en ambos casos
(ya que o metemos en la caja o metemos y sacamos). Coste O(c log n).

*/

struct data{
    bool operator()(data const& d1, data const& d2){
        return d1.tiempo < d2.tiempo || (d1.tiempo == d2.tiempo && d1.caja < d2.caja);
    }
    int tiempo, caja;
};



int resolver(int n, int c){
    PriorityQueue<data, data> cajas;
    int tiempo;

    if(n > c){ //menos clientes que cajas
        int sol = c + 1;
        while(c--) std::cin >> tiempo; //limpiamos la entrada
        return sol;
    }

    for(int i = 1; i <= c; i++){
        std::cin >> tiempo; 
        if(i <= n) cajas.push({tiempo, i});
        else {
            data d = cajas.top(); cajas.pop();
            cajas.push({d.tiempo + tiempo, d.caja});
        }
    }
    return cajas.top().caja;
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