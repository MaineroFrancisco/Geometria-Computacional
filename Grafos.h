#ifndef GRAFOS_H
#define GRAFOS_H
#include "Estructuras.h"
#include <list>
#include <deque>

using namespace std;

//Revisar si es necesaria una clase, o un Struct con las 3 DCEL
//Un Struct posiblemente sea mas ocnveniente para hcer la intergracion con interseccion...
class Grafos {
private:
	
	deque<vertex> vertice;
	deque<halfedge> arista;
	deque<face> cara;	//siempre deberia comenzar con 1 elemento, por la cara exterior al infinito.
	
public:
	Grafos();
	~Grafos();
};

#endif

