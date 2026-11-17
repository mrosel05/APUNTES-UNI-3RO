#include <iostream>
#include <queue>
#include <vector>

/*
Tenemos que hacer busqueda bfs desde el nodo origen hasta el final y contar
el camino. 
Realmente no necesitamos guardar un grafo. Vamos a simularlo mediante 
las tres operaciones (+1), (*2) y (/3) como si fueran aristas que nos 
llevan a 3 nuevos grafos.

Como el espacio de vectores esta acotado en 10000, podemos llevar dos vectores
de visitados y dist. 
*/

const int MAX = 10000;

class BfsCalculator{
public: 
    BfsCalculator (int origen, int destino): visit(MAX, false), dist(MAX), distancia(0){
        bfs(origen, destino);
    }

    int sol(){
        return distancia;
    }

private: 
    void bfs(int origen, int destino){
        if(origen == destino){
            distancia = 0;
            return;
        }

        std::queue<int> q;
        visit[origen] = true;
        dist[origen] = 0;
        q.push(origen);
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int w: adyacentes(v)){
                if(!visit[w]){
                    if(w == destino){
                        distancia = dist[v] + 1;
                        return;
                    }
                    visit[w] = true;
                    dist[w] = dist[v] + 1;
                    q.push(w);
                }
            }
            
        }
    }

    std::vector<int> adyacentes(int v){
        std::vector<int> ady(3);
        ady[0] = (v+1)%MAX; 
        ady[1] = (v*2)%MAX; 
        ady[2] = (v/3)%MAX;
        return ady;
    }

    std::vector<bool> visit;
    std::vector<int> dist;
    int distancia;
};

int main(){
    int n, origen, destino;
    std::cin >> n;
    while(n--){
        std::cin >> origen >> destino;
        BfsCalculator c(origen, destino);
        std::cout << c.sol() << "\n";
    }
    return 0;
}