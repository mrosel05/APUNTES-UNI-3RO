#include <iostream>
#include <vector>
#include "Grafo.h"

/*
Buscamos comprobar que sea aciclico y conexo. 
Para comprobar si es conexo, haremos dfs una sola vez y comprobaremos 
que hemos llegado a todos los vértices. 
Si durante el dfs, tocamos un vértice que ya se haya tocado, devuelve false. 

Empezamos todos los dfs desde el 0, ya que el enunciado nos asegura que al menos 
habrá un vértice. 

El coste sabemos teóricamente que es O(V + A).
*/

class ArbolLibre{
    public: 
        ArbolLibre(Grafo const& g): visit(g.V(), false), ant(g.V(), -1), ciclo(false){
            dfs(g, 0);
        }

        bool esLibre() const{
            if(ciclo) return false;
            for(bool b: visit) if(!b) return false;
            return true;
        }

    private: 
        std::vector <bool> visit;
        std::vector <int> ant;
        bool ciclo;

        void dfs(Grafo const& g, int v){
            visit[v] = true;
            for(int w: g.ady(v)){
                if(ciclo) return;
                if(w == ant[v]) continue;
                if(visit[w]){ciclo = true; return;}
                ant[w] = v;
                dfs(g, w);
            }
        }
};

int main(){
    int n;
    std::cin >> n;
    while(n--){
        Grafo g(std::cin);
        ArbolLibre al(g);
        std::cout << (al.esLibre() ? "SI\n" : "NO\n");
    }
    


}