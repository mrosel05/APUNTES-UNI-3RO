#include <iostream>
#include "Digrafo.h"

class Sumidero{
public:
    Sumidero(Digrafo const& dg): sum(-1){
        buscarSumidero(dg);
    }

    int sumidero() const{
        return sum;
    }

private: 
    int sum;

    void buscarSumidero(Digrafo const& dg){
        auto inverso = dg.inverso();
        for(int i = 0; i < dg.V(); i++){
            if(dg.ady(i).size() == 0 && inverso.ady(i).size() == (dg.V() - 1))
                sum = i;
        }
    }
};

int main(){
    while(true){
        Digrafo dg(std::cin);
        if(!std::cin) break;
        Sumidero s(dg);
        if(s.sumidero() == -1) std::cout << "NO\n";
        else std::cout << "SI " << s.sumidero() << "\n";
    }

    return 0;
}