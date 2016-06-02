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
	
	Grafos grafo_resultante(); ///Devuelve finalmente el grafo resultante del conjunto de segmentos.
	
	/// S almacena el conjunto de segmentos a tratar, los puntos estan almacenados en cada segmento.
	void FindIntersection( vector<segmento> S );
	
	void HandleEventPoint(event_point p);//Maneja el punto de evento de Q, y controla el estado en T.
	void findNewEvent(segmento sl, segmento sr, event_point p);//sl = vecino izquierdo, sr = vecino derecho
		//reviso interseccion entre vecinos y maneja puntos de eventos en Q.
	bool calc_inter(segmento s1, segmento s2, event_point &p); //calculo la interseccion entre segmentos.
	
	///Pasos para la convercion a grafo.
	
	///Dado un conjunto de segmentos, devuelve los mismos traducidos a una estructura de grapo, para
	///luego hacer la correccion de este grafo con las intersecciones encontradas.
	//El resultado es almacenado en el componente "grafo" de la clase.
	
	//Cada segmento debera tener una referencia a su halfedge corespondiente, ma medida que se intersectan
	//los segmentos se vana actualizando las referencias de los segmentos a los halfedge para reflejar 
	//las aristas por debajo de la sweep_line que aun pueden ser intersectadas.
	//Se continua con el supuesto central de que todo lo que esta por encimad e la sweep_line ya fue 
	//correctamente tratado.
	
	void GraphConverter( segmento S );
	
	segmento LeftmostSegment(vector<segmento> mayor, vector<segmento> menor);
	segmento RightmostSegment(vector<segmento> mayor, vector<segmento> menor);
	///Funcion de Correccion...
	
	vector<punto> GetIntersection();
	
	~Interseccion();
};

#endif

