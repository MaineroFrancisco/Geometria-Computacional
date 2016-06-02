#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include "Arbol_B.h"
#include "Interseccion.h"

using namespace std;

void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,10,0,10);
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
	
	punto a,b,c,d;
	a.x = 5; b.x = 3; c.x = 0; d.x = 10;
	a.y = 0; b.y = 10; c.y = 4; d.y = 6;
	
	segmento A,B;
	A.ini = b; B.ini = d;
	A.fin = a; B.fin = c;
	
	vector<segmento> S;
	S.push_back(A); S.push_back(B);
	
	Interseccion i;
	
	i.FindIntersection(S);
	
	vector<punto> I = i.GetIntersection();
	
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

//	int* a;
//	int b = 1;
//	
//	a=&b;
//	
//	cout<<*a<<endl;
//	
//	b = 0;
//	
//	cout<<*a<<endl;
}

int main (int argc, char **argv) {
	glutInit (&argc, argv);
	initialize();
	glutMainLoop();
	return 0;
}
