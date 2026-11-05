#include <iostream>
#include <algorithm>
#include "PriorityQueue.h"

void sol(int x, int n){
    PriorityQueue mayores;
    PriorityQueue <int, std::greater<int>> menores;
    menores.push(x);
    while(n--){
        int aux1, aux2;
        std::cin >> aux1 >> aux2;
        if(aux1 > menores.top()) mayores.push(aux1);
        else menores.push(aux1);

        if(aux2 > menores.top()) mayores.push(aux2);
        else menores.push(aux2);

        if(menores.size() - 1 > mayores.size()){
            mayores.push(menores.top());
            menores.pop();
        } else if (menores.size() - 1 < mayores.size()){
            menores.push(mayores.top());
            mayores.pop();
        }

        std::cout << menores.top() << " ";
    }
    std::cout << "\n";
}

int main(){
    int x, n;
    std::cin >> x >> n;
    while(n != 0 && x != 0){
        sol(x, n);
        std::cin >> x >> n;
    }
    return 0;
}