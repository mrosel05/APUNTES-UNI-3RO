#include <iostream>
#include <vector>
#include "PriorityQueue.h"
#include "ConjuntosDisjuntos.h"

struct Arista{
    int v1, v2, precio;
    bool operator< (Arista const& a) const{
        return precio < a.precio;
    }
};

class MenorPrecio{
public: 
    MenorPrecio(int I, std::vector<Arista> const& puentes): precio(0){
        calcular(I, puentes);
    }

    int sol(){
        return precio;
    }

private: 
    int precio;

    void calcular(int I, std::vector<Arista> const& puentes){
        int contPuentes = 0;
        ConjuntosDisjuntos cd(I);
        PriorityQueue<Arista> pq(puentes);
        while(!pq.empty()){
            auto a = pq.top(); pq.pop();
            int v = a.v1, w = a.v2;
            if(!cd.unidos(v, w)){
                cd.unir(v, w);
                contPuentes++;
                precio += a.precio;
                if(contPuentes == I - 1) return;
            }
        }
        if(contPuentes == I - 1) return;
        precio = -1;
    }
};

int main(){
    int n, I, P, v1, v2, precio;
    std::cin >> n;
    while(n--){
        std::cin >> I >> P;
        std::vector<Arista> puentes;
        while(P--){
            std::cin >> v1 >> v2 >> precio;
            puentes.push_back({v1-1, v2-1, precio});
        }
        MenorPrecio mp (I, puentes);
        if(mp.sol() == -1) std::cout << "No hay puentes suficientes\n";
        else std::cout << mp.sol() << "\n";
    }

}