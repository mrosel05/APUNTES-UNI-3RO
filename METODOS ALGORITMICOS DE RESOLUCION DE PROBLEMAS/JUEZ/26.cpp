#include <iostream>
#include <vector>
#include <climits>
#include "DigrafoValorado.h"
#include "IndexPQ.h"

class PagWeb{
public: 
    PagWeb (DigrafoValorado<int> const& gv, int origen, std::vector<int> const& destinos): 
        origen(origen), dist(gv.V(), std::vector<int> (2, INF)), total(0), camino(true){
        dijkstra(gv, 0);
        dijkstra(gv.inverso(), 1);
        calcularTotal(destinos);
    }

    bool hayCamino(){
        return camino;
    }

    int caminoTotal(){
        return total;
    }

private:
    const int INF = INT_MAX;
    bool camino;
    int origen, total;
    std::vector<std::vector<int>> dist;

    void dijkstra(DigrafoValorado<int> const& gv, int ida){
        IndexPQ<int> pq(gv.V());
        dist[origen][ida] = 0;
        pq.push(origen, 0);
        while(!pq.empty()){
            int v = pq.top().elem; pq.pop();
            for(auto a: gv.ady(v)){
                relajar(a, ida, pq);
            }
        }
    }

    void relajar(AristaDirigida<int> const& a, int ida, IndexPQ<int> & pq){
        int v = a.desde(), w = a.hasta();
        if(dist[w][ida] > dist[v][ida] + a.valor()){
            dist[w][ida] = dist[v][ida] + a.valor();
            pq.update(w, dist[w][ida]);
        }
    }

    void calcularTotal(std::vector<int> const& destinos){
        for(int i: destinos){
            if(dist[i][0] == INF || dist[i][1] == INF){
                camino = false;
                return;
            } 
            else total += dist[i][0] + dist[i][1];
        } 
    }

};

int main(){
    int n, N, C, v1, v2, longitud, O, P;
    std::cin >> n;
    while(n--){
        std::cin >> N >> C;
        DigrafoValorado<int> gv(N);
        while(C--){
            std::cin >> v1 >> v2 >> longitud;
            gv.ponArista({v1 - 1, v2 - 1, longitud});
        }
        std::cin >> O >> P;
        std::vector<int> destinos;
        while(P--){
            std::cin >> v2;
            destinos.push_back(v2 - 1);
        }
        PagWeb pw(gv, O - 1, destinos);
        if(!pw.hayCamino()) std::cout << "Imposible\n";
        else std::cout << pw.caminoTotal() << "\n";
    }
    return 0;
}
