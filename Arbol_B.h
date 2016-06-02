#ifndef ARBOL_B_H
#define ARBOL_B_H
#include <cstddef>
#include <iostream>
#include <cstdlib>

///COMPLETAR ESTA CLASE TEMPLATE Y PROBARLA CON ENTEROS, LUEGO PASAR A PROBARLA CON ESTRUCTURAS QUE
///TENGAN SOBRECARGADO EL OPERADOR < ...

using namespace std;

template <typename T>

struct nodo{	//Revisar si es necesario que este constituida como una clase, o si es suficiente que sea un Struct...
	T p;
	nodo<T>* father;
	nodo<T>* left;
	nodo<T>* right;
	
//	int operator<<(nodo<T> Q){
//		return 0;
//	}
};

//Este arbol va a ser usado con Event_Points y con Segmentos, asi que ambas deberan tener sobregarcada la "Operacion <",
//el arbol la empleara como funcion fundamental para le balanceo, la insercion, el borrado y la busqueda.

//Estructura de Arbol Balanceado AVL, resolver situacion para los segmentos(requiera para su ordenamiento la altura de la sweep line)
//revisar el template

//Para ser usada, tanto para la estrctura Q, que maneja puntos de Eventos, como para la estructura T, qeu controla el estado de la sweep_line.
//En casod e complejisarse, se pueden crear 2 estructuras arbol separadas, únicas o usar Set para Q como se uso antes. 
template<class T>
class Arbol_B {
private:

	nodo<T>* raiz;
	nodo<T>* actual;
	
	
public:
	
	///ARREGLAR LA CLASE ARBOL!!!!!
	
	Arbol_B();
	
	//Ya se realiza una correcta insercion y balanceo...
	nodo<T>* Insert(T ep);	//REVISAR SI NO SERIA NECESARIO RETORNAR EL AGREGADO...
	nodo<T>* Insert(T ep, nodo<T>* act);
	
	nodo<T>* Delete(T ep);
	nodo<T>* Delete(T ep, nodo<T>* act);
	
	nodo<T>* Find(T ep);	//para buscar un elemento determinado en el arbol.Util para el Delete
	nodo<T>* Find(T ep, nodo<T>* act);
	
	void FindNeighbors(T &sl, T &sr, T p);
	void FindNeighbors(T &sl, T &sr, T p, nodo<T>* act);
	
	//El mas izquierdo a partir de un nodo
	nodo<T>* leftmost(nodo<T>* l);
	//El mas derecho a partir de un nodo
	nodo<T>* rightmost(nodo<T>* r);
	
	nodo<T>* rotate_left(nodo<T>* root);
	nodo<T>* rotate_right(nodo<T>* root);
	
	//realiza el balance del arbol, para arbol AVL, chequeo la altura por izuqierda y por derecha, si difiere, debo realizar un giro en ese sentido.
	nodo<T>* balance(nodo<T>* P);
	
	bool empty();
	nodo<T>* begin();
	
	///Para mostrar el arbol al testear...
	void show();
	void show(nodo<T>* act, int nivel);
	
	~Arbol_B();
};

#endif

