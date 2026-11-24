#include <iostream>
#include <algorithm>
#include "PriorityQueue.h"

struct trab{
    int ini, fin;
    bool operator() (trab const& e1, trab const& e2){
        return e1.ini < e2.ini;
    }
};

int sol(PriorityQueue<trab, trab> & pq, int C, int F){
    int cont = 0;
    while(C < F){
        int maxFin = C;
        while(!pq.empty() && pq.top().ini <= C){
            maxFin = std::max(maxFin, pq.top().fin);
            pq.pop();
        }
        if(maxFin == C) return 0;

        C = maxFin;
        cont++;
    }
    return cont;
}

int main(){ 
    int n, C, F, N, ini, fin;
    std::cin >> n;
    while(n--){
        std::cin >> C >> F >> N;
        PriorityQueue<trab, trab> pq;
        while(N--){
            std::cin >> ini >> fin;
            pq.push({ini, fin});
        }
        int solucion = sol(pq, C, F);
        if(solucion == 0) std::cout << "Imposible\n";
        else std::cout << solucion << "\n";
    }
    return 0;
}