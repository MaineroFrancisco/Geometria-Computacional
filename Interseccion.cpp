#include "Interseccion.h"

Interseccion::Interseccion() {
	
}

Interseccion::~Interseccion() {
	
}

Grafos* Interseccion::grafo_resultante ( ) {
	return &grafo;
}

///MEDIDA DEL ERROR: 0.000000005;

///AL PRINCIPIO CUANDO SETEO LOS SEGMENTOS, DEBO HACER QUE SU CAMPO (*Y) APUNTE AL DOUBLE SWEEP_LINE
/// DE LA CLASE, DE ESTE MODO LO UNICO QUE TENGO QUE HACER LUEGO ES CAMBIAR SWEEP_LINE CUANDO LO NECESITE
/// Y NO VOLVER A TOCAR EL CAMPO (*Y).
///COMO (*Y) APUNTA A LA DIRECCION DOUBLE SWEEP_LINE, AL CAMBIAR EL VALOR DE SWEEP_LINE, NO CAMBIA 
///SU DIRECCION, CON LO CUAL (*Y) SIEMPRE SE MANTIENE ACTUALIZADA CON EL ULTIMO VALOR DE LA SWEEP_LINE.

void Interseccion::FindIntersection (vector<segmento> S ) {
	
	///1) Inicializar Q el arbol que contendra los event_point
	
	InitializeQ(S);
	
	///2) Inicializar el arbol de estados T...
	
	event_point aux;
	
	while(!Q.empty()){
		
		aux = Q.leftmost(Q.begin())->p;
		
		HandleEventPoint(aux);
		
		Q.Delete(aux);
		
	}
	
}

///AGREGAR FIND_LC A INTERSECCION...LO MISMO PARA LAS QUE SON EXTRA EN ARBOL...ELIMINAR LEFT Y RIGHT DE ARBOL_B
void find_LC(event_point ep, vector<segmento> &L, vector<segmento> &C, nodo<segmento>* n){
	
	if(!n) return;
	
	find_LC (ep,L,C,n->left);
	
	if(n->p.fin == ep.p){
		L.push_back(n->p);
	}
	else{
		if(n->p.contains(ep.p)){
			C.push_back(n->p);
		}
	}
	
	find_LC (ep,L,C,n->right);
}
///------------------------------------------------------------------------------------------------------

void Interseccion::HandleEventPoint (event_point ep) {
	
	///Paso 1: Encontrar los segmentos en T que terminan o contienen al evento 'p', los que incian con 'p' 
	///	se encuentran almacenados en el mismo event_point.
	
	vector<segmento> L, C;
	
	find_LC(ep,L,C,T.begin());
	
	int nU = ep.U.size();
	int nL = L.size();
	int nC = C.size();
	
	int nI = nU + nL + nC;

	///Creo un vertice en el grafo por cada event_point, obtengo luego una referencia a dicho vertex en el 
	///grafo, para poder hacer el enlace con los halfedge en pasos posteriores.
	
	vertex V, *Ve = NULL;
	
	V.p = ep.p;
	grafo.vertice.push_back(V);
	Ve = &grafo.vertice[grafo.vertice.size()-1];
	
	///------------------------------------------------------------------------------------------------------
	///Paso 3: Eliminar todos los segmentos que terminan o contienen al evento del arbol de estado, en el caso
	/// de los segmentos que contienen al evento, su eliminacion y reinsercion.	
	
	sweep_line = ep.p.y;
	
	if(nC > 1) sweep_line += 1e-9;
	
	halfedge H, *He=NULL , Tw, *Te=NULL;
	
	for(int i=0;i<nL;i++){
		
		///Completar al grafo, con el vertex origen de las twin para cada segmento que necesite terminar...
		He = L[i].arista;
		Te = He->gemela;
		
		Te->origen = Ve;
		
		Ve->incidente = Te;
		
		T.Delete(L[i]);
	}
	
	for(int i=0;i<nC;i++){
		T.Delete(C[i]);
	}
	
	///------------------------------------------------------------------------------------------------------
	///Paso 4: Insertar todos los segmentos que comienzan en el evento, los cuales estaran almacenados en el
	/// event_point mismo, y reinsertar todos los segmentos que pertenecen al conjunto C, para su reinsercion
	/// se concidera una altura por debajo de la sweep_line, asi evaluara la posicion de los segemtos justo 
	/// despues de la interseccion.
	
	if(nC > 1) sweep_line -= 2*1e-9;
	
	for(int i=0;i<nU;i++){
		
		///Crear HALFEDGE para incorporar al grafo...
		
		H.origen = Ve;
		grafo.arista.push_back(H);
		
		He = &grafo.arista.back();
		
		grafo.arista.push_back(Tw);
		
		Te = &grafo.arista.back();
		
		He->gemela = Te;
		Te->gemela = He;
		
		Ve->incidente = He;
		
		ep.U[i].arista = He;
		
		T.Insert( ep.U[i] );
	}
	
	for(int i=0;i<nC;i++){
		T.Insert(C[i]);
	}
	
	///------------------------------------------------------------------------------------------------------
	///Paso 5: Insertar todos los segmentos que comienzan en el evento, los cuales estaran almacenados en el
	/// event_point mismo, y reinsertar todos los segmentos que pertenecen al conjunto C, para su reinsercion
	
	nodo<segmento>* sl = NULL;
	nodo<segmento>* sr = NULL;
	
	if( nU + nC == 0){
		
		for(int i =0;i<nL;i++){
			FindNeighbors(sl,sr,L[i], T.begin());
			
			if(sl && sr){
				findNewEvent(sl->p,sr->p,ep);
			}
		}
		
	}
	else{
		segmento sI,sD; /// sI = segmento mas iaquierdo entre U y C, sL = segmento mas derecho entre U y C.
		
		if(ep.U.size()<C.size()){
			sI = LeftmostSegment(C, ep.U);
			sD = RightmostSegment(C, ep.U);	
		}
		else{
			sI = LeftmostSegment(ep.U, C);
			sD = RightmostSegment(ep.U, C);	
		}
		
		FindNeighbors(sl,sr,sI, T.begin());
		
		if(sl){
			findNewEvent(sl->p,sI,ep);
		}
		
		sl = NULL;
		sr = NULL;
		
		FindNeighbors(sl,sr,sD, T.begin());
		
		if(sr){
			findNewEvent(sr->p,sD,ep);
		}
		
	}
	
	///------------------------------------------------------------------------------------------------------
	///Paso 2: Si existe mas de un segmento que comienza, contiene, o termina con este punto de evento, es una 
	///	interseccion de varios segmentos y se lo trata correctamente.
	
	if(nI > 1){
		
		///Si el evento es interseccion y no es ni inicio ni fin de un segmento, hara falta crear el vertex
		///en el grafo para corregir los segmnetos, sino ya se lo tiene referenciado en Ve.
		
		///En los conjuntos U,C y L se poseen todos los segmentos que se intersectan en este punto de evento,
		///en cuanto a la integracion del grafo, en este punto puedo tomar todos estos segmentos y realizar
		/// los procedimientos necesario para actualizar el grafo.
		
		///Para el caso de los segmentos en C, se deberan crear 2 halfedge nuevos, enlazarlos correctamente, y corregir
		///los NEXT y PREV como corresponda, debido a esto, los segmentos en C deberian ser los primeros en tratarse,
		///REVISAR!...
		
		///Para el caso de los segmentos en U y L solo se requerira cambiar los NEXT y PREV.
		
		intersecciones.push_back(ep.p);
	}
}

///-----------------------------------------------------------------------------------------------------------
void Interseccion::FindNeighbors( nodo<segmento>* &sl, nodo<segmento>* &sr, segmento p, nodo<segmento>* act){
	if(!act) return;
	
	if(p == act->p){
		if(act->left){
			sl = T.rightmost(act->left);
		}
		if(act->right){
			sr = T.leftmost(act->right);
		}
		return;
	}
	
	if(p<act->p){
		sr = act;
		FindNeighbors(sl,sr,p,act->left);
	}
	else{
		sl = act;
		FindNeighbors(sl,sr,p,act->right);
	}
}

void Interseccion::findNewEvent (segmento sl, segmento sr, event_point ep) {
	event_point inter;
	
	if(calc_inter(sl,sr, inter)){
		if(inter.p.y < ep.p.y && !Q.Find(inter)){
			Q.Insert(inter);
		}
	}
	
}

double producto_cruz(punto A, punto B, punto C){	//segmento AB(b-a), y segmento AC(c-a)
	return (B.x - A.x)*(C.y - A.y) - (C.x - A.x)*(B.y - A.y);
}

bool Interseccion::calc_inter (segmento s1, segmento s2, event_point &ep) {
	
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
		ep.p.y = C.y*(1-a)+D.y*a;
		ep.p.x = C.x*(1-a)+D.x*a;
	}
	else{
		a = (0-CDA)/(CDB-CDA); 
		ep.p.y = A.y*(1-a)+B.y*a;
		ep.p.x = A.x*(1-a)+B.x*a;
	}
	
	return true;
}

segmento Interseccion::LeftmostSegment(vector<segmento> mayor, vector<segmento> menor){
	int n = mayor.size();
	int m = menor.size();
	
	segmento leftmost = mayor[0];
	
	for (int i=1;i<n;i++){
		
		if(mayor[i]< leftmost){
			leftmost = mayor[i];
		}
		if(n<m){
			if(menor[i]<leftmost){
				leftmost = menor[i];
			}
		}
		
	}
	
	return leftmost;
}

segmento Interseccion::RightmostSegment(vector<segmento> mayor, vector<segmento> menor){
	int n = mayor.size();
	int m = menor.size();
	
	segmento rightmost = mayor[0];
	
	for (int i=1;i<n;i++){
		
		if(rightmost<mayor[i]){
			rightmost = mayor[i];
		}
		if(n<m){
			if(rightmost<menor[i]){
				rightmost = menor[i];
			}
		}
		
	}
	
	return rightmost;
}

void Interseccion::InitializeQ(vector<segmento> S){
	event_point evI, evF;
	
	///Agregar los puntos de los segmentos en S como puntos de evento en Q
	int n = S.size();
	for (int i=0; i<n;i++){
		
		S[i].y = &sweep_line;
		
		evI.p = S[i].ini;
		evI.U.push_back(S[i]);
		
		evF.p = S[i].fin;
		
		nodo<event_point>* n_aux = Q.Find(evI);
		
		if(!n_aux){
			Q.Insert(evI);
		}
		else{
			n_aux->p.U.push_back(S[i]);
		}
		
		n_aux = Q.Find(evF);
		if(!n_aux){
			Q.Insert(evF);
		}
		
		evI.U.clear();
	}
}


vector<punto> Interseccion::GetIntersection(){
	return intersecciones;
}
