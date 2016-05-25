#ifndef INTERSECCION_H
#define INTERSECCION_H
#include <vector>
#include "Grafos.h"
#include "Arbol_B.h"

using namespace std;

struct punto{
	double x;
	double y;
	
	//orden lexicografico para la distincion, mayor y, va a la izquierda, mismo y,
	//si x es menor va a la izquierda, en otro caso, va a la derecha.
	bool operator<(punto P){
		bool menor = false;
		
		//Este colocara los mas altos e izquierdos, ala izquierda del arbol para 
		//obtener el siguiente evento, buscando el leftmost(mas izquierdo) del arbol...
		if(this->y > P.y)
			menor = true;
		if(this->y == P.y){
			if(this->x < P.x){
				menor = true;
			}
		}
		
//		
//		//Esta parte esta al reves, si y es mayor va a la derecha, y si x es menor va a la derecha...
//		if(this->y < P.y)
//			menor = true;
//		if(this->y == P.y){
//			if(this->x > P.x){
//				menor = true;
//			}
//		}
		
		return menor;
	}
	
	bool operator==(punto P){
		return (this->x==P.x && this->y==P.y);
	}
};

struct segmento{
	punto ini;
	punto fin;
	
	///CARGO EN CADA SEGMENTO UN PUNTERO A LA ALTURA DE LA SWEEP_LINE, SIEMPRE ANTES DE INSERTAR O ELIMINAR DEL ARBOL...
	double* y;
	
	///VAS A TERMINAR PASANDO LA ALTURA COMO UN VALOR PARA EL SEMGENTO... MUCHO GASTO DE MEMORIA...
	
	double get_x(double altura){
		double delta_x, delta_y, m, b, X;
		
		//Considerando la ecuacion de la recta:
		//				y = m * x + b; Siendo m la pendiente de la recta, y b su coordenada de origen.
		//Deduzco a partir de los puntos conocidos inicial y final la pendiente 
		//y ordenada de origen, luego reemplazo por la altura buscada y calculo la x.
		
		delta_x = ini.x - fin.x;
		delta_y = ini.y - fin.y;
		
		///Pendiente
		m = delta_y/delta_x;
		
		///Coordenada de Origen
		b= fin.y - m*fin.x;
		
		///Calculo de X a partir de un Y conocido.
		//				(y - b) / m = x;
		X = (altura - b) / m;
		
		return X;
	}
	//Para testearlo lo ponemos en esta forma simplificada...
	//bool operator<(segmento Q, double altura){
	bool operator<(segmento Q){
		
		double error = 0.00000005;
		double altura = *(this->y);
		double A = this->get_x(altura);
		double C = Q.get_x(altura);
		
		if(C < A-error){
			return true;
		}
		else{
			if(C > A+error){
				return false;
			}
			else{
				//bajo un poco la sweep_line y reviso, solo se da en el caso de ser un punto interseccion
				*(this->y) = altura - 2*error;  
				return *this < Q;
			}
		}
		
		
	}
	
	bool operator==(segmento Q){
		return (this->ini==Q.ini && this->fin==Q.fin);
	}
	
	void show(){
		cout<< "Segmento: X: "<< this->ini.x <<"	Y: "<<this->ini.y<<endl;
	}
};

///Puntos de Eventos, puntos significativos a la hora de recorrer con la linea de barrido
struct event_point{
	punto p; ///Coordenada del punto.
	vector<segmento> U;	///Conjuntos de INDICES de los segmentos que comienzan en ese punto.
	
	//Para realizar la comparacion emplea la sobrecarga del operador "<", el cual utiliza
	//orden lexicografico, primero el de mayor y, si hay varios con la misma y, el de menor x.
	bool operator<(event_point P){
		return this->p<P.p;
	}
	
	//Sobrecarga necesaria para realizar la eliminacion de los elementos del arbol,
	//es necesaria tanto para segmentos, como para eventos, ambas aprovechan la sobrecarga
	//del mismo operador en la clase Punto.
	bool operator==(event_point P){
		return this->p==P.p;
	}
	
	void show(){
		cout<< "Punto de Evento: X: "<< this->p.x <<"	Y: "<<this->p.y<<endl;
	}
	
};

///Dado un conjunto de puntos y los segmentos formados por esos puntos, construyo el grafo correspondiente
///aplicando el algoritmo de interseccion de la Sweep Line visto en clase.
class Interseccion {
private:
	
	vector<segmento> S;///Almacena el conjunto de segmentos a tratar, los puntos estan almacenados en cada segmento.
	
	/*Cola de Eventos*/	//falta decidir la estructura para almacenar los eventos (SET  lo mas probable)
	Arbol_B<event_point> Q;
	
	/*Arbol de Estado*/ //y crear el arbol balanceado para los estados.
	Arbol_B<segmento> T;
	
	vector<vertex> intersecciones; ///Vector que almacena los puntos resultantes de las intersecciones 
	
	double sweep_line; ///Altura de la Sweep Line
	
	Grafos grafo;	///Grafo final formado por los segmentos.
	
public:
	Interseccion();
	Interseccion(vector<segmento> Seg);
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

