#include <iostream>
#include <vector>
#include "IndexPQ_gen.h"

struct tData {
    long long votos, escaños;
    int id;

    bool operator() (tData const& d1, tData const& d2) const {
        long long prod1 = d1.votos * (d2.escaños + 1);
        long long prod2 = d2.votos * (d1.escaños + 1);

        if (prod1 != prod2) {
            return prod1 > prod2;
        }
        if (d1.votos != d2.votos) {
            return d1.votos > d2.votos;
        }
        return d1.id < d2.id;
    }
};

void sol(long long C, long long N){
    if(C == 0){
        std::cout << "\n";
        return;
    } 

    IndexPQ<int, tData, tData> votos;
    for(int i = 0; i < C; i++){
        long long v; 
        std::cin >> v;
        votos.push(i, {v, 0, i});
    }

    while(N--){
        auto v = votos.top();
        v.prioridad.escaños++;
        votos.update(v.elem, v.prioridad);
    }
    
    std::vector<long long> sol(C, 0);
    while(!votos.empty()){
        auto v = votos.top(); 
        votos.pop();
        sol[v.elem] = v.prioridad.escaños;
    }
    
    for(int i = 0; i < sol.size(); i++){
        if (i > 0) std::cout << ' ';
        std::cout << sol[i];
    }
    std::cout << "\n";
}

int main(){
    long long C, N;
    if (std::cin >> C >> N) {
        while(N != 0 || C != 0){
            sol(C, N);
            std::cin >> C >> N;
        }
    }
    return 0;
}