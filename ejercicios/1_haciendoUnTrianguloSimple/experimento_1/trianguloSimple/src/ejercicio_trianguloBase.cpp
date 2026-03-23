/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA 
 * ======================================================================================
 * ARCHIVO: ejercicio_trianguloBase.cpp
 * TIPO: Laboratorio Inicial / Primitivas Básicas
 * * TÉCNICAS DE GRAFICACIÓN EMPLEADAS (Ref. Red Book Cap. 1 & 2):
 * 1. MODO INMEDIATO: Uso de glBegin() / glEnd() para el envío de vértices a la GPU.
 * 2. RASTERIZACIÓN DE PRIMITIVAS: Implementación de GL_TRIANGLES (rasterización de 
 * tres vértices para formar una superficie rellena).
 * 3. ESTADO DE COLOR (RGB): Uso de glColor3f como máquina de estados (el color 
 * "mojado" en el pincel se mantiene hasta cambiarlo).
 * 4. GESTIÓN DE VENTANAS (GLUT): Abstracción del sistema operativo para manejo de 
 * contextos gráficos y el Event Loop (glutMainLoop).
 * * RELACIÓN CON OBJETIVOS DEL PROYECTO (1-13):
 * - [10] CONTROLES (Inicialización): Establece el color de limpieza (glClearColor) 
 * y la configuración básica del viewport.
 * ======================================================================================
 */

//#include <iostream>
#include <GL/glut.h>

/*Prototipos de funciones a usar*/
void iniciarVentana(int *argc, char** argv);
void configurarEntorno();
void dibujarEsena();

int main(int argc , char** argv)
{
    //1.- Iniciamos la ventana
	iniciarVentana(&argc, argv);
	configurarEntorno();
	glutMainLoop();
	return 0;
}


/*
La idea detras de esta funcion es 
*/
void dibujarEsena() {
	//limpiamos la basura
	glClear(GL_COLOR_BUFFER_BIT);

	// elegimos el color del pincel (rojo R=1 , G=0 , B = 0)
	// con la idea de que estamos mojando el pincel
	glColor3f(1.0f, 0.0f , 0.0f);


	/*Aqui empezaremos a dibujar, pensando que a opengl 
	le indicaremos los 3 puntos en donde debe de comenzar a unir los puntos para formar un triangulo*/
	glBegin(GL_TRIANGLES);

	// *1.- vertice 1 : arriba en el centro x=0 , y = 0.5
	glVertex2f(0.0f , 0.5f);
	// *2.- vertice 2 : abajo a la izquierda
	glVertex2f(-0.5f, -0.5f);
	// *3.- vertice 3 : abajo a la derecha
	glVertex2f(0.5f, -0.5f);

	//Ahora terminamos de dibujar
	glEnd();

	//forzamos la orden para forzar el dibujo
	glFlush();


}

void configurarEntorno() {
	// fondo negro
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void iniciarVentana(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Triangulito Modular");

	glutDisplayFunc(dibujarEsena);

}
