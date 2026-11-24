#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include "GrafoValorado.h"
#include "IndexPQ.h"

class MejorCamino{
public: 
    MejorCamino (GrafoValorado<int> const& gv, int v1, int v2): origen(v1), destino(v2),
            dist(gv.V(), INF), saltos(gv.V(), INF), minSaltos(gv.V()), pq(gv.V()),
            visit(gv.V(), false){
        dijkstra(gv);
        bfs(gv);
    }

    bool hayCamino(){
        return dist[destino] != INF;
    }

    int distMin(){
        return dist[destino];
    }

    bool minCalles(){
        return saltos[destino] == minSaltos[destino];
    }

private:
    const int INF = INT_MAX;
    int origen, destino;
    std::vector<int> dist;
    std::vector<int> saltos;
    std::vector<int> minSaltos;
    std::vector<bool> visit;
    IndexPQ<int> pq;

    void dijkstra(GrafoValorado<int> const& gv){
        saltos[origen] = dist[origen] = 0;
        pq.push(origen, 0);
        while(!pq.empty()){
            int v = pq.top().elem; pq.pop();
            if(v == destino) return;
            for(auto a: gv.ady(v)){
                relajar(a, v);
            }
        }
    }

    void bfs(GrafoValorado<int> const& gv){
        std::queue<int> q;
        minSaltos[origen] = 0; visit[origen] = true;
        q.push(origen);
        while(!q.empty()){
            int v = q.front(); q.pop();
            if(v == destino) return;
            for(auto a : gv.ady(v)){
                int w = a.otro(v);
                if(!visit[w]){
                    minSaltos[w] = minSaltos[v] + 1;
                    visit[w] = true;
                    q.push(w);
                }
                
            }
        }
    }

    void relajar(Arista<int> const& a, int v){
        int w = a.otro(v);
        if(dist[w] > dist[v] + a.valor()){
            dist[w] = dist[v] + a.valor();
            saltos[w] = saltos[v] + 1;
            pq.update(w, dist[w]);
        }
        else if (dist[w] == dist[v] + a.valor())
            saltos[w] = std::min(saltos[w], saltos[v] + 1);
    }

};

int main(){
    int n, N, C, v1, v2, longitud, K;
    std::cin >> n;
    while(n--){
        std::cin >> N >> C;
        GrafoValorado<int> gv(N);
        while(C--){
            std::cin >> v1 >> v2 >> longitud;
            gv.ponArista({v1 - 1, v2 - 1, longitud});
        }
        std::cin >> K;
        while(K--){
            std::cin >> v1 >> v2;
            MejorCamino mc(gv, v1 - 1, v2 - 1);
            if(!mc.hayCamino()) std::cout << "SIN CAMINO\n";
            else std::cout << mc.distMin() << " " << (mc.minCalles() ? "SI\n" : "NO\n");
        }
        std::cout << "---\n";
    }
    return 0;
}
