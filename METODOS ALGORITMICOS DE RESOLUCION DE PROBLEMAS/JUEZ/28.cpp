#include <iostream>
#include <vector>
#include <algorithm>

int sol(std::vector <int> const& enemigos, std::vector <int> const& defensa){
    int cont = 0, pEn = 0, pDef = 0;
    while(pEn < enemigos.size() && pDef < defensa.size()){
        if(defensa[pDef] >= enemigos[pEn]) {
            cont++; pEn++; pDef++;
        }
        else pDef++;
    }
    return cont;
}


int main(){
    int n, N;
    std::cin >> n;
    while(n--){
        std::cin >> N;
        std::vector <int> enemigos(N), defensa(N);
        for(int& i: enemigos) std::cin >> i;
        for(int& i: defensa) std::cin >> i;
        std::sort(enemigos.begin(), enemigos.end());
        std::sort(defensa.begin(), defensa.end());
        std::cout << sol(enemigos, defensa) << "\n";
    }
    return 0;
}