#include <iostream>
#include <vector>
#include <queue>

/*
Vamos a hacer un bfs sobre una estructura de digrafo pero usando las aristas
de una manera distinta. 
Las verdaderas aristas serán simuladas, pues serán todas las caras que 
pueden salir en el dado lo que nos hará cambiar de una arista a otra. 
El digrafo con sus aristas nos servirá para que, llegados a cada casilla,
preguntemos si desde ahí nace una serpiente o una escalera y nos obliga
a tomarla. Con ello, cambiaremos el vértice actual. 
El digrafo puede ser sustituido por un simple vector donde para cada 
casilla, puede tener una posición obligatoria a la que ir, o no. 

Realmente tendremos tantos vértices como casillas en el tablero (V) y
a su vez tendremos K aristas unidireccionales por cada casilla (una por cara
del dado, hasta la casilla V - K, q no tiene tantas porque sino se sale
del tablero). Por ello tendremos (V - K)*K + (K-1) + (K-2) ... + 1 = 
(V-K)*K + K*(K-1)/2 = (V-K + (K-1)/2) * K = (2V-K-1)*K/2 aristas. 
*/

class SerpEsca{
public: 
    SerpEsca(){
        int _N, _K, S, E, v1, v2;
        std::cin >> _N >> _K >> S >> E;

        N = _N; K = _K;
        visit.assign(N*N, false);
        camino.assign(N*N, -1);
        dist.assign(N*N, -1);
        for(int i = 0; i < S+E; i++){
            std::cin >> v1 >> v2;
            camino[v1 - 1] = v2 - 1;
        }

        buscarOptimo();
    }

    int getOptimo(){
        return optimo;
    }

private:
    std::vector<int> camino;
    std::vector<bool> visit;
    std::vector<int> dist;

    int N, K;
    int optimo = -1;

    void buscarOptimo(){
        std::queue<int> q;
        visit[0] = true; dist[0] = 0;
        q.push(0);
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int i = 1; i <= K; i++){
                int w = v + i;
                if(w >= N*N) continue;
                if(camino[w] != -1) w = camino[w];

                if(!visit[w]){
                    visit[w] = true;
                    dist[w] = dist[v] + 1;
                    q.push(w);
                    
                    if(w == N*N-1){
                        optimo = dist[w];
                        return;
                    }
                }
            }
        }
    }



};

int main(){
    int n;
    std::cin >> n;
    while(n--){
        SerpEsca se;
        std::cout << se.getOptimo() << "\n";
    }
    return 0;
}