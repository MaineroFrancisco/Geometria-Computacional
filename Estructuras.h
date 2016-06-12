#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <cmath>

using namespace std;

struct vertex;
struct halfedge;
struct face;

//------------------------------------------------------------------------------------------------------------

struct punto{
	double x;
	double y;
	
	//orden lexicografico para la distincion, mayor y, va a la izquierda, mismo y,
	//si x es menor va a la izquierda, en otro caso, va a la derecha.
	bool operator<(punto P){
		bool menor = false;
		
		double error = 1e-8;
		
		//Este colocara los mas altos e izquierdos, ala izquierda del arbol para 
		//obtener el siguiente evento, buscando el leftmost(mas izquierdo) del arbol...
		if(this->y > P.y + error)
			menor = true;
		if(fabs(this->y - P.y)<= error){
			if(this->x < P.x - error){
				menor = true;
			}
		}
		
		return menor;
	}
	
	bool operator==(punto P){
		
		double error = 1e-8;
		
		return fabs(this->x - P.x) <= error && fabs(this->y - P.y) <= error;
		
	}
};

struct segmento{
	punto ini;
	punto fin;
	
	halfedge* arista;
	
	double* y;
	
	double get_x(double altura){
		
		if(ini.x==fin.x){
			return ini.x;
		}
		
		double delta_x, delta_y, m, b, X;
		
		//Considerando la ecuacion de la recta:
//				y = m * x + b; Siendo m la pendiente de la recta, y b su coordenada de origen.
		//Deduzco a partir de los puntos conocidos inicial y final la pendiente 
		//y ordenada de origen, luego reemplazo por la altura buscada y calculo la x.
		
		delta_x = ini.x - fin.x;	//Se caga para segmentos veritcales, arreblar...
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
		
		///ARREGLADO PARCIAL, EL BIAS DEL ERROR ME ESTABA PROVOCANDO PROBLEMAS, REVISAR PORQUE...
		double error = 1e-15;
		
		double altura = *y;
		double A = get_x(altura);
		double C = Q.get_x(altura);
		
		if(C < A-error){
			return true;
		}
		else{
			if(C > A+error){
				return false;
			}
			else{
				A = get_x(altura-1);
				C = Q.get_x(altura-1);
				if(C < A - error) return true;
				else 
					return false;
			}
		}
		
	}
	
	bool contains(punto p){
		bool cont;
		double error = 1e-8;
		double denom = fin.x-ini.x;
		double alpha, suma;
		if(!denom){
			alpha = (p.y-ini.y)/(fin.y - ini.y);
			suma = (1-alpha)*ini.x + (alpha)*fin.x;
			cont = fabs(suma - p.x) <= error && alpha>0 && alpha<1;
		}
		else{
			alpha = (p.x-ini.x)/denom;
			suma = (1-alpha)*ini.y + (alpha)*fin.y;
			cont = fabs(suma - p.y )<= error && alpha>0 && alpha<1;
		}
		
		return cont;
	}
	
	bool operator==(segmento Q){
		return (this->ini==Q.ini && this->fin==Q.fin);
	}
	
	void show(){
		cout<< "Segmento: X: "<< this->get_x(*this->y)<<"	Y: "<<*this->y<<endl;
	}
};


struct event_point{
	punto p; ///Coordenada del punto.
	vector<segmento> U;	///Conjuntos de INDICES de los segmentos que comienzan en ese punto.
	
	bool operator<(event_point P){
		return this->p<P.p;
	}
	
	bool operator==(event_point P){
		return this->p==P.p;
	}
	
	void show(){
		cout<< "Punto de Evento: X: "<< this->p.x <<"	Y: "<<this->p.y<<endl;
	}
	
};

//------------------------------------------------------------------------------------------------------------
struct vertex{
	punto p;
	halfedge* incidente;
	
//	vector<halfedge*> incidentes;
//	
//	void add_halfedge(halfedge *he) {
//		incidentes.push_back(he);
//	}
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

//------------------------------------------------------------------------------------------------------------

struct Grafos{
	
	deque<vertex> vertice;
	deque<halfedge> arista;
	deque<face> cara;	//siempre deberia comenzar con 1 elemento, por la cara exterior al infinito.
	
};

//------------------------------------------------------------------------------------------------------------
#endif
