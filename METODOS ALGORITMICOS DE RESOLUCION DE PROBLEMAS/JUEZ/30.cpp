#include <iostream>
#include <algorithm>
#include "PriorityQueue.h"

struct edif{
    int ini, fin;
    bool operator() (edif const& e1, edif const& e2){
        return e1.ini < e2.ini;
    }
};

int sol(PriorityQueue<edif, edif> & pq){
    int cont = 1;
    auto act = pq.top(); pq.pop();
    int ini = act.ini, fin = act.fin;
    while(!pq.empty()){
        auto aux = pq.top(); pq.pop();
        if(aux.ini >= fin){
            cont++;
            ini = aux.ini;
            fin = aux.fin;
        }
        else{
            ini = std::max(ini, aux.ini);
            fin = std::min(fin, aux.fin);
        }
    }
    return cont;
}

int main(){ 
    int n, N, ini, fin;
    std::cin >> n;
    while(n--){
        std::cin >> N;
        PriorityQueue<edif, edif> pq;
        while(N--){
            std::cin >> ini >> fin;
            pq.push({ini, fin});
        }
        std::cout << sol(pq) << "\n";
    }
    return 0;
}