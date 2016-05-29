#include "Interseccion.h"

Interseccion::Interseccion() {
	
}

Interseccion::~Interseccion() {
	
}

Grafos Interseccion::grafo_resultante ( ) {
	
}

vector<vertex> Interseccion::FindIntersection (vector<segmento> S ) {
	
	///1) Inicializar Q el arbol que contendra los event_point
	
	event_point ini_aux, fin_aux;
	
	///Agregar los puntos de los segmentos en S como puntos de evento en Q
	int n = S.size();
	for (int i=0; i<n;i++){
		
		GraphConverter(S[i]);
		
		///Como las aristas siempre se insertan al final, hago uso de eso 
		///para asignar los halfedge correspondientes a cada segmento.
		
		//Señala a la arista izquierda, la que corresponderia al interior, puedo moverme a 
		//cualquiera haciendo uso de la gemela.
		S[i].halfEdge = grafo.arista.size()-1;	
		
		ini_aux.p = S[i].ini;
		ini_aux.U.push_back(S[i]);
		
		fin_aux.p = S[i].fin;
		
		nodo<event_point>* n_aux = Q.Find(ini_aux);
		
		if(!n_aux){
			Q.Insert(ini_aux);
		}
		else{
			n_aux->p.U.push_back(S[i]);
		}
		
		n_aux = Q.Find(fin_aux);
		if(!n_aux){
			Q.Insert(fin_aux);
		}
		
	}
	
	///2) Inicializar el arbol de estados T...
	
	event_point aux;
	while(!Q.empty()){
		
		aux = Q.leftmost(Q.begin())->p;
		
		HandleEventPoint(aux);
		
		Q.Delete(aux);
		
	}
	
}

///AGREGAR FIND_LC A INTERSECCION...LO MISMO PARA LAS QUE SON EXTRA EN ARBOL...ELIMINAR LEFT Y RIGHT DE ARBOL_B
void find_LC(event_point p, vector<segmento> L, vector<segmento> C, nodo<segmento>* n){
	
	find_LC (p,L,C,n->left);
	
	if(n->p.fin == p.p){
		L.push_back(n->p);
	}
	else{
		if(n->p.contains(p.p)){
			C.push_back(n->p);
		}
	}
	
	find_LC (p,L,C,n->right);
}

///LO SIGUIENTES PASOS NO CONTEMPLAN UNA RECTA HORIZONTAL, REVISAR Y ARREGLAR...
void Interseccion::HandleEventPoint (event_point p) {
	
///------------------------------------------------------------------------------------------------------
///Paso 1: Encontrar los segmentos en T que terminan o contienen al evento 'p', los que incian con 'p' 
///	se encuentran almacenados en el mismo event_point.
	
	vector<segmento> L, C;
	find_LC(p,L,C,T.begin());
	
	int nU = p.U.size();
	int nL = L.size();
	int nC = C.size();
	
///------------------------------------------------------------------------------------------------------
///Paso 2: Si existe mas de un segmento que comienza, contiene, o termina con este punto de evento, es una 
///	interseccion de varios segmentos y se lo trata correctamente.
	if(nU + nL + nC > 1){
		
		//REVISAR LA INTEGRACION DE ESTAS COSAS CON EL GRAFO....
		
		///En los conjuntos U,C y L se poseen todos los segmentos que se intersectan en este punto de evento,
		///en cuanto a la integracion del grafo, en este punto puedo tomar todos estos segmentos y realizar
		/// los procedimientos necesario para actualizar el grafo.
		
		///Para el casod e lso segmentos en C, se deberan crear 2 halfedge nuevos, enlazarlos correctamente, y corregir
		///los NEXT y PREV como corresponda, debido a esto, los segmentos en C deberian ser los primeros en tratarse,
		///REVISAR!...
		
		///Para el caso de los segmentos en U y L solo se requerira cambiar los NEXT y PREV.
		
		intersecciones.push_back(p.p);
	}

///------------------------------------------------------------------------------------------------------
///Paso 3: Eliminar todos los segmentos que terminan o contienen al evento del arbol de estado, en el caso
/// de los segmentos que contienen al evento, su eliminacion y reinsercion.	
	
	///Altura de la sweep_line, para hacer la busqueda para eliminacion necesito pasarle la altura a los
	///segmentos, para evitar problemas al ser punto de interseccion, elevo la sweep_line un poco, con lo
	///cual evito que recorra de manera equivocada el arbol al comparar empleando el operador '<'.
	double altura = p.p.y + 0.0000005;
	for(int i=0;i<nL;i++){
		L[i].y = &altura;
		T.Delete(L[i]);
	}
	
	for(int i=0;i<nC;i++){
		C[i].y = &altura;
		T.Delete(C[i]);
	}
	
///------------------------------------------------------------------------------------------------------
///Paso 4: Insertar todos los segmentos que comienzan en el evento, los cuales estaran almacenados en el
/// event_point mismo, y reinsertar todos los segmentos que pertenecen al conjunto C, para su reinsercion
/// se concidera una altura por debajo de la sweep_line, asi evaluara la posicion de los segemtos justo 
/// despues de la interseccion.
	
	altura = p.p.y - 0.0000005;
	
	for(int i=0;i<nU;i++){
		p.U[i].y = &altura;
		T.Insert( p.U[i] );
	}
	
	for(int i=0;i<nC;i++){
		C[i].y = &altura;
		T.Insert(C[i]);
	}
	
///------------------------------------------------------------------------------------------------------
///Paso 5: Insertar todos los segmentos que comienzan en el evento, los cuales estaran almacenados en el
/// event_point mismo, y reinsertar todos los segmentos que pertenecen al conjunto C, para su reinsercion
	
	nodo<segmento>* sl = NULL;
	nodo<segmento>* sr = NULL;
	
	if( nU + nC == 0){
		
		for(int i =0;i<nL;i++){
			T.FindNeighbors(sl,sr,L[i]);
			findNewEvent(sl->p,sr->p,p);
		}
		
	}
	else{
		segmento sI,sD; /// sI = segmento mas iaquierdo entre U y C, sL = segmento mas derecho entre U y C.
		
		//Crear estas funciones...
		sI = LeftmostSegment(p.U, C);
		sD = rightmostSegment(p.U, C);
		
		//Crear esta funcion...
		T.FindNeighbors(sl,sr,sI);
		findNewEvent(sl->p,sI,p);
		
		sl = NULL;
		sr = NULL;
		
		T.FindNeighbors(sl,sr,sD);
		findNewEvent(sr->p,sD,p);
		
	}
	
	///NO OLVIDAR PASAR LA ALTURA COMO REFERENCIA A LOS SEGMENTOS A LA HORA DE INSERTAR Y ELIMINAR...
}

void Interseccion::findNewEvent (segmento sl, segmento sr, event_point p) {
	event_point* inter = NULL;
	
	if(Interseccion(sl,sr,inter)){
		if(inter->p.y < p.p.y && !Q.Find(*inter)){
			Q.Insert(*inter);
		}
	}
	
}

double producto_cruz(punto A, punto B, punto C){	//segmento AB(b-a), y segmento AC(c-a)
	return (B.x - A.x)*(C.y - A.y) - (C.x - A.x)*(B.y - A.y);
}

bool Interseccion::calc_inter (segmento s1, segmento s2, event_point &p) {
	
	///Segmento P
	punto A = s1.ini;
	punto B = s1.fin;
	
	///Segmento Q
	punto C = s2.ini;
	punto D = s2.fin;
	
	double ABC,ABD,CDA,CDB;
	double a;
	
	ABC = producto_cruz(A,B,C);	///Vector AB  (A---->B) = B-A
	ABD = producto_cruz(A,B,D);
	CDA = producto_cruz(C,D,A);	///Vector CD  (C---->D) = D-C
	CDB = producto_cruz(C,D,B);
	
	if(ABC*ABD > 0){
		return false;
	}
	if(CDA*CDB > 0){
		return false;
	}
	
	if(fabs(ABD-ABC)<fabs(CDB-CDA)){
		a = (0-ABC)/(ABD-ABC); 
		p.y = C.y*(1-a)+D.y*a;
		p.x = C.x*(1-a)+D.x*a;
	}
	else{
		a = (0-CDA)/(CDB-CDA); 
		p.y = A.y*(1-a)+B.y*a;
		p.x = A.x*(1-a)+B.x*a;
	}
	
	return true;
}


void Interseccion::GraphConverter (segmento S) {
	
	vertex inAux,fnAux;
	
	halfedge iAux,dAux;
	
	inAux.p = S.ini; 
	fnAux.p = S.fin; 
	
	dAux.origen = &inAux;
	dAux.gemela = &iAux;
	dAux.siguiente = &iAux;
	dAux.anterior = &iAux;
	dAux.incidente = NULL;
	
	iAux.origen = &fnAux;
	iAux.gemela = &dAux;
	iAux.siguiente = &dAux;
	iAux.anterior = &dAux;
	iAux.incidente = NULL;
	
	inAux.incidente = &dAux; 
	fnAux.incidente = &iAux; 
	
	grafo.vertice.push_back(fnAux);
	grafo.vertice.push_back(inAux);
	
	grafo.arista.push_back(iAux);
	grafo.arista.push_back(dAux);
	
}

