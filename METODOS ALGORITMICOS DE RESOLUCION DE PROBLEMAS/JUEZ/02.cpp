#include <iostream>
#include "TreeSet_AVL_02.h"

/*
Explicación: 
Para poder encontrar el k-ésimo en tiempo logarítmico, vamos a guardar la posición que ocupa cada nodo con 
respecto a los nodos de su izquierda. Es decir, si tiene 5 nodos a la derecha, la raiz es el sexto y por ello 
guarda un 6. Este contador empieza a uno, ya que si no tiene a nadie a la izquierda es el número 1 por orden. 
Es por esto que en la función insertar, cuando se nos devuelve un booleano a true diciendo que se ha insertado
a la izquierda del nodo, añadimos uno a este contador. 
Sin embargo, en las rotaciones para equilibrar el árbol se pueden intercambiar los hijos entre sí, y por ello debemos
actualizar el num_i. 

Tenemos dos casos, asumiento que una doble rotación solo son dos rotaciones simples: 
+ Rotación a la derecha: con nodos k1 y k2, siendo k1 k2->left, vamos a pasar haciendo que k1->right sea k2, y por ello
k2->left va a ser lo que habia antes en k1->right. Para conocer el numero de nodos que habia en k1-> right, hacemos 
k2->num_i -= k1->num_1, pues k2->num_i es igual a k1->num_1 + el numero de nodos a la derecha de k1. 

+ Rotación a la izquierda: con nodos k1 y k2, siendo k2 k1->right, vamos a pasar haciendo que k2->left sea k1 y que k1->right 
pase a ser el antiguo hijo izquierdo de k2. Viendo esto, el antiguo hijo de k2 sigue estando a su izquierda y lo unico que cambia
es que los hijos de k1 y k1 pasa a estar a su izquierda. Es por ello que hacemos k2->num_i += k1->num_i, ya que se añaden estos
nodos. 

Manteniendo estas cuentas a la hora de insertar, la función se resuelve de manera sencilla. 
T const& kesimo(int k, TreeNode* a) const {
      if (a == nullptr) throw std::out_of_range("Posición inexistente");
      
      if (a->tam_i > k) return kesimo(k, a->iz);
      else if (a->tam_i == k) return a->elem;
      else return kesimo(k - a->tam_i, a->dr);
   }

Primero preguntamos si es nulo, pues esto implica que no se ha encontrado la posición o que se ha llamado con un árbol vacío.
Utilizamos uso de excepciones en vez de mandar un valor centinela, ya que está parametrizado y no podemos mandar -1. 
Entonces, si se busca un kesimo menor que el que te dice que es la raíz, vamos a la izquierda. Si es ese mismo, devolvemos
la raíz. 
El caso distinto es lo que ocurre cuando pide uno mayor. Cada nodo guarda su referencia de orden con respecto a los nodos 
a su izquierda. Por ello, si bajamos al hijo derecho buscando un orden, solo debemos buscar la diferencia con respecto al 
orden de su padre. 
Por ejemplo, si la raiz tiene un 5, es porque es el quinto elemento de izquierda a derecha. Pero si buscamos el sexto, entonces
es el elemento numero uno a la derecha de la raiz. Por eso restamos. 

En cuanto al coste, todo lo relacionado con añadir tam_i son operaciones constantes. En cuanto a la operacion kesimo: 
n = numero de nodos. k = el numero del elemento buscado. a = raiz del arbol. 
kesimo(n) =     c0 si a == nullptr o a->tam_i == k. 
                kesimo(n/2) en otro caso. 
Por el teorema de la division, esto es del orden de O(log n), siendo n el numero de nodos. 
*/

int main(){
    int n;
    std::cin >> n;
    while(n != 0){
        Set<int> s;
        int aux, m;
        for(int i = 0; i < n; i++){
            std::cin >> aux;
            s.insert(aux);
        }
        std::cin >> m;
        while(m--){
            std::cin >> aux;
            try{
                std::cout << s.kesimo(aux) << '\n';
            }
            catch (std::out_of_range){
                std::cout << "??\n";
            }
            
        }
        std::cout <<"---\n";
        std::cin >> n;
    }


}