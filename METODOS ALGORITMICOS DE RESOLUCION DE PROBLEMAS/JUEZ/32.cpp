#include <iostream>
#include <algorithm>
#include <vector>

int sol(std::vector<int> const& pilas, int V){
    int coches = 0, pilaBaja = 0, pilaAlta = pilas.size() - 1;
    while(pilaBaja < pilaAlta){
        if(pilas[pilaBaja] + pilas[pilaAlta] >= V){
            coches++; pilaAlta--;
        }
        pilaBaja++;
    }
    return coches;
}

int main(){ 
    int n, N, V;
    std::cin >> n;
    while(n--){
        std::cin >> N >> V;
        std::vector <int> pilas(N);
        for(int& i: pilas) std::cin >> i;
        std::sort(pilas.begin(), pilas.end());
        std::cout << sol(pilas, V) << "\n";
    }
    return 0;
}