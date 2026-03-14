#include <glew.h>
#include <glut.h>
#include <iostream>
//#include <math_3d.h>
#include <stdio.h>
#include <math.h>
using namespace std;

/**************************************************************
*                                                            *
* This program demonstrates alteration                       *
* of menus in OpenGL and GLUT.                                *
*                                                            *
**************************************************************/


#include <stdlib.h>

int menu2D, menu3D;
int pointsChosen;
GLsizei ysize;

void opciones3D(GLint opcion_seleccionada);

void opciones2D(GLint opcion_seleccionada)
{
	switch (opcion_seleccionada)
	{
	case 1: cout << "Se agrego Cubo" << endl;
		
		break;	

	case 2: cout << "Es Modo 2D. Para cambiarlo a 3D haz clic boton mediano" << endl;		
		
		break;		
	}
	glutPostRedisplay();
}

void opciones3D(GLint opcion_seleccionada)
{
	switch (opcion_seleccionada)
	{
	case 1: cout << "se apago la luz" << endl;
		break;

	case 2: cout << "Es Modo 3D. Para cambiarlo a 2D haz clic boton mediano" << endl;		
		break;
	}
	glutPostRedisplay();
}

#define SIZE 300
void setupRC()
/* This is the routine that initializes some graphics parameters. */
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, SIZE - 1, 0.0, SIZE - 1);
	glClearColor(1.0, 1.0, 1.0, 0.0);  /* make the background white */
	
	pointsChosen = 0;
	ysize = SIZE - 1;
}
void display(void)
/* This is the callback function that gets executed every time the display
needs to be updated.
*/
{	
	glClear(GL_COLOR_BUFFER_BIT);	
	glFlush();
}

void resize(GLsizei w, GLsizei h)
/* This function gets called whenever the window is resized. */
{
	/* Adjust the clipping rectangle. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, w - 1, 0.0, h - 1);
	ysize = h - 1;

	/* Adjust the viewport. */
	glViewport(0, 0, w, h);
}
void mouse(int button, int status, int x, int y){
	if (button == GLUT_MIDDLE_BUTTON&&
		status == GLUT_DOWN){
		glutDetachMenu(GLUT_RIGHT_BUTTON);		
		if (glutGetMenu() == menu2D){		
			glutSetMenu(menu3D);
			cout << "menu3D=" << menu3D<<" Es Modo 3D. Para cambiarlo a 2D\n haz clic con el menu cerrado en boton mediano\n" << endl;
		}
		else if (glutGetMenu() == menu3D)
		{		
			glutSetMenu(menu2D);			
			cout << "menu2D=" << menu2D<< " Ha entrado a modo 2D. Para cambiarlo a 3D\n haz clic con el menu cerrado en boton mediano\n" << endl;
		}
		else { 
			cout << "Error de logica de alteracion de menus" << endl;
			return; 
		}
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE  | GLUT_DEPTH | GLUT_MULTISAMPLE);
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 256);
	glutCreateWindow("Vista de supervisor");//give the glut window a title	
	
	glutDisplayFunc(display);//set our display callback
	glutReshapeFunc(resize);//set out resize callback
	glutMouseFunc(mouse);
	setupRC();	
	
	menu3D = glutCreateMenu(opciones3D);
	glutAddMenuEntry("Apagar Luz", 1);
	glutAddMenuEntry("Es Modo 3D. Para cambiar a 2D haz clic boton mediano con menu cerrado", 2);

	menu2D = glutCreateMenu(opciones2D);
	glutAddMenuEntry("Agregar Cubo", 1);		
	glutAddMenuEntry("Es Modo 2D. Para cambiarlo a 3D haz clic boton mediano con menu cerrado", 2);
		
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//robot_virtual::final.Camara2->zbuf_mitad1    =(MainOpenGL)prueba0::gl;
	glutMainLoop(); //start the glut main loop

	return 0;
}
