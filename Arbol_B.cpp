#include "Arbol_B.h"

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
nodo<T>* Arbol_B<T>::Insert (T ep) {
//	if(raiz == null){
//		
//		raiz= new nodo;
//		
//		raiz->p=ep;
//		
//		raiz->father=NULL;
//		raiz->left=NULL;
//		raiz->right=NULL;
//		
//		///El factor de balanceo, se obtiene por al diferencia de altura entre el lado derecho y el izquierdo..
//		///cada ves que agrego un nodo a la derecha, debo sumarle 1, y cada ves que elimino, debo restarle 1...
//		///esas sumas y restas se manejan en la insercion y eliminacion(se supone que la eliminacion siempre se llama con valores que esten en el arbol)...
//		raiz->balance_factor=0;
//		
//		actual = raiz;
//		
//		return raiz;
//	}
	
//	///Si la raiz no esta vacia, indica que ya hay un nodo en el arbol, debo evaluar si el nuevo elemento a 
//	///insertar, se halla a la izquierda o la derecha, entonces comenzar las llamadas recursivas para insertar el nodo
//	///en la primer hoja vacia a la que llegue...
	actual = raiz;
	
	///Puede ser que se de una rotacion hasta la raiz...
	//REVISAR!!...
	actual = Insert(ep, actual);
	actual->father = NULL;
	
	raiz = actual;
	
	
}

///Operacion de insercion recursiva, al encontrar una hoja en NULL, inserta alli el nodo correspondiente...
template<class T>
nodo<T>* Arbol_B<T>::Insert (T ep, nodo<T>* act) {
	
	///Si era nulo, lo crea y le asigna valores, luego lo retorna...
	if(act == NULL){
		act = new nodo<T>;
		
		act->p = ep;
		
		///No configuro aca el padre, sino cuando retorna...
		act->left = NULL;
		act->right = NULL;
		
		act->balance_factor=0;
		
		return act;
	}
	
	///Si no era nulo, revisa si va a izquierda o derecha, y realiza recursion...
	if(ep<act->p){
		///Si va a la izquierda, corrige el factor y inserta...
		act->balance_factor -=1;
		
		///se cambia el nuevo hijo, no cambiara salvo que haya sido NULL, o se haya realizado balanceo...
		act->left = Insert(ep, act->left);
		///se le asigna el nuevo padre...
		act->left->father = act;
	}
	else{
		///Si va a la derecha, corrige el valor e inserta...
		act->balance_factor +=1;
		
		act->right = Insert(ep, act->right);
		act->right->father = act;
	}
	
	///Balanceo para corregir el arbol, luego retorno el nodo que cambio con el balanceo...
	act = balance(act); //REVISAR LOS ROTATE PARA EL BALANCEO...
	
	return act;
}

//------------------------------------------------------------------------------

//nodo * Arbol_B::Delete (T ep) {
//	
//}
//
//nodo * Arbol_B::Delete (T ep, nodo * act) {
//	
//}

//------------------------------------------------------------------------------

//nodo * Arbol_B::Find (T ep) {
//	
//}
//
//nodo * Arbol_B::Find (T ep, nodo * act) {
//	
//}

//------------------------------------------------------------------------------
template<class T>
nodo<T>* Arbol_B<T>::rotate_left (nodo<T>* root) {
	nodo<T>* P=root->right;
	if(P!=NULL){
		root->right=P->left;
		if(P->left!=NULL){
			P->left->father=root;
		}
		P->left=root;
	}
	root->father=P;
	
	return P;
}
template<class T>
nodo<T>* Arbol_B<T>::rotate_right (nodo<T>* root) {
	nodo<T>* P=root->left;
	if(P!=NULL){
		root->left=P->right;
		if(P->right!=NULL){
			P->right->father=root;
		}
		P->right=root;
	}
	root->father=P;
	
	return P;
}
template<class T>
nodo<T>* Arbol_B<T>::balance (nodo<T>* root) {
	
	if(root->balance_factor==0) return NULL;
	
/*	///Debo tener 4 tipos de rotaciones, izquierda derecha, derecha izquierda, izquierda izquierda, derecha derecha...*/
	if(root->balance_factor<0){
		root->balance_factor += 1;
		root = rotate_right(root);
	}
	else{
		root->balance_factor -= 1;
		root = rotate_left(root);
	}
	
}

//------------------------------------------------------------------------------
template<class T>
void Arbol_B<T>::show(){
	show(raiz);
}
template<class T>
void Arbol_B<T>::show (nodo<T>* act) {
	
	if(act == NULL) return;
	
	if(act->left!=NULL){
		show(act->left);
	}
	
	cout<<act->p<<"	";
	
	if(act->right!=NULL){
			show(act->right);
	}
	
}

template class Arbol_B<int>;
