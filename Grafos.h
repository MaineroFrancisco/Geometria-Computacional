#ifndef GRAFOS_H
#define GRAFOS_H

struct vertex{
	double x;
	double y;
	halfedge* incidente;
};

struct halfedge{
	vertex* origen;
	face* incidente;
	
	halfedge* gemela;
	halfedge* anterior;
	halfedge* siguiente;
};

struct face{
	halfedge* exterior;
	list<halfedge*> f_interior;
};

//Revisar si es necesaria una clase, o un Struct con las 3 DCEL
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

