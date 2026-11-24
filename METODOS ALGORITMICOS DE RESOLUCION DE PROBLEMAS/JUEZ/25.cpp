#include <iostream>
#include <vector>
#include <climits>
#include "DigrafoValorado.h"
#include "IndexPQ.h"

class PagWeb{
public: 
    PagWeb (DigrafoValorado<int> const& gv, std::vector<int> const& tCarga): 
        origen(0), destino(gv.V() - 1), dist(gv.V(), INF), pq(gv.V()){
        dijkstra(gv, tCarga);
    }

    bool hayCamino(){
        return dist[destino] != INF;
    }

    int camino(){
        return dist[destino];
    }

private:
    const int INF = INT_MAX;
    int origen, destino;
    std::vector<int> dist;
    IndexPQ<int> pq;

    void dijkstra(DigrafoValorado<int> const& gv, std::vector<int> const& tCarga){
        dist[origen] = tCarga[origen];
        pq.push(origen, dist[origen]);
        while(!pq.empty()){
            int v = pq.top().elem; pq.pop();
            for(auto a: gv.ady(v)){
                relajar(a, v, tCarga);
            }
        }
    }

    void relajar(AristaDirigida<int> const& a, int v, std::vector<int> const& tCarga){
        int w = a.hasta();
        if(dist[w] > dist[v] + a.valor() + tCarga[w]){
            dist[w] = dist[v] + a.valor() + tCarga[w];
            pq.update(w, dist[w]);
        }
    }

};

int main(){
    int n, N, C, v1, v2, longitud;
    std::cin >> n;
    while(n--){
        std::cin >> N;
        DigrafoValorado<int> gv(N);
        std::vector<int> tCarga(N);
        for(int& i: tCarga) std::cin >> i;

        std::cin >> C;
        while(C--){
            std::cin >> v1 >> v2 >> longitud;
            gv.ponArista({v1 - 1, v2 - 1, longitud});
        }
        PagWeb pw(gv, tCarga);
        if(!pw.hayCamino()) std::cout << "IMPOSIBLE\n";
        else std::cout << pw.camino() << "\n";
    }
    return 0;
}
