#include <iostream>
#include <deque> 
#include "Digrafo.h"

/*
Usaremos el algorimo de ordenación topológica y simultaneamente comprobando que no hay ciclos. 
La solución se va acumulando en el vector orden, pero si hay ciclos será 
inválido.  
*/

class Tareas{
public: 
    Tareas (Digrafo const& dg): visit(dg.V(), false), apilado(dg.V(), false), ciclo(false){
        for(int i = 0; i < dg.V(); i++) 
            if(!visit[i] && !ciclo) dfs(dg, i);
    }

    bool hayCiclo() const{
        return ciclo;
    }

    std::deque<int> getOrden() const{
        return orden;
    }


private: 
    std::vector<bool> visit;
    std::vector<bool> apilado;
    std::deque<int> orden;
    bool ciclo;

    void dfs(Digrafo const& dg, int v){
        apilado[v] = visit[v] = true;
        for(int w: dg.ady(v)){
            if(ciclo) return;

            if(!visit[w]) dfs(dg,w);
            else if(apilado[w]) ciclo = true;
        }
        apilado[v] = false;
        orden.push_front(v);
    }
};

int main(){
    int n;
    std::cin >> n;
    while(n--){
        Digrafo dg(std::cin, 1);
        Tareas t(dg);
        if(t.hayCiclo()) std::cout << "Imposible\n";
        else {
            auto orden = t.getOrden();
            std::cout <<  orden[0] + 1;
            for(int i = 1; i < orden.size(); i++){
                std::cout << " " << orden[i] + 1;
            }
            std::cout << "\n";
        }
    }

    return 0;
}