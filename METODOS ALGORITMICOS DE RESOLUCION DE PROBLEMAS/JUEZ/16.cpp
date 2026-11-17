#include <iostream>
#include <algorithm>
#include "ConjuntosDisjuntos.h"

class Manchas{
public: 
    Manchas(ConjuntosDisjuntos & cd, int F, int C): F(F), C(C), v(F, std::vector<bool>(C)),
            maximoConjunto(0), numConjuntos(0){
        calcularConjuntos(cd);
        mirarSolucion(cd);
    }

    int numConj() const{
        return numConjuntos;
    }

    int maxConj() const{
        return maximoConjunto;
    }

private: 
    int F, C;
    int numConjuntos, maximoConjunto = 0;
    std::vector<std::vector<bool>> v; 

    void calcularConjuntos(ConjuntosDisjuntos & cd){
        char aux;
        for(int i = 0; i < F; i++){
            for(int j = 0; j < C; j++){
                std::cin >> aux;
                v[i][j] = aux == '#';
            }
        }

        for(int i = 0; i < F; i++){
            for(int j = 0; j < C; j++){
                if(!v[i][j]) continue;
                int id = i * C + j;
                for(int id2: vecinos(i, j)){
                    cd.unir(id, id2);
                }
            }
        }
    }

    void mirarSolucion(ConjuntosDisjuntos const& cd){
        for(int i = 0; i < F; i++){
            for(int j = 0; j < C; j++){
                if(!v[i][j]) continue;

                int id = i*C+j;
                if(cd.buscar(id) == id){
                    numConjuntos++;
                    maximoConjunto = std::max(maximoConjunto, cd.tam(id));
                }
            }
        }
    }

    std::vector<int> vecinos(int i, int j){
        std::vector <int> vecino;
        if(i > 0 && v[i-1][j]) vecino.push_back((i-1)*C + j);
        if(i < F - 1 && v[i+1][j]) vecino.push_back((i+1)*C + j);
        if(j > 0 && v[i][j-1]) vecino.push_back(i*C + (j-1));
        if(j < C - 1 && v[i][j+1]) vecino.push_back(i*C + (j+1));
        return vecino;
    }

};

int main(){
    int n, F, C; 
    std::cin >> n;
    while(n--){
        std::cin >> F >> C;
        ConjuntosDisjuntos cd(F*C);
        Manchas m(cd, F, C);
        std::cout << m.numConj() << " " << m.maxConj() << "\n";
    }
}