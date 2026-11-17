#include <iostream>
#include <vector>
#include "ConjuntosDisjuntos.h"

/*
Buscaremos la componente conexa del vertice que nos digan. 
Para ello vamos usar los conjuntos disjuntos. Almacenaremos todas las 
relaciones y daremos la componente del que nos piden. 
*/

class ComponenteConexa{
    public: 
        ComponenteConexa(){
            int N, M, g, v1, v2;
            std::cin >> N >> M;
            ConjuntosDisjuntos cd(N);
            while(M--){
                std::cin >> g;
                if(g--){
                   std::cin >> v1;
                    while(g--){
                        std::cin >> v2;
                        cd.unir(v1 - 1, v2 - 1);
                    }  
                } 
            }
            for(int i = 0; i < N; i++){
                tams.push_back(cd.tam(i));
            }
        }

        std::vector<int> const& tamanos() const{
            return tams;
        }

    private: 
        int max;
        std::vector<int> tams;
};

int main(){
    int n;
    std::cin >> n;
    while(n--){
        ComponenteConexa mcc;
        for(int i: mcc.tamanos()){
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
}