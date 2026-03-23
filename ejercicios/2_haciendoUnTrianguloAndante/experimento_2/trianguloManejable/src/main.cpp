/*
segunda parte, objetivo: Lograr que el triangulito se desplace como un pecesito
Solución de movimiento, perspectiva y Double Buffering.
*/

#include "glut.h"

/*Prototipos de funciones a usar*/
void iniciarVentana(int *argc, char** argv);
void configurarEntorno();
void dibujarEsena();
void procesarTeclasEspeciales(int key, int x, int y);


/*area de variables globales*/
// Ahora usamos notación de punto (.) para decimales en C++
float posX = 0.0f;
float posY = 0.0f;
float angulo = 0.0f;


int main(int argc , char** argv)
{
    //1.- Iniciamos la ventana
	iniciarVentana(&argc, argv);
	configurarEntorno();
	glutMainLoop();
	return 0;
}


/*
La idea detras de esta funcion es pintar el cuadro actual de la animación
*/
void dibujarEsena() {
	// limpiamos la basura (Color y Z-Buffer por buenas prácticas)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Guardamos la matriz de identidad limpia PUSH
	glPushMatrix();

	// Aplicamos las transformaciones (El orden importa: Primero trasladar, luego rotar)
	glTranslatef(posX, posY, 0.0f);
	glRotatef(angulo, 0.0f, 0.0f, 1.0f);

	// Dibujamos el gusanito / pececito
	// Se ajustaron las coordenadas para que el objeto tenga un tamaño
    // proporcional (0.1f) y su centro de rotación quede equilibrado.
	glColor3f(0.0f, 1.0f, 0.0f); // Verde
	glBegin(GL_TRIANGLES); 
	glVertex2f(0.0f, 0.1f);      // nariz del pez (apuntando al norte)
	glVertex2f(-0.05f, -0.05f);  // aleta izquierda
	glVertex2f(0.05f, -0.05f);   // aleta derecha
	glEnd();

	// Restauramos el universo limpio POP
	glPopMatrix();

	// IMPORTANTE: Sustituimos glFlush() por SwapBuffers para la animación (Red Book)
	glutSwapBuffers();
}

void configurarEntorno() {
	// fondo negro
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Aseguramos que estamos trabajando sobre la matriz de vista y modelo
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void iniciarVentana(int *argc, char **argv) {
	glutInit(argc, argv);
    
    // IMPORTANTE: Cambiamos GLUT_SINGLE por GLUT_DOUBLE para habilitar animaciones suaves
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Triangulito Pescadoso");
	
	glutDisplayFunc(dibujarEsena);
	
	// metemos a la funcion que esta siempre al pendiente de las flechitas 
	glutSpecialFunc(procesarTeclasEspeciales);
}

/*
Modulo de interacción (entrada | input)
Resuelve el Punto 10: Controles Básicos
*/
void procesarTeclasEspeciales(int key, int x , int y) {
    // Redujimos la velocidad de 1.0f a 0.05f para que el avance sea visible
    // y no se salga de las coordenadas base de OpenGL (-1.0 a 1.0).
	float velocidad = 0.05f; 
	
	switch (key) {
	case GLUT_KEY_UP:
		posY = posY + velocidad; 
		angulo = 0.0f;
		break;
	case GLUT_KEY_DOWN:
		posY = posY - velocidad;
		angulo = 180.0f;
		break;
	case GLUT_KEY_RIGHT:
		posX = posX + velocidad; 
		angulo = -90.0f;
		break;
	case GLUT_KEY_LEFT:
		posX = posX - velocidad;
        // CORRECCIÓN: Giro positivo de 90 grados hacia la izquierda
		angulo = 90.0f;
		break;
	}
    
    // Notificar a GLUT que las variables cambiaron y debe repintar
    glutPostRedisplay();
}
