#include <iostream>
#include "IndexPQ.h"

struct canal{
    int id, info; //para canales = nº de espectadores, para acumulado tiempo
    bool operator() (canal const& c1, canal const& c2) const {
        return c1.info > c2.info || 
            (c1.info == c2.info && c1.id < c2.id);
    }
};

void sol(){
    int D, C, N;
    std::cin >> D >> C >> N;

    IndexPQ<canal, canal> canales(C), acumulado(C);
    std::vector <int> acum (C, 0);

    for(int i = 0; i < C; i++){
        int aux; std::cin >> aux;
        canales.push(i, {i, aux});
    }

    int tiempoAnt = 0;
    while(N--){
        int t, num; std::cin >> t >> num;
        auto top = canales.top();
        acum[top.elem] += (t - tiempoAnt);
        acumulado.update(top.elem, {top.elem, acum[top.elem]});
        tiempoAnt = t;

        
        while(num--){
            int canal, actual; std::cin >> canal >> actual;
            canales.update(canal - 1, {canal - 1, actual});
        }
    }

    auto top = canales.top();
    acum[top.elem] += (D - tiempoAnt);
    acumulado.update(top.elem, {top.elem, acum[top.elem]});

    while(!acumulado.empty()){
        auto a = acumulado.top(); acumulado.pop();
        std::cout << a.elem + 1 << " " << a.prioridad.info << "\n";
    }
    std::cout << "---\n";
}

int main(){
    int n;
    std::cin >> n;
    while(n--)sol();
    return 0;
}