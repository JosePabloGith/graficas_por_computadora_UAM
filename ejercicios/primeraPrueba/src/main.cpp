
/*
Primer ejercicio, este consta solo de ver si ya tienes correctamente instalado la suit de visual studio 2017
ademas de hacer que el ejemplo de la pelota rebotando se ejecute colocando los archivos 
glut.h, glt32.dll , glut32.lib , descartando glut.def y el readme que viene
SALUDOS ALUMNO, BIENVENIDO AL TRIMESTRE 26I
*/

// OPEN_GL_test.cpp : Defines the entry point for the console application.
//

// siempre recomendado comentar esto, amenos que sea linux #include "stdafx.h"
#include <GL/glut.h>

// Initial square position and size

GLfloat posX = 0.0f;
GLfloat posY = 0.0f;
GLfloat rsize = 25;

// Step size in x and y directions
// (number of pixels to move each time)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Keep track of windows changing width and height
GLfloat windowWidth;
GLfloat windowHeight;

///////////////////////////////////////////////////////////

// Called to draw scene

void RenderScene(void)
{
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);
	// Set current drawing color to red
 //           R     G       B
	glColor3f(1.0f, 0.0f, 0.0f);

	// Draw a filled rectangle with current color
	glRectf(posX, posY, posX + rsize, posY - rsize);

	// Flush drawing commands and swap
	glutSwapBuffers();
}
///////////////////////////////////////////////////////////

// Called by GLUT library when idle (window not being
// resized or moved)

void TimerFunction(int value)
{    // Reverse direction when you reach left or right edge
	if (posX > windowWidth - rsize || posX < -windowWidth)
		xstep = -xstep;

	// Reverse direction when you reach top or bottom edge
	if (posY > windowHeight || posY < -windowHeight + rsize)
		ystep = -ystep;
	// Actually move the square
	posX += xstep;
	posY += ystep;

	// Check bounds. This is in case the window is made
	// smaller while the rectangle is bouncing and the
	// rectangle suddenly finds itself outside the new
	// clipping volume
	if (posX > (windowWidth - rsize + xstep))
		posX = windowWidth - rsize - 1;
	else if (posX < -(windowWidth + xstep))
		posX = -windowWidth - 1;
	if (posY > (windowHeight + ystep))
		posY = windowHeight - 1;
	else if (posY < -windowHeight + rsize - ystep)
		posY = -windowHeight + rsize - 1;

	// Redraw the scene with new coordinates
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

///////////////////////////////////////////////////////////
// Setup the rendering state

void SetupRC(void)
{    // Set clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
///////////////////////////////////////////////////////////
// Called by GLUT library when the window has changed size

void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	/*glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();*/
}

///////////////////////////////////////////////////////////

//int _tmain(int argc, _TCHAR* argv[])
//{
//glutInit(&argc, (char **)argv);
// glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
// glutCreateWindow ((char *)argv[0]);
//    //glutCreateWindow("Bounce");
//    glutDisplayFunc(RenderScene);
//    glutReshapeFunc(ChangeSize);
//    glutTimerFunc(33, TimerFunction, 1);
//    SetupRC();
//    glutMainLoop();
//return 0;
//}

int main(int argc ,char* argv[])
{
	//llamamos a glut init antes de cualquier otra funcion
	glutInit(&argc, argv);
	//configuramos la ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//le damos nombre a la ventana
	glutCreateWindow("Bounce");
	//registro de callbacks
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	//nuestra configuracion de renderizado
	SetupRC();
	//entramos al ciclo principal
	glutMainLoop();
	return 0;
}
