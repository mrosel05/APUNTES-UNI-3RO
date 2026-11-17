#include <iostream>
#include <algorithm>
#include "ConjuntosDisjuntos.h"

class Manchas{
public: 
    Manchas(ConjuntosDisjuntos & cd, int F, int C): F(F), C(C), v(F, std::vector<bool>(C)),
            maximoConjunto(0){
        calcularConjuntos(cd);
        mirarSolucion(cd);
    }

    int maxConj() const{
        return maximoConjunto;
    }

    void nuevaMancha(ConjuntosDisjuntos & cd, int x, int y){
        int id = x * C + y;
        v[x][y] = true;
        for(int id2: vecinos(x, y)){
            cd.unir(id, id2);
        }
        maximoConjunto = std::max(maximoConjunto, cd.tam(id));
    }

private: 
    int F, C;
    int maximoConjunto = 0;
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
        //diagonales
        if(i > 0 && j > 0 && v[i-1][j-1]) vecino.push_back((i-1)*C + (j-1));
        if(i < F - 1 && j < C - 1 && v[i+1][j+1]) vecino.push_back((i+1)*C + (j+1));
        if(i < F - 1 && j > 0 && v[i+1][j-1]) vecino.push_back((i+1)*C + (j-1));
        if(i > 0 && j < C - 1 && v[i-1][j+1]) vecino.push_back((i-1)*C + (j+1));
        return vecino;
    }

};

int main(){
    int n, F, C, nuevos, x, y;
    std::cin >> n;
    while(n--){
        std::cin >> F >> C;
        ConjuntosDisjuntos cd(F*C);
        Manchas m(cd, F, C);
        std::vector<int> soluciones;
        std::cin >> nuevos;
        soluciones.push_back(m.maxConj());
        while(nuevos--){
            std::cin >> x >> y;
            m.nuevaMancha(cd, x - 1, y - 1);
            soluciones.push_back(m.maxConj());
        }
        for(int i = 0; i < soluciones.size(); i++){
            if(i != 0) std::cout << " ";
            std::cout << soluciones[i];
        }
        std::cout << "\n";
    }
}