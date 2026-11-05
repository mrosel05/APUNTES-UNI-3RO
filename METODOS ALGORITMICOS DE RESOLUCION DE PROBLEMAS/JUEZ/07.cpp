#include <iostream>
#include "PriorityQueue.h"

/*
EXPLICACION: 
Llevamos una estructura de datos con el tiempo de comienzo y de final. 
Para evitar crear dos tipos de estructura distintos, todos incluyen un periodo, 
pero esta por defecto a -1 para luego comprobar si lo tienen o no. 
El comparador solo comprueba fecha de inicio estrictamente menor. En caso
de igualdad nos dará igual, ya que las tareas tienen un tiempo mínimo de 1
y la siguiente tarea tendrá solapamiento. 
En cuanto a la función, cargamos los datos en el pqueue. 
Sacamos el primer evento e iniciamos un bucle que parará cuando no queden mas elementos
(porque si no hay eventos que comparar, no hay solapamiento) o sobrepasemos T. 
Si la siguiente fecha comienza después de T, ya sabemos que no hay otra fecha
que empiece antes y por ello, es imposible solapamiento en (o, T].
En cambio, si la finalización del siguente es anterior al comienzo de la primera, 
es que se solapan y devolvemos true. 
El último paso es reintroducir los periódicos (aquellos con periodo 
distinto de -1) al final con el comienzo y final actualizados. 

COSTE: n = N + M
Introducir los datos en el calen es de orden O(n log n) por el push. 
El último while se ejecuta n veces y tiene coste log n por el pop y push. 
Por ello, es coste O(n log n).
*/

struct data{
    data(){};
    data(int com, int fin, int per = -1): c(com), f(fin), p(per){};
    int c, f, p;
    bool operator()(data const& d1, data const& d2) const {
        return d1.c < d2.c;
    }
};

bool sol(){
    PriorityQueue<data, data> calen;
    int N, M, T, c, f, p;
    std::cin >> N >> M >> T;
    while(N--) {
        std::cin >> c >> f;
        calen.push({c, f});
    }
    while(M--){
        std::cin >> c >> f >> p;
        calen.push({c, f, p});
    }

    data d = calen.top(); calen.pop();
    while(d.c < T && !calen.empty()){
        if(T <= calen.top().c) return false;
        if(d.f > calen.top().c) return true;
        if(d.p != -1) calen.push({d.c + d.p, d.f + d.p, d.p});
        d = calen.top(); calen.pop();
    }

    return false;
}

int main(){
    int n;
    std::cin >> n;
    while(n--) std::cout << (sol() ? "SI\n" : "NO\n");
    return 0;
}