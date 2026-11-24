#include <iostream>
#include <vector>

int sol(int L, std::vector<int> const& v){
    int cont = 1, init = v[0];
    for(int i = 1; i < v.size(); i++){
        if(init + L < v[i]){
            cont++;
            init = v[i];
        }
    }
    return cont;
}

int main(){
    int n, N, L;
    std::cin >> n;
    while(n--){
        std::cin >> N >> L;
        std::vector<int> v(N); 
        for(int& i: v) std::cin >> i;
        std::cout << sol(L, v) << "\n";
    }
    return 0;
}