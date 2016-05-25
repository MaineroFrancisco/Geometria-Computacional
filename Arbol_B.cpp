#include "Arbol_B.h"
#include "Interseccion.h"

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
void Arbol_B<T>::Insert (T ep) {

	actual = raiz;
	
	//va a retornar un puntero al nodo insertado...
	if(!raiz){ 
		raiz = Insert(ep, actual, NULL);
		actual = raiz;
	}
	else{
		actual = Insert(ep, actual, NULL);
	}
	//Como el nodo insertado sera una hoja, el balanceo en esta punto no sera necesario, entonces me
	//muevo uno hacia arriba.

	if(actual->father){
		actual = actual->father;
		raiz = balance_insert(actual->father, actual);
	}
	
	return;
}

///Operacion de insercion recursiva, al encontrar una hoja en NULL, inserta alli el nodo correspondiente...
template<class T>
nodo<T>* Arbol_B<T>::Insert (T ep, nodo<T>* act, nodo<T>* fact) {
	
	nodo<T>* insertado;
	
	///Si era nulo, lo crea y le asigna valores, luego lo retorna...
	if(!act){	//Cuando act = NULL, el ! cambia la evalucaion de la expresion de falso a verdadero...
		act = new nodo<T>;
		
		act->p = ep;
		
		///No configuro aca el padre, sino cuando retorna...
		act->left = NULL;
		act->right = NULL;
		act->father = fact;
		
		insertado = act;
		
		return insertado;
	}
	
	///Si no era nulo, revisa si va a izquierda o derecha, y realiza recursion...
	if(ep<act->p){
		
		///se cambia el nuevo hijo, no cambiara salvo que haya sido NULL, o se haya realizado balanceo...
		insertado = Insert(ep, act->left, act);
		
		if(!act->left) act->left = insertado;
		
	}
	else{
		
		///se cambia el nuevo hijo, no cambiara salvo que haya sido NULL, o se haya realizado balanceo...
		insertado = Insert(ep, act->right, act);
		
		if(!act->right) act->right = insertado;
	}

	return insertado;
}

//------------------------------------------------------------------------------
//REVISAR PROBLEMAS RECURSIVOS Y BALANCEO... MODIFICAR EL SHOW PARA MOSTRAR NIVELES DEL ARBOL...
//PROBLEMA DE BALANCEO... PROBAR CON UNA GRAN CANTIDAD DE ENTEROS, E IR REFINANDO EL FACTOR DE
//BALANCEO...

//Se supone que el nodo que quiero borrar esta si o si en el arbol.
template<class T>
void Arbol_B<T>::Delete (T ep) {
	
	actual=raiz;

	///PROBLEMA CON EL PASAJE POR REFERENCIA DE DATOS, SOLUCIONAR PARA LA GENERALIDAD DEL TEMPLATE...
	nodo<T>* elim = NULL;
	
	raiz = Delete(ep, actual, elim);
	
	if(elim){
		raiz = balance_delete(elim->father, elim);
	}
	
}

template<class T>
nodo<T>* Arbol_B<T>::Delete (T ep, nodo<T>* act, nodo<T>* elim) {
	//Voy a hacer la misma busqueda en el Delete, pero aprovecho el mecanismo para 
	//controlar el factor de balanceo de los nodos.
	nodo<T>* eliminado;
	
	if(ep == act->p){
		
		//Con esta solucion yo puedo buscar el que deseo eliminar, actualizando los 
		//factores de balanceo mientras busco, una ves encontrado, busco el mas
		//izquierdo/derecho (segun corresponda), cambio los valores de sus variables,
		//actualizo los factores, y ejecuto el delete para el mas izquierdo/derecho,
		//con lo cual recursivamente se actualizan los factores y se elimina lo
		//que corresponda.
		
		nodo<T>* aux;
		
		if(act->left!=NULL){	
			aux = rightmost(act->left);	
			act->p=aux->p;
			eliminado = Delete(aux->p,act->left,elim);
			act->left = eliminado;
		}
		else{
			if(act->right!=NULL){	
				aux = leftmost(act->right);	
				act->p=aux->p;
				eliminado = Delete(aux->p,act->right,elim);
				act->right = eliminado;
			}
			else{
				//Si no tiene Hijos, ya e llegado a una hoja, puedo borrar directamente
				//sin preocuparme por su balanceo, por lo tanto retorno directamente NULL
				return NULL;
			}
		}
		
		//llegue al nodo que busco
	}
	else{
		
		if(ep<act->p){
			//Como el nodo que quiero borrar esta a la izquierda al momento de borrarlo,
			//mi factor de balanceo se incrementa en 1, ya que hay un elemento izquierdo menos.
			//act->balance_factor += 1;
			eliminado = Delete(ep,act->left, elim);
			act->left = eliminado;
		}
		else{
			//Como el nodo que quiero borrar esta a la derecha al momento de borrarlo,
			//mi factor de balanceo se decrementa en 1, ya que hay un elemento derecho menos.
			//act->balance_factor -= 1;
			eliminado = Delete(ep,act->right, elim);
			act->right = eliminado;
		}
	}
	
	if(!eliminado){
		elim = act;
	}
	
	return act;
}

//------------------------------------------------------------------------------
template<class T>
nodo<T>* Arbol_B<T>::left (nodo<T> * l) {
	
	if(l->left){
		return rightmost(l->left);
		
	}
	else{
		if(l->father->right == l)
			return l->father;
	}
	
	return NULL;
}

template<class T>
nodo<T> * Arbol_B<T>::right (nodo<T> * r) {
	
	if(r->right){
		return leftmost(r->right);
		
	}
	else{
		if(r->father->left == r)
			return r->father;
	}
	
	return NULL;
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
	
	if(ep == act->p)
		return act;
	
	if(ep<act->p){
		return Find(ep,act->left);
	}
	else{
		return Find(ep,act->right);
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
	P->father=root->father;
	root->father=P;
	
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
	P->father=root->father;
	root->father=P;
	
	return P;
}

template<class T>
//N es el nodo al cual se le inserto un nodo, P es su padre, de este modo considero de a
//2 elementos para hacer los rotate

nodo<T>* Arbol_B<T>::balance_insert(nodo<T>* P, nodo<T>* N) {
	
	if(P==NULL) return N;	//La raiz es el unico con un padre(P) NULL
	
	nodo<T>* R = P;
	nodo<T>* F = P->father;
	bool izq = F && F->left==P;
	
	// N iS the child of P whoSe height increaSeS by 1.
	do {
		// balance_factor(P) haS not yet been uPdated!
		if (N == P->left) { // the left Subtree increaSeS
			if (calc_factor(P) == -2) { // The left column in the Picture
				// ==> the temPorary balance_factor(P) == 2 ==> rebalancing iS required.
				if (calc_factor(N) == 1) { // Left Right CaSe
					P->left = rotate_left(N); // Reduce to Left Left CaSe
				}
				// Left Left CaSe
				R = rotate_right(P);
				break; // Leave the looP
			}
			
		} else { // N == right_child(P), the child whoSe height increaSeS by 1.
			if (calc_factor(P) == 2) { // The right column in the Picture
				// ==> the temPorary balance_factor(P) == -2 ==> rebalancing iS required.
				if (calc_factor(N) == -1) { // Right Left CaSe
					P->right = rotate_right(N); // Reduce to Right Right CaSe
				}
				// Right Right CaSe
				R = rotate_left(P);
				break; // Leave the looP
			}
			//balance_factor(P) = -1; // Height increaSeS at P
		}
		N = P;
		P = P->father;
		
		if(P){
			F = P->father;
			izq = F && F->left==P;
		}
		
	} while (P != NULL); // PoSSibly uP to the root

	if(F){
		if(izq)	F->left = R;
		else F->right = R;
	}
	//Si produce un giro hasta la raiz, se vera reflejado, 
	//y se retornará el valor correspondiente, el ultimo agregado a N
	
	//Si no llega a un giro hasta la raiz, se movera hasta al raiz y la retornara para no perder el
	//hilo del arbol, actualizarlo siempre
	
	///ANDAAAAAAA!!! ... Ahora optimizar y empezar la interseccion...
	///ARREGLAR LO DE ACA ABAJO... UNA PIJA...
	P = R;
	N = R;
	while(P != NULL){
		N = P;
		P = P->father;
	}
	
	return N;
}

template<class T>
nodo<T>* Arbol_B<T>::balance_delete(nodo<T>* P, nodo<T>* N) {
	
	if(P==NULL) return N;	//La raiz es el unico con un padre(P) NULL
	
	nodo<T>* S;
	nodo<T>* R = P;
	nodo<T>* F = P->father;
	bool izq = F && F->left==P;
	
	// N iS the child of P whoSe height increaSeS by 1.
	do {
		// balance_factor(P) haS not yet been uPdated!
		if (N == P->left) { // the left Subtree increaSeS
			if (calc_factor(P) == 2) { // The left column in the Picture
				
				S = P->right;
				// ==> the temPorary balance_factor(P) == 2 ==> rebalancing iS required.
				if (calc_factor(S) == -1) { // Left Right CaSe
					P->right = rotate_right(S); // Reduce to Left Left CaSe
				}
				// Left Left CaSe
				R = rotate_left(P);
				break; // Leave the looP
			}
			
		} else { // N == right_child(P), the child whoSe height increaSeS by 1.
			if (calc_factor(P) == -2) { // The right column in the Picture
				
				S = P->left;
				// ==> the temPorary balance_factor(P) == -2 ==> rebalancing iS required.
				if (calc_factor(S) == 1) { // Right Left CaSe
					P->left = rotate_left(S); // Reduce to Right Right CaSe
				}
				// Right Right CaSe
				R = rotate_right(P);
				break; // Leave the looP
			}
			//balance_factor(P) = -1; // Height increaSeS at P
		}
		N = P;
		P = P->father;
		
		if(P){
			F = P->father;
			izq = F && F->left==P;
		}
		
	} while (P != NULL); // PoSSibly uP to the root
	
	if(F){
		if(izq)	F->left = R;
		else F->right = R;
	}
	//Si produce un giro hasta la raiz, se vera reflejado, 
	//y se retornará el valor correspondiente, el ultimo agregado a N
	
	//Si no llega a un giro hasta la raiz, se movera hasta al raiz y la retornara para no perder el
	//hilo del arbol, actualizarlo siempre
	
	P = R;
	N = R;
	while(P != NULL){
		N = P;
		P = P->father;
	}
	
	return N;
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
	
///	cout<<act->p<<"	";
	
	//SOLO PARA TESTEO, ELIMINAR LUEGO...
	act->p.show();
	
	//cout<<"Balance Factor: "<<act->balance_factor<<endl;
	cout <<endl;
	
	if(act->right){
			show(act->right,aux);
	}
	
}

///NECESARIO PARA EL FUNCIONAMIENTO, DEFINIR AQUI EN QUE TIPOS SE PODRA EMPLEAR EL ARBOL...
//template class Arbol_B<int>;
template class Arbol_B<segmento>;
template class Arbol_B<event_point>;
