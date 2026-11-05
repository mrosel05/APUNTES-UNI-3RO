#include <iostream>
#include <climits>
#include "IndexPQ_gen.h"

struct priority{
    int pri, time;
    bool operator() (priority const& p1, priority const& p2) const {
        return p1.pri > p2.pri || (p1.pri == p2.pri && p1.time < p2.time);
    }
};

void sol(int n) {
    std::string op, id;
    int eventos;
    IndexPQ<std::string, priority, priority> top;
    while(n--){
        std::cin >> op;
        if(op == "C"){
            std::cin >> id >> eventos;

            int pAnt = !top.contiene(id)? 0 : top.priority(id).pri;
            top.update(id, {pAnt + eventos, n});
        }
        else if(op == "E"){
            std::cin >> id >> eventos;
            if(top.contiene(id)){
                if(top.priority(id).pri <= eventos){
                    top.update(id, {INT_MAX, 0});
                    top.pop();
                }
                else {
                    auto pAnt = top.priority(id);
                    pAnt.pri -= eventos;
                    top.update(id, pAnt);
                }
            }
        }
        else {
            int cont = 1;
            std::vector<std::pair<std::string, priority>> aux;
            while(!top.empty() && cont <= 3){
                std::cout << cont++ << " " << top.top().elem << "\n";
                aux.push_back({top.top().elem, top.top().prioridad}); top.pop();
            }
            for(auto par: aux) top.push(par.first, par.second);
        }
    }
    std::cout << "---\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    while(std::cin >> n){
        sol(n);
    }
    return 0;
}