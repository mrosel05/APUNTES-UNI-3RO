#include <iostream>
#include <algorithm>
#include "ConjuntosDisjuntos.h"

/*
Buscaremos la máxima componente conexa de todo el grafo. 
Para ello vamos usar los conjuntos disjuntos, especialmente porque 
indica que puede darnos una amistad dos veces y si lo hacemos con la 
implementación normal del grafo, o comprobamos si ya están relacionados 
mirando todos los adyacentes de uno de ellos (V*A en el peor caso) o 
permitimos dobles aristas (estructura erronea del grafo y empeora el dfs).

Vamos a añadir V vertices y luego a recorrer las componentes, que en el 
peor caso serán igual a V. Por ello, esto es del coste de O(V) en tiempo
y del tamaño de ConjuntosDisjuntos en memoria. 
*/

class MaximaComponenteConexa{
    public: 
        MaximaComponenteConexa(): max(0){
            int N, M, v1, v2;
            std::cin >> N >> M;
            ConjuntosDisjuntos cd(N);
            while(M--){
                std::cin >> v1 >> v2;
                cd.unir(v1 - 1, v2 - 1);
            }
            for(int i = 0; i < N; i++){
                max = std::max(max, cd.tam(i));
            }
        }

        int maxima() const{
            return max;
        }

    private: 
        int max;
};

int main(){
    int n;
    std::cin >> n;
    while(n--){
        MaximaComponenteConexa mcc;
        std::cout << mcc.maxima() << "\n";
    }
}