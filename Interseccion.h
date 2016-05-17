#ifndef INTERSECCION_H
#define INTERSECCION_H
#include <vector>
#include "Grafos.h"

using namespace std;

struct punto{
	double x;
	double y;
	
};

struct segmento{
	punto ini;
	punto fin;
	
};

///Puntos de Eventos, puntos significativos a la hora de recorrer con la linea de barrido
struct event_point{
	punto p; ///Coordenada del punto.
	vector<segmento> U;	///Conjuntos de INDICES de los segmentos que comienzan en ese punto.
};

///Dado un conjunto de puntos y los segmentos formados por esos puntos, construyo el grafo correspondiente
///aplicando el algoritmo de interseccion de la Sweep Line visto en clase.
class Interseccion {
private:
	
	vector<segmento> S;///Almacena el conjunto de segmentos a tratar, los puntos estan almacenados en cada segmento.
	
	/*Cola de Eventos*/	//falta decidir la estructura para almacenar los eventos (SET  lo mas probable)
	/*Arbol de Estado*/ //y crear el arbol balanceado para los estados.
	
	vector<vertex> intersecciones; ///Vector que almacena los puntos resultantes de las intersecciones 
	
	Grafos grafo;	///Grafo final formado por los segmentos.
	
public:
	Interseccion();
	//Interseccion(/*lista de puntos, no decido se un vector<punto>, o 2 vector<double> X, Y*/, vector<segmento>);	//Tambien es una posibilidad que yo armo la lista de segmentos afuera al azar, y luego la mando aca como parametro.
	
	Grafos grafo_resultante(); ///Devuelve finalmente el grafo reusltante del conjunto de segmentos.
	
	vector<vertex> FindIntersection();//Usa el vector de segmentos de la clase.
	void HandleEventPoint(event_point p);//Maneja el punto de evento de Q, y controla el estado en T.
	void findNewEvent(segmento sl, segmento sr, event_point p);//sl = vecino izquierdo, sr = vecino derecho
		//reviso interseccion entre vecinos y maneja puntos de eventos en Q.
	punto calc_inter(segmento s1, segmento s2); //calculo la interseccion entre segmentos.
	
	~Interseccion();
};

#endif

