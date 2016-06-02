#include "Arbol_B.h"
#include "Estructuras.h"

using namespace std;

template<class T>
Arbol_B<T>::Arbol_B() {
	raiz = NULL;
	actual = raiz;
}

template<class T>
Arbol_B<T>::~Arbol_B() {
	
}

//------------------------------------------------------------------------------
///Inserta el nodo a partir de la raiz, para insertarlo correctamente a izquierda o derecha, segun corresponda,
///y realizar el balanceo...
template<class T>
//no retorna nada, cambiar por void...

///ULTIMA OPCION.... ELIMINAR EL PADRE DE LOS NODOS, USAR SOLO IZQUIERDA Y 
///DERECHA, HACER EL BALANCEO AL INSERTAR CADA NODO, EN EL MISMO INSERT(), ME AHORRA EL USO EL PADRE
nodo<T>* Arbol_B<T>::Insert (T ep) {
	
	if(!raiz){
		raiz = new nodo<T>;
		
		raiz->p = ep;
		
		///No configuro aca el padre, sino cuando retorna...
		raiz->left = NULL;
		raiz->right = NULL;
		
		return raiz;
	}
	else{
		raiz = Insert(ep, raiz);
	}
	
//	if(actual->father){
//		actual = actual->father;
//		raiz = balance_insert(actual->father, actual);
//	}
	
	return actual;
}

///Operacion de insercion recursiva, al encontrar una hoja en NULL, inserta alli el nodo correspondiente...
template<class T>
nodo<T>* Arbol_B<T>::Insert (T ep, nodo<T>* act) {
	
	if(!act){	//Cuando act = NULL, el ! cambia la evalucaion de la expresion de falso a verdadero...
		act = new nodo<T>;
		
		act->p = ep;
		
		///No configuro aca el padre, sino cuando retorna...
		act->left = NULL;
		act->right = NULL;
		
		return act;
	}
	
	///Si no era nulo, revisa si va a izquierda o derecha, y realiza recursion...
	if(ep<act->p){
		
		///se cambia el nuevo hijo, no cambiara salvo que haya sido NULL, o se haya realizado balanceo...
		act->left = Insert(ep, act->left);
		
	}
	else{
		
		///se cambia el nuevo hijo, no cambiara salvo que haya sido NULL, o se haya realizado balanceo...
		act->right = Insert(ep, act->right);
		
	}

	return balance(act);
}

//------------------------------------------------------------------------------

//Se supone que el nodo que quiero borrar esta si o si en el arbol.
template<class T>
nodo<T>* Arbol_B<T>::Delete(T ep){
	
	///Posible problema al borrar la raiz...
	raiz = Delete(ep, raiz);

//	///actual es el padre del nodo eliminado...
//	raiz = balance_delete(actual->father, actual);
//	
}

template<class T>
nodo<T>* Arbol_B<T>::Delete (T ep, nodo<T>* act) {
	//Voy a hacer la misma busqueda en el Delete, pero aprovecho el mecanismo para 
	//controlar el factor de balanceo de los nodos.
	
	if(!act) return NULL;
	
	if(ep == act->p){
		
		nodo<T>* aux;
		
		if(act->left!=NULL){	
			aux = rightmost(act->left);	
			act->p=aux->p;
			
			if(!act->left) cout<<"NULL LEFT"<<endl;
			
			act->left = Delete(aux->p,act->left);
			
		}
		else{
			if(act->right!=NULL){	
				aux = leftmost(act->right);	
				act->p=aux->p;
				
				if(!act->right) cout<<"NULL RIGHT"<<endl;
				
				act->right = Delete(aux->p,act->right);
				
			}
			else{
				//free(act);
				return NULL;
			}
		}
		
	}
	else{
		
		if(ep<act->p){
			
			if(!act->left) cout<<"NULL LEFT SIN"<<endl;
			act->left = Delete(ep,act->left);
			
		}
		else{
			
			if(!act->right) cout<<"NULL RIGHT SIN"<<endl;
			act->right = Delete(ep,act->right);
			
		}
	}
	
	return balance(act);
}

//------------------------------------------------------------------------------
template<class T>
nodo<T> * Arbol_B<T>::leftmost (nodo<T> * l) {
	if(l->left==NULL){
		return l;
	}
	return leftmost(l->left);
}

template<class T>
nodo<T> * Arbol_B<T>::rightmost (nodo<T> * r) {
	if(r->right==NULL){
		return r;
	}
	return rightmost(r->right);
}

//------------------------------------------------------------------------------
template<class T>
nodo<T>* Arbol_B<T>::Find (T ep) {
	return Find(ep,raiz);	
}

template<class T>
nodo<T>* Arbol_B<T>::Find (T ep, nodo<T>* act) {
	
	if(act == NULL) return NULL;
	
	if(ep == act->p)
		return act;
	
	if(ep<act->p){
		return Find(ep,act->left);
	}
	else{
		return Find(ep,act->right);
	}
}

template<class T>
void Arbol_B<T>::FindNeighbors(T &sl, T &sr, T p){
	FindNeighbors(sl,sr,p,raiz);
}

template<class T>
void Arbol_B<T>::FindNeighbors (T &sl, T &sr, T p, nodo<T>* act) {
	if(!act) return;
	if(p == act->p){
		if(act->left){
			sl = rightmost(act->left)->p;
		}
		if(act->right){
			sr = leftmost(act->right)->p;
		}
		return;
	}
		
	
	if(p<act->p){
		sr = act->p;
		FindNeighbors(sl,sr,p,act->left);
	}
	else{
		sl = act->p;
		FindNeighbors(sl,sr,p,act->right);
	}
}
//------------------------------------------------------------------------------
template<class T>
nodo<T>* Arbol_B<T>::rotate_left (nodo<T>* root) {
	nodo<T>* P=root->right;
	if(P){
		root->right=P->left;
		if(P->left){
			P->left->father=root;
		}
		P->left=root;
	}

	return P;
}

template<class T>
nodo<T>* Arbol_B<T>::rotate_right (nodo<T>* root) {
	nodo<T>* P=root->left;
	if(P){
		root->left=P->right;
		if(P->right!=NULL){
			P->right->father=root;
		}
		P->right=root;
	}
	
	return P;
}

template<class T>
//N es el nodo al cual se le inserto un nodo, P es su padre, de este modo considero de a
//2 elementos para hacer los rotate

nodo<T>* Arbol_B<T>::balance(nodo<T>* P) {
	
	// si es -2 indica que hay de mas a la izquierda
	if (calc_factor(P) == -2) { 
		if (calc_factor(P->left) == 1) {
			P->left = rotate_left(P->left); 
		}
		
		return rotate_right(P);
		
	}
	else{
		if (calc_factor(P) == 2) {
			
			if (calc_factor(P->right) == -1) {
				P->right = rotate_right(P->right);
			}
			
			return rotate_left(P);
			
		}
	}
	
	return P;
}

//------------------------------------------------------------------------------
//Funciones Auxiliares Extras
template<class T>
int calc_nivel(nodo<T>* act){

	if(!act){
		return 0;
	}

	int nivel = 1;
	nivel += max(calc_nivel(act->left),calc_nivel(act->right)); 
	
	return nivel;
}

template<class T>
int calc_factor(nodo<T>* act){
	int factor = 0;
	
	factor = 0;
	factor += (act->right)? calc_nivel(act->right): 0;
	factor -= (act->left)? calc_nivel(act->left) : 0;
	
	return factor;
}

//------------------------------------------------------------------------------
template<class T>
void Arbol_B<T>::show(){
	show(raiz,0);
}

template<class T>
void Arbol_B<T>::show (nodo<T>* act, int nivel) {
	
	int aux = nivel+1; 
	
	if(!act) return;
	
	if(act->left){
		show(act->left,aux);
	}
	
	cout<<"Nivel: "<<nivel<<endl;
	
//	cout<<act->p<<"	";
	
	//SOLO PARA TESTEO, ELIMINAR LUEGO...
	act->p.show();
	
	//cout<<"Balance Factor: "<<act->balance_factor<<endl;
	cout <<endl;
	
	if(act->right){
			show(act->right,aux);
	}
	
}

template<class T>
bool Arbol_B<T>::empty ( ) {
	return !raiz;
}

template<class T>
nodo<T>* Arbol_B<T>::begin ( ) {
	return raiz;
}

///NECESARIO PARA EL FUNCIONAMIENTO, DEFINIR AQUI EN QUE TIPOS SE PODRA EMPLEAR EL ARBOL...
//template class Arbol_B<int>;
template class Arbol_B<segmento>;
template class Arbol_B<event_point>;


