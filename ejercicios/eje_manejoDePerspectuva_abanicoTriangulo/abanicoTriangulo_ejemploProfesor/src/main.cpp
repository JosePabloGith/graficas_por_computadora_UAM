/*
 * =========================================================================================
 * [EXPEDIENTE TÉCNICO: VISUALIZACIÓN DE PRIMITIVAS Y CONTROL DE ESTADOS]
 * Creado por: Jose Pablo Perez De Anda
 * =========================================================================================
 * * DESCRIPCIÓN GENERAL:
 * Este programa demuestra la construcción de cuerpos volumétricos (un cono) mediante el uso
 * de abanicos de triángulos. Se enfoca en la manipulación de la "Máquina de Estados" de 
 * OpenGL para gestionar la visibilidad, el rendimiento y el modelo de sombreado.
 * * * TÉCNICAS DE GRAFICACIÓN IDENTIFICADAS (Lista de Cotejo):
 * 3.  [X] Cámara controlable: Implementación de 'gluLookAt' con lógica de navegación 
 * orbital en el plano XZ y acercamientos.
 * 10. [X] Controles (Shading, Polígono, Culling, Depth): 
 * - F1: Culling (Eliminación de caras posteriores).
 * - F2: Depth Test (Prueba de profundidad/Z-buffer).
 * - F3: Polygon Mode (Alternancia entre alambre y sólido en caras posteriores).
 * - F4: Shading Model (Conmutación entre GL_FLAT y GL_SMOOTH).
 * Primitivas: Uso eficiente de 'GL_TRIANGLE_FAN' para superficies cónicas.
 * * * SUSTENTO BIBLIOGRÁFICO:
 * - Geometría: La generación de la base circular emplea la parametrización de curvas 
 * en coordenadas polares ($x=r \cdot \sin, y=r \cdot \cos$), según Thomas (Cálculo, Cap. 11).
 * - OpenGL: El "Red Book" (Cap. 2) define el 'Triangle Fan' como una estructura óptima 
 * donde el primer vértice actúa como el "ápice" o centro compartido.
 * - Álgebra: La rotación de la cámara en 'SpecialKeys' utiliza conceptos de vectores 
 * directores y ortogonalidad para mantener el radio de giro (Howard Anton, Cap. 3).
 * * * ANALOGÍA PARA EL DESARROLLADOR JAVA:
 * En Java (ej. Swing), cambiar un color implica modificar una propiedad de un objeto. 
 * En C++/OpenGL, el comando 'glColor3f' actúa como si cambiaras físicamente el cartucho 
 * de color de una impresora: todos los puntos (vértices) que se manden después saldrán 
 * de ese color hasta que decidas cambiar el cartucho por otro.
 * * * NOTA DE SEGURIDAD OPERATIVA:
 * Se ha desactivado "stdafx.h" para mejorar la compatibilidad. Si usas Linux (Mint), 
 * el encabezado <GL/glut.h> es el estándar para el Makefile.
 * =========================================================================================
 */

#include<math.h>
//#include "stdafx.h"

#define GL_PI 3.1415f
#include<GL/glut.h>  // NOTA ESTO ES HASTA EL FINAL
GLfloat xRot = 0.45f; // EST SON EXPERIMENTALES :d
GLfloat yRot = 0.35f;
GLboolean bCull = glIsEnabled(GL_CULL_FACE);
GLboolean bDepth = glIsEnabled(GL_DEPTH_TEST);
GLboolean bOutline = (GLboolean)true;

GLenum shademode = GL_FLAT;

GLfloat ex = 0.0f;
GLfloat ey = 0.0f;
GLfloat ez = -120.0f;
GLfloat delta = 0.01f;
GLfloat deltaR = 0.01f;


///////////////////////////////////////////////////////////


void SetupRC()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);

	// Set color shading model to flat
	glShadeModel(shademode);

	// Clockwise-wound polygons are front facing; this is reversed
	// because we are using triangle fans
	glFrontFace(GL_CW);
}

// Called to draw scene

void RenderScene(void)
{
	// Reset coordinate system

	glLoadIdentity(); // 

	//ojo con esete bicho :O
	//en total son 9 parametros
	//las primeras 3 para indicar el ojo
	// las otras 3 hacia adonde mira el ojo
	//las ultimas 3 es para indicar adonde es arriba, adonde es abajo
													gluLookAt(ex, ey, ez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	GLfloat x, y, angle;         // Storage for coordinates and angles
	int iPivot = 1;            // Used to flag alternating colors

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // esta cosa limpia el teñido del buffer del color
	// tambien el buffer de profundidad :O
	/*
	 piensa, si ese lente obtico choca con un objeto, tienes 2 obciones, ver lo que esta de tras o no
	 esa informacion se guarda en el buffer de profundidad
	 piensa qu renderScene se enfoca en renderizar cada cosa
	 - Cuando das un incremento de angulo de vista puchando una tecla
	 -- se debe de redibujar (ojo con eso :S)

	 RECUERDA QUE CADA TRIANGULO TIENE UNA CARA Y UN REVERSO

	 dentro del copdigo f3 hace que el cfondo del cono sea traslucido, si lo puchas otra vez lo deniegas 

	 TAREA ANALIZA QUE ES LO QUE HACE F1,F2,F3,F4
	 y extrae la informacion mas relevante para tu glosario de funciones
	 Recuerda que estas tecnicas las puedes usar en tu proyecto

	 en ese ejemplo vamos empujado las funciones 

	*/


	// Turn culling on if flag is set
	if (bCull)   glEnable(GL_CULL_FACE);
	else        glDisable(GL_CULL_FACE);

	// Enable depth testing if flag is set
	if (bDepth)  glEnable(GL_DEPTH_TEST);
	else        glDisable(GL_DEPTH_TEST);

	// Draw the back side as a wireframe only, if flag is set
	if (bOutline)glPolygonMode(GL_BACK, GL_LINE);
	else        glPolygonMode(GL_BACK, GL_FILL);

	// Save matrix state and do the rotation
	// OJO CON ESTA TRANSFORMACION
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Begin a triangle fan
	glBegin(GL_TRIANGLE_FAN);  //ojo aqui
	//se hace uso de la primitiva gl_triangile_fan
	/*
	se menciona que tiene un vertice distingido en donde los demas triangulos se pegan

	*/

	// Pinnacle of cone is shared vertex for fan, moved up z-axis
	// to produce a cone instead of a circle
	glVertex3f(0.0f, 0.0f, 75.0f); // aqui hay otro truco
	// este vertice (x , y , z) ; entonces puedes ver que z sale del plano
	//aparte del punto en comun que debe de haber de esa llamada nececitamos usar un trco

	// Loop around in a circle and specify even points along the circle
	// as the vertices of the triangle fan

	/*Este for es especial
	Todo depende del parametro del angulo, de 0 a 2pi, cada uno de esos valores determina los parametros de x y y
	la logica detras del abanico es presentar un monton de verices 
	a lo largo de cada uno de los 

	Este ciclo es para la parte superior del cono
	*/
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))

	{
		// Calculate x and y position of the next vertex
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		// Alternate color between red and green


		// esta parte es para los colores
		if ((iPivot % 2) == 0)    glColor3f(0.0f, 1.0f, 0.0f); //par // un color
		else			        glColor3f(1.0f, 0.0f, 0.0f); // impar // otro color
		/*con esa alternancia podemos colorear los diferentes triangulo de diferente color*/

		// Increment pivot to change color next time
		iPivot++; // aqui se modifica el pivote 
		//el pivote sirve para 

		// Specify the next vertex for the triangle fan
		glVertex2f(x, y);  //AQUI ESTA EL TRUCO	explicitamente tenemos coordenadas x,y,z ; pero z es cero por defecto
		// tambien hay otra oculta
	}

	// Done drawing fan for cone
	glEnd(); //ojo aqui tambien

	// Begin a new triangle fan to cover the bottom
	glBegin(GL_TRIANGLE_FAN);

	//esta parte es similar al anterior
	glVertex2f(0.0f, 0.0f);

	/*
	considera que el fondo y el vertice 
	*/
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Calculate x and y position of the next vertex
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		// Alternate color between red and green
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);

		// Increment pivot to change color next time
		iPivot++;

		// Specify the next vertex for the triangle fan
		glVertex2f(x, y);
	}

	// Done drawing the fan that covers the bottom
	glEnd();

	// Restore transformations
	glPopMatrix(); // ESTE REGRESA EL ESTADO DE TRANSFORMACIONES SIN IMPORTAR LO ANTERIOR

	// Flush drawing commands
	glutSwapBuffers();
}

// Called by GLUT library when the window has changed size

void ChangeSize(GLsizei w, GLsizei h)
{    // Set Viewport to window dimensions
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho (-100.0, 100.0, -100, 100, -270.0, 270.0);
	//glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	gluPerspective(60.0, 1.0, 1.5, 500.0);
	glMatrixMode(GL_MODELVIEW);
}


/*
ojo con la nomenclatura de estas teclitas especiales
en los archivos de especificaciones puedes tener la idea de 
los parametros
"nombre de tecla", + Parametros especiales
*/
void SpecialKeys(int key, int x, int y)
{

	/*
	TAREA. LEE EL ARCHIVO DIVICION DE VECTORES
	EN ESTA PARTE SE IMPLEMENTA LA LOGICA DE LA FORMA DE MOVIMIENTO DE LA CAMARA
	DEPENDIENDO DE HACIA DONDE LE PUCHES ↑,→,

	*/
	GLfloat dx, dz;


	if (key == GLUT_KEY_UP)
	{//increase distance from camera to origin
		ex *= (1.0f + deltaR);  ey *= (1.0f + deltaR);	ez *= (1.0f + deltaR);
	}

	if (key == GLUT_KEY_DOWN)
	{//reduce distance from camera to origin (close up)
		ex *= (1.0f - deltaR); ey *= (1.0f - deltaR);	ez *= (1.0f - deltaR);
	}

	if (key == GLUT_KEY_LEFT)
		//Rotate camera around origin in Oxz plane
	{
		dx = -ez;     dz = ex;
		GLfloat s = sqrtf(ex*ex + ey * ey + ez * ez);
		ex += delta * dx;       ez += delta * dz;
		GLfloat s1 = sqrtf(ex*ex + ey * ey + ez * ez) / s;
		ex /= s1;	ey /= s1;	ey /= s1;
	}

	if (key == GLUT_KEY_RIGHT)
		//Rotate camera around origin in Oxz plane
	{
		dx = -ez;     dz = ex;
		GLfloat s = sqrtf(ex*ex + ey * ey + ez * ez);
		ex -= delta * dx;      	ez -= delta * dz;
		GLfloat s1 = sqrtf(ex*ex + ey * ey + ez * ez) / s;
		ex /= s1;	ey /= s1;	ey /= s1;
	}


	if (key == GLUT_KEY_F1) bCull = !bCull;
	if (key == GLUT_KEY_F2)bDepth = !bDepth;
	if (key == GLUT_KEY_F3)bOutline = !bOutline;
	if (key == GLUT_KEY_F4)
	{
		if (shademode == GL_FLAT) { shademode = GL_SMOOTH; }
		else { if (shademode == GL_SMOOTH) { shademode = GL_FLAT; } };
		glShadeModel(shademode);
	}
	// Refresh the Window
	glutPostRedisplay();


	// Refresh the Window
	glutPostRedisplay();
}


///////////////////////////////////////////////////////////



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(100, 100); // esto es algo nuevo, representa la posicion de la ventanilla inicial
	glutCreateWindow(argv[0]);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys); // esta es una nueva funcion "para las teclas especiales"

	SetupRC();
	glutMainLoop();
	return 0;
}
