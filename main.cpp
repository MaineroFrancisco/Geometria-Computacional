#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include "Arbol_B.h"
#include "Interseccion.h"
#include <ctime>

using namespace std;

vector<segmento> S;
vector<punto> I;

bool hecho = false;

segmento Get_Segment(){
	punto a,b;
	
	segmento seg;
	
	///Crea punto scon una precision de hasta 4 decimales...
	a.x = (rand()%101);
	a.y = (rand()%101);
	
	b.x = (rand()%101);
	b.y = (rand()%101);
	
	if(b.y > a.y){
		
		seg.ini=b;
		seg.fin=a;
		
	}
	else{
		if(b.y < a.y){
			seg.ini=a;
			seg.fin=b;
		}
		else{
			///CASO CUANDO ES HORIZONTAL, TODAVIA NO CONTEMPLADO...
			
			b.y-=1;
			
			seg.ini=a;
			seg.fin=b;
		}
	}
	
	return seg;
}


void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,100,0,100);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	//------------------------------------------------------------------------------
	
	///LO UNICO QUE ME QUEDA SOLUCIONAR SON ERRORES DE PRECISION... LOS BIAS DE ERROR ME HACEN CAGADA...
	
	if(!hecho){
		
		for(int i=0;i<50;i++){
			S.push_back(Get_Segment());
		}
		
		for(int i = 0; i< S.size();i++){
			
			glColor3f(.0f,.0f,.0f);
			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glVertex2d(S[i].ini.x,S[i].ini.y);
			glVertex2d(S[i].fin.x,S[i].fin.y);
			glEnd();
			
			glColor3f(1.0f,.0f,.0f);
			glPointSize(3.0f);
			glBegin(GL_POINTS);
			glVertex2d(S[i].ini.x,S[i].ini.y);
			glVertex2d(S[i].fin.x,S[i].fin.y);
			glEnd();
			
		}
		
		///--------------------------------------------------------------------
		Interseccion i;
		
		///ERROR, NO ME ESTA HACIENDO BIEN EL SWAP DE LOS SEGMENTOS...
		///REALIZAR UN SEGUIMIENTO DEL ALGORITMO PARA VERIFICAR LOS CASOS...
		///MUY PROBABLE PROBLEMA DE PRECISION...
		i.FindIntersection(S);
		
		I = i.GetIntersection();
		
		if(!I.empty()){
			cout<<"HAY INTERSECCION: "<< I.size()<<endl;
		}
		
		hecho = true;
	}
	//------------------------------------------------------------------------------
	else{
		for(int i = 0; i< S.size();i++){
			
			glColor3f(.0f,.0f,.0f);
			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glVertex2d(S[i].ini.x,S[i].ini.y);
			glVertex2d(S[i].fin.x,S[i].fin.y);
			glEnd();
			
			glColor3f(1.0f,.0f,.0f);
			glPointSize(3);
			glBegin(GL_POINTS);
			glVertex2d(S[i].ini.x,S[i].ini.y);
			glVertex2d(S[i].fin.x,S[i].fin.y);
			glEnd();
			
		}
		
		if(!I.empty()){
			for(int i = 0; i< I.size();i++){
				
				glColor3f(.0f,.0f,1.0f);
				glPointSize(3);
				
				glBegin(GL_POINTS);
				glVertex2d(I[i].x,I[i].y);
				glEnd();
				
			}
		}
	}
	
	glutSwapBuffers();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (640,480);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Ventana OpenGL");
	glutDisplayFunc (display_cb);
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
	
	//------------------------------------------------------------------------------	
	
	
	//------------------------------------------------------------------------------
	
	/////INSERT Y DELETE FUNCIONAN MEJOR...
//	
	//Arbol_B<int> A;
	//A.Insert(1);
	//A.Insert(2);
	//A.Insert(3);
	//A.Insert(4);
	//A.Insert(5);
	//A.Insert(6);
	//A.Insert(7);
	//
	//A.Delete(1);
	//A.Delete(2);
	//A.Delete(3);
	//A.Delete(4);
	//A.Delete(5);
	//A.Delete(6);
	//A.Delete(7);
	//
	//A.show();
//	
	//------------------------------------------------------------------------------	
	
//	int *a;
//	int *b;
//	int c = 1;
//	b = &c;
//	a = b;
//	cout<<*a<<endl;
//	
//	b = 0;
//	
//	cout<<*a<<endl;
}

int main (int argc, char **argv) {
	
	srand(time(NULL));
	
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}
