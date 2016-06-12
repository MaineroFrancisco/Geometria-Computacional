#ifndef INTERSECCION_H
#define INTERSECCION_H
#include <vector>
#include "Arbol_B.h"
#include "Estructuras.h"
#include <cmath>

using namespace std;

///Dado un conjunto de puntos y los segmentos formados por esos puntos, construyo el grafo correspondiente
///aplicando el algoritmo de interseccion de la Sweep Line visto en clase.
class Interseccion {
private:
	
	vector<punto> intersecciones; ///Vector que almacena los puntos resultantes de las intersecciones 
	
	double sweep_line; ///Altura de la Sweep Line
	
	/*Cola de Eventos*/
	Arbol_B<event_point> Q;
	
	/*Arbol de Estado*/
	Arbol_B<segmento> T;
	
	Grafos grafo;	///Grafo final formado por los segmentos.
	
public:
	Interseccion();
	
	Grafos* grafo_resultante(); ///Devuelve finalmente el grafo resultante del conjunto de segmentos.
	
	void InitializeQ(vector<segmento> S);
	
	void FindIntersection( vector<segmento> S );
	
	void HandleEventPoint(event_point p);
	void findNewEvent(segmento sl, segmento sr, event_point p);
	
	bool calc_inter(segmento s1, segmento s2, event_point &p);
	
	void FindNeighbors( nodo<segmento>* &sl, nodo<segmento>* &sr, segmento p, nodo<segmento>* act);
	
	
	segmento LeftmostSegment(vector<segmento> mayor, vector<segmento> menor);
	segmento RightmostSegment(vector<segmento> mayor, vector<segmento> menor);
	
	vector<punto> GetIntersection();
	
	~Interseccion();
};

#endif
