#include <iostream>
#include <vector>
#include <queue>
#include "GrafoValorado.h"

class HayCamino{
public: 
    HayCamino(GrafoValorado<int> const& gv, int i1, int i2, int anch):
        origen(i1), destino(i2), anchura(anch), visit(gv.V(), false),
        hayCamino(false){
    buscarCamino(gv);
    }

    bool sol() const{
        return hayCamino;
    }

private: 
    bool hayCamino;
    int origen, destino, anchura;
    std::vector <bool> visit;


    void buscarCamino(GrafoValorado<int> const& gv){
        std::queue<int> q;
        visit[origen] = true;
        q.push(origen);

        while(!q.empty()){
            int v = q.front(); q.pop();
            for(auto calle: gv.ady(v)){
                int w = calle.otro(v);
                if(visit[w] || calle.valor() < anchura) continue;
                if(w == destino){
                    hayCamino = true;
                    return;
                }
                visit[w] = true;
                q.push(w);
            }
        }
    }



};

int main(){
    int n, V, E, i1, i2, anchura, consultas;
    std::cin >> n;
    while(n--){
        std::cin >> V >> E;
        GrafoValorado<int> gv (V);
        while(E--){
            std::cin >> i1 >> i2 >> anchura;
            gv.ponArista({i1 - 1, i2 - 1, anchura});
        }
        std::cin >> consultas;
        while(consultas--){
            std::cin >> i1 >> i2 >> anchura;
            HayCamino hc(gv, i1 - 1, i2 - 1, anchura);
            std::cout << (hc.sol() ? "SI\n":"NO\n");
        }
    }

    return 0;
}