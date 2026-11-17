#include <iostream>
#include <queue>
#include <vector>
#include "Grafo.h"

class BfsTTL{
public: 
    BfsTTL (Grafo const& g, int origen, int grado): visit(g.V(), false), 
            ant(g.V()), dist(g.V()), origen(origen), grado(grado), visitados(0){
        bfs(g);
    }

    int unreachable() const{
        return visit.size() - visitados;
    }

private: 
    void bfs(Grafo const& g){
        std::queue<int> q;
        visit[origen] = true;
        visitados++;
        dist[origen] = 0;
        q.push(origen);
        while(!q.empty()){
            int v = q.front(); q.pop();
            if(dist[v] < grado){
                for(int w: g.ady(v)){
                    if(!visit[w]){
                        ant[w] = v; 
                        visit[w] = true;
                        visitados++;
                        dist[w] = dist[v] + 1;
                        q.push(w);
                    }
                }
            }
            
        }
    }

    std::vector<bool> visit;
    std::vector<int> ant;
    std::vector<int> dist;
    int visitados;
    int origen;
    int grado;

};

int main(){
    int n, consultas, origen, grado;
    std::cin >> n;
    while(n--){
        Grafo f(std::cin, 1);
        std::cin >> consultas;
        while(consultas--){
            std::cin >> origen >> grado;
            BfsTTL b(f, origen - 1, grado);
            std::cout << b.unreachable() << "\n";
        }
        std::cout << "---\n";
    }
    return 0;
}