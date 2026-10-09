#include <iostream>
#include <climits>
#include <algorithm>
#include "bintree_eda.h"

/*
EXPLICACIÓN: para comprobar si se cumple recursivamente que sea un árbol de búsqueda 
(los hijos derechos son mayores que la raiz y los de la izquierda son menores) y además
equilibrado (diferencia de alturas entre hijos no mayor que uno) haremos una llamada a la función avl. 
Esta nos dira 4 cosas: el valor minimo de ese árbol, el máximo, la altura y un booleano diciendo si es
avl. Devuelve todo esto para solo tener que hacer una llamada. 
En caso de que el derecho sea avl, hacemos la llamada en la izquierda también. 
Los datos que nos han devuelto las llamadas los usamos para comprobar tres cosas: 
+ Que el elemento máximo de la izquierda (izq.max) sea menor que la raíz. 
+ Que el elemento mínimo de la derecha (der.min) sea mayor que la raíz. 
+ Que la diferencia de alturas no supere 1. 

En caso de que alguna de estas cosas no se cumpla, devolvemos false a que es avl, dejando el resto de datos
como sea, pues no nos afectan. 

Devolvemos el nuevo minimo del arbol, el nuevo máximo, la nueva altura y un true, pues se cumple lo anterior. 

Destacamos que se podria haber gestionado el ejercicio con excepciones en lugar de un booleano, pero se ha preferido 
este enfoque por simplicidad. 



RECURRENCIA: siendo n el numero de nodos en b. 
+ En el caso peor: 
avl (n) =   c0 si n = 0. 
            avl (n/2) + avl (n/2) + c1 en otro caso. 
Debemos destacar que, en caso de que el derecho no sea equilibrado, 
directamente ni preguntamos en el izquierdo. En la recurrencia se destaca el caso 
peor, pero no siempre será así. Preguntando previamente optimizamos. 

Expansion: 
avl (n) = 2* avl (n/2) + c1. Conocemos que esto es coste O(n) por el teorema de la división
siendo n el número de nodos. 

+ En el caso mejor. Cuando la primera rama ya no es avl. 
avl (n) =   c0 si n = 0
            avl(n-1) + c1 en otro caso. 
Esto es debido a que si ya no podemos asegurar que sea avl, debemos contar con que la 
siguiente llamada coste avl(n-1)
Por el teorema de la división, esto es coste O(n) siendo n el número de nodos. 

En un caso muy específico, en que el arbol izquierdo sea pequeño y no avl, la ejecución sería
casi constante, dependiendo solo de lo que tardemos en comprobar el arbol izquierdo. 

*/

struct data{
    data(int mn, int mx, int n, bool b): min(mn), max(mx), altura(n), esAVL(b){};
    int min, max, altura;
    bool esAVL;
};

data avl(const bintree<int>& b){
    if(b.empty()) return {INT_MAX, -1, 0, true};
    else {
        data der = avl(b.right());
        if(!der.esAVL) return {INT_MAX, -1, 0, false};
        data izq = avl(b.left());

        if(!izq.esAVL || der.min <= b.root() || izq.max >= b.root() || std::abs(der.altura - izq.altura) > 1)
            return {INT_MAX, -1, 0, false};

        return {std::min({der.min, izq.min, b.root()}), std::max({der.max, izq.max, b.root()}), 
            std::max(der.altura, izq.altura) + 1, true};
    }
}

int main(){
    int n;
    std::cin >> n;
    while(n--){
        bintree b = leerArbol(-1);
        std::cout << (avl(b).esAVL ? "SI" : "NO") << '\n';
    }
    return 0;
}