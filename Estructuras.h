#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <iostream>
#include <vector>
#include <list>

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
		
		return menor;
	}
	
	bool operator==(punto P){
		
		double error = 0.000000005;
		bool up = this->x<=P.x + error  && this->y<=P.y + error;
		bool down = this->x>=P.x - error  && this->y>=P.y - error;
		
		return up&&down;
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
				///Posiblemente pueda sacar esto, revisar el HandleEventPoint...
				//bajo un poco la sweep_line y reviso, solo se da en el caso de ser un punto interseccion
				*(this->y) = altura - 2*error;  
				return *this < Q;
			}
		}
		
		
	}
	
	bool contains(punto p){
		bool cont;
		double error = 0.000000005;
		double denom = fin.x-ini.x;
		double alpha, suma;
		if(!denom){
			alpha = (p.y-ini.y)/(fin.y - ini.y);
			suma = (1-alpha)*ini.x + (alpha)*fin.x;
			cont = ((suma <= p.x+error)&&(suma >= p.x-error))&&(alpha>0&&alpha<1);
		}
		else{
			alpha = (p.x-ini.x)/denom;
			suma = (1-alpha)*ini.y + (alpha)*fin.y;
			cont = ((suma <= p.y+error)&&(suma>=p.y-error))&&(alpha>=0&&alpha<=1);
		}
		
		return cont;
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
	vector<int> U;	///Conjuntos de INDICES de los segmentos que comienzan en ese punto.
	
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

//------------------------------------------------------------------------------------------------------------
struct halfedge;
struct face;

struct vertex{
	punto p;
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


#endif
