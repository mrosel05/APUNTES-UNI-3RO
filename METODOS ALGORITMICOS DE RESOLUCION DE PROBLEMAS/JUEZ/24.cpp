#include <iostream>
#include <vector>
#include <climits>
#include "GrafoValorado.h"
#include "IndexPQ.h"

class NumeroCaminos{
public: 
    NumeroCaminos (GrafoValorado<int> const& gv): origen(0), destino(gv.V() - 1),
            dist(gv.V(), INF), pq(gv.V()), contCaminos(gv.V(), 0){
        dijkstra(gv);
    }

    int numCaminos(){
        return contCaminos[destino];
    }

private:
    const int INF = INT_MAX;
    int origen, destino;
    std::vector<int> contCaminos;
    std::vector<int> dist;
    IndexPQ<int> pq;

    void dijkstra(GrafoValorado<int> const& gv){
        dist[origen] = 0;
        contCaminos[origen] = 1;
        pq.push(origen, 0);
        while(!pq.empty()){
            int v = pq.top().elem; pq.pop();
            for(auto a: gv.ady(v)){
                relajar(a, v);
            }
        }
    }

    void relajar(Arista<int> const& a, int v){
        int w = a.otro(v);
        if(dist[w] > dist[v] + a.valor()){
            dist[w] = dist[v] + a.valor();
            pq.update(w, dist[w]);
            contCaminos[w] = contCaminos[v];
        }
        else if (dist[w] == dist[v] + a.valor()) contCaminos[w] += contCaminos[v];
    }

};

int main(){
    int n, N, C, v1, v2, longitud;
    std::cin >> n;
    while(n--){
        std::cin >> N >> C;
        GrafoValorado<int> gv(N);
        while(C--){
            std::cin >> v1 >> v2 >> longitud;
            gv.ponArista({v1 - 1, v2 - 1, longitud});
        }
        NumeroCaminos mc(gv);
        std::cout << mc.numCaminos() << "\n";
    }
    return 0;
}
