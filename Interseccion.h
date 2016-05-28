#ifndef INTERSECCION_H
#define INTERSECCION_H
#include <vector>
#include "Grafos.h"
#include "Arbol_B.h"
#include "Estructuras.h"

using namespace std;

///Dado un conjunto de puntos y los segmentos formados por esos puntos, construyo el grafo correspondiente
///aplicando el algoritmo de interseccion de la Sweep Line visto en clase.
class Interseccion {
private:
		
	vector<vertex> intersecciones; ///Vector que almacena los puntos resultantes de las intersecciones 
	
	double sweep_line; ///Altura de la Sweep Line
	
	Grafos grafo;	///Grafo final formado por los segmentos.
	
public:
	Interseccion();
	
	Grafos grafo_resultante(); ///Devuelve finalmente el grafo reusltante del conjunto de segmentos.
	
	/// S almacena el conjunto de segmentos a tratar, los puntos estan almacenados en cada segmento.
	vector<vertex> FindIntersection( vector<segmento> S );
	
	void HandleEventPoint(event_point p);//Maneja el punto de evento de Q, y controla el estado en T.
	void findNewEvent(segmento sl, segmento sr, event_point p);//sl = vecino izquierdo, sr = vecino derecho
		//reviso interseccion entre vecinos y maneja puntos de eventos en Q.
	punto calc_inter(segmento s1, segmento s2); //calculo la interseccion entre segmentos.
	
	~Interseccion();
};

#endif

