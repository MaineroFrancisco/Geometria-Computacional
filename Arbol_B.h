#ifndef ARBOL_B_H
#define ARBOL_B_H
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include "Interseccion.h"

///COMPLETAR ESTA CLASE TEMPLATE Y PROBARLA CON ENTEROS, LUEGO PASAR A PROBARLA CON ESTRUCTURAS QUE
///TENGAN SOBRECARGADO EL OPERADOR < ...

using namespace std;

template <typename T>

struct nodo{	//Revisar si es necesario que este constituida como una clase, o si es suficiente que sea un Struct...
	T p;
	nodo<T>* father;
	nodo<T>* left;
	nodo<T>* right;
	
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

	//double sweep_line;	//Altura de la Sweep Line. Parametro extra para usarlo con los segmentos, posibemente se termine cambiando.
	nodo<T>* raiz;
	nodo<T>* actual;
	
	
public:
	Arbol_B();
	//Arbol_B(double SL_heigth); //al momenot de usarlo con los segmentos, necesitaria un parametro extra para considerar la altura de la sweep_line;
			//si se emplea este metodo, se debera considerar en Arbol_B(), que se debe poner "sweep_line" como un valor invalido para saber cuando se trata de uno u otro, y no generar conflictos.
	
	//Ya se realiza una correcta insercion y balanceo...
	void Insert(T ep);	//REVISAR SI NO SERIA NECESARIO RETORNAR EL AGREGADO...
	nodo<T>* Insert(T ep, nodo<T>* act, nodo<T>* fact);
	
	void Delete(T ep);
	nodo<T>* Delete(T ep, nodo<T>* act, nodo<T>* elim);
	
	nodo<T>* Find(T ep);	//para buscar un elemento determinado en el arbol.Util para el Delete
	nodo<T>* Find(T ep, nodo<T>* act);
	
	//void alturaSL(double SL_heigth);	//para pasar la altura de la SL cada ves que se requiera.
	
	//UTILES PARA LA ELIMINACION
	//Busca el vecino por izquierda de un nodo, el mas derecho (rightmost) de la izquierda
	nodo<T>* left(nodo<T>* l);
	//Busca el vecino por izquierda de un nodo, el mas izquierdo (leftmost) de la derecha
	nodo<T>* right(nodo<T>* r);

	//El mas izquierdo a partir de un nodo
	nodo<T>* leftmost(nodo<T>* l);
	//El mas derecho a partir de un nodo
	nodo<T>* rightmost(nodo<T>* r);
	
	nodo<T>* rotate_left(nodo<T>* root);
	nodo<T>* rotate_right(nodo<T>* root);
	
	//realiza el balance del arbol, para arbol AVL, chequeo la altura por izuqierda y por derecha, si difiere, debo realizar un giro en ese sentido.
	nodo<T>* balance_insert(nodo<T>* P, nodo<T>* N);
	nodo<T>* balance_delete(nodo<T>* P, nodo<T>* N);
	///void balance(nodo<T>* root);
	
	///Para mostrar el arbol al testear...
	void show();
	void show(nodo<T>* act, int nivel);
	
	~Arbol_B();
};

#endif

