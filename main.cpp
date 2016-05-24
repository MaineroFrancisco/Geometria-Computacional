#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include "Arbol_B.h"

using namespace std;

void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void display_cb() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,0); glLineWidth(3);
	glBegin(GL_LINE_STRIP);
		glVertex2i(130,060); glVertex2i( 50,060);
		glVertex2i(130,150); glVertex2i( 50,150);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i( 70,100); glVertex2i(110,100);
		glVertex2i(150,100); glVertex2i(230,100);
		glVertex2i(190,140); glVertex2i(190,070);
		glVertex2i(250,100); glVertex2i(330,100);
		glVertex2i(290,140); glVertex2i(290,070);
	glEnd();
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
	//ESTO ES ENTERAMENTE PARA EL TESTEO DE LA ESTRUCTURA ARBOL, LUEGO ELIMINAR...
	Arbol_B<int> A;
	for (int i=0;i<16;i++){
		A.Insert(i);
	}
	//No funciona para event_points y segmentos todavia... probar con MUCHOS ENTEROS,
	//problema en el manejo del factor de balanceo... posiblemente deba eliminarlo como atributo,
	//reemplazandolo por una funcion, o cambiar su tratamiento...
	
//	event_point a,b;
//	a.p.x = 1;
//	a.p.y = 3;
//	b.p.x = 1;
//	b.p.y = 1; 
//	
//	event_point c,d;
//	c.p.x = 3;
//	c.p.y = 3;
//	d.p.x = 3;
//	d.p.y = 1; 
//	
//	event_point e,f;
//	e.p.x = 2;
//	e.p.y = 4;
//	f.p.x = 2;
//	f.p.y = 6;
//	
//	A.Insert(a);
//	A.Insert(b);
//	A.Insert(c);
//	A.Insert(d);
//	A.Insert(e);
//	A.Insert(f);

//	A.show();
	//cout<<endl;
	//cout<<"Delete:"<<endl;
	
	//A.Delete(c);
	cout<<"Insert: "<<endl<<endl;
	A.show();
	cout<<endl;
	cout<<"Delete: "<<endl<<endl;
	A.Delete(14);
	A.show();
//	char* asd = NULL;
//	if(asd)	NULL = no...
//		cout<<"es null"<<endl;
//	if(!asd)
//		cout<<"no es null"<<endl;
	
//------------------------------------------------------------------------------
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}
