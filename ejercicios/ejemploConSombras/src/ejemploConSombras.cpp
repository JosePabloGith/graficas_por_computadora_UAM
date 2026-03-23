#include <math.h>
#include <stdio.h>
#include"glut.h"

// ---------------- VARIABLES GLOBALES ---------------- //

// 1. Control de CÁMARA (Sistema Polar para efecto Drone)
float camDist = 10.0f;  // Distancia inicial
float camAngleX = 0.0f; // Rotación horizontal
float camAngleY = 0.0f; // Rotación vertical
int lastMouseX, lastMouseY; // Para rastrear el arrastre del mouse
//bool isDragging = false;

int interactionMode = 0; // 0 = Nada, 1 = Moviendo CÁMARA, 2 = Moviendo CUBO

bool animacionActiva = false; // El interruptor inicia apagado

// 2. Control de LUZ (El Foco)
// Posición inicial: Arriba y a la izquierda
float lightPos[4] = { -4.0f, 4.0f, 4.0f, 1.0f };

// 3. Definición del PISO (Plano matemático)
// Ecuación: 0x + 1y + 0z + 2 = 0  (Piso en Y = -2.0)
GLfloat floorPlane[4] = { 0.0f, 1.0f, 0.0f, 2.0f };

static GLfloat theta[] = { 0.0f, 0.0f, 0.0f }; // Ángulos de rotación del cubo

// ---------------- GEOMETRÍA DEL CUBO (LEGACY) ---------------- //

void cara0() {
	glShadeModel(GL_FLAT);
	glBegin(GL_POLYGON);
	glVertex3f(1., 1., 0);
	glVertex3f(1., -1., 0);
	glVertex3f(-1., -1., 0);
	glVertex3f(-1., 1., 0);
	glEnd();
}

void cara1() { glPushMatrix(); glTranslatef(0., 0., 1.); cara0(); glPopMatrix(); }
void cara2() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara1(); glPopMatrix(); }
void cara3() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara2(); glPopMatrix(); }
void cara4() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara3(); glPopMatrix(); }
void cara5() { glPushMatrix(); glRotatef(90., 0., 1., 0.); cara1(); glPopMatrix(); }
void cara6() { glPushMatrix(); glRotatef(-90., 0., 1., 0.); cara1(); glPopMatrix(); }

// Cubo CON color (Para el objeto real)
void colorcube(void) {
	glColor3f(0.5, 0., 0.); cara1();
	glColor3f(0., 0.5, 0.); cara2();
	glColor3f(0., 0., 0.5); cara3();
	glColor3f(0., 0.5, 0.5); cara4();
	glColor3f(0.5, 0.5, 0.); cara5();
	glColor3f(0.5, 0., 0.5); cara6();
}

// Cubo SIN color (Para la sombra negra)
void colorcube_sin_color(void) {
	cara1(); cara2(); cara3(); cara4(); cara5(); cara6();
}

// ---------------- FUNCIONES AUXILIARES ---------------- //

// Dibuja el piso gris y una rejilla para referencia
void DibujarPiso() {
	// Superficie sólida
	glColor3f(0.6f, 0.6f, 0.6f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-10.0f, -2.0f, -10.0f);
	glVertex3f(-10.0f, -2.0f, 10.0f);
	glVertex3f(10.0f, -2.0f, 10.0f);
	glVertex3f(10.0f, -2.0f, -10.0f);
	glEnd();
}

// Dibuja una esfera amarilla donde está la luz (para saber qué movemos)
void DibujarFoco() {
	glPushMatrix();
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
}

// MATEMÁTICA DE SOMBRAS (Planar Shadow Matrix)
// Fuente: Diapositiva 13 del curso
void gltMakeShadowMatrix(GLfloat vPlaneEquation[], GLfloat vLightPos[], GLfloat destMat[]) {
	GLfloat dot;
	dot = vPlaneEquation[0] * vLightPos[0] + vPlaneEquation[1] * vLightPos[1] +
		vPlaneEquation[2] * vLightPos[2] + vPlaneEquation[3] * vLightPos[3];

	destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
	destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
	destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
	destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];

	destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
	destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
	destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
	destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];

	destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
	destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
	destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
	destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];

	destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
	destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
	destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
	destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}

// ---------------- CONTROL DE USUARIO (INPUTS) ---------------- //

// Teclado Normal: W/S para Zoom
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': case 'W': // Zoom In
		if (camDist > 3.0f) camDist -= 0.5f;
		break;
	case 's': case 'S': // Zoom Out
		camDist += 0.5f;
		break;
	case 27: // ESC para salir
		exit(0);
		break;
	}
	glutPostRedisplay();
}

// Flechas: Mover la Luz
void specialKeys(int key, int x, int y) {
	float step = 0.5f;
	switch (key) {
	case GLUT_KEY_UP:    lightPos[2] -= step; break; // Z Atrás
	case GLUT_KEY_DOWN:  lightPos[2] += step; break; // Z Adelante
	case GLUT_KEY_LEFT:  lightPos[0] -= step; break; // X Izquierda
	case GLUT_KEY_RIGHT: lightPos[0] += step; break; // X Derecha
	}
	glutPostRedisplay();
}

// Mouse: Click para empezar a arrastrar
void mouseButton(int button, int state, int x, int y) {
	// Guardamos la posición inicial siempre que se presiona un botón
	if (state == GLUT_DOWN) {
		lastMouseX = x;
		lastMouseY = y;
	}

	// Lógica de selección de MODO
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			interactionMode = 1; // Modo 1: Mover Cámara (Drone)
		}
		else {
			interactionMode = 0; // Soltar: Dejar de mover
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			interactionMode = 2; // Modo 2: Girar CUBO (Trackball)
		}
		else {
			interactionMode = 0; // Soltar: Dejar de mover
		}
	}
}

void mouseMove(int x, int y) {
	// Si no estamos haciendo nada, salimos
	if (interactionMode == 0) return;

	// Calcular cuánto se movió el mouse desde el último cuadro
	int dx = x - lastMouseX;
	int dy = y - lastMouseY;

	// CASO 1: MOVEMOS LA CÁMARA (Clic Izquierdo)
	if (interactionMode == 1) {
		camAngleX += dx * 0.5f;
		camAngleY += dy * 0.5f;

		// Limites para que la cámara no de vueltas locas verticales
		if (camAngleY > 89.0f) camAngleY = 89.0f;
		if (camAngleY < -89.0f) camAngleY = -89.0f;
	}

	// CASO 2: GIRAMOS EL CUBO (Clic Derecho) -> ¡Lo que tú pediste!
	if (interactionMode == 2) {
		// Nota la matemática cruzada:
		// Mover mouse en X (horizontal) -> Gira el cubo en eje Y (Vertical)
		// Mover mouse en Y (vertical) -> Gira el cubo en eje X (Horizontal)
		theta[1] += dx * 0.5f;
		theta[0] += dy * 0.5f;
	}

	// Actualizamos la posición "anterior" para el siguiente cuadro
	lastMouseX = x;
	lastMouseY = y;

	// ¡Obligatorio! Pedir a OpenGL que dibuje la escena con los nuevos datos
	glutPostRedisplay();
}

// ---------------- RENDERIZADO PRINCIPAL ---------------- //

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// 1. CÁMARA (Drone) - Se mueve con Clic IZQUIERDO
	float radX = camAngleX * 3.14159f / 180.0f;
	float radY = camAngleY * 3.14159f / 180.0f;
	float camX = camDist * sin(radX) * cos(radY);
	float camY = camDist * sin(radY);
	float camZ = camDist * cos(radX) * cos(radY);

	gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// 2. DIBUJAR FOCO Y PISO
	DibujarFoco();
	DibujarPiso();

	// 3. DIBUJAR LA SOMBRA (Debe rotar igual que el cubo)
	GLfloat sombraMat[16];
	glDisable(GL_DEPTH_TEST); // Evitar parpadeo

	glPushMatrix();
	gltMakeShadowMatrix(floorPlane, lightPos, sombraMat);
	glMultMatrixf(sombraMat); // 1. Aplastamos

	// --- CORRECCIÓN AQUÍ: Rotación Dinámica para la Sombra ---
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	// ---------------------------------------------------------

	glColor3f(0.0f, 0.0f, 0.0f);
	colorcube_sin_color();
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

	// 4. DIBUJAR OBJETO REAL (Debe obedecer al mouse)
	glPushMatrix();
	// --- CORRECCIÓN AQUÍ: Quitamos el "30.0" fijo y ponemos tus variables ---
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	// -----------------------------------------------------------------------

	colorcube();
	glPopMatrix();

	glutSwapBuffers();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f); // Perspectiva real
	glMatrixMode(GL_MODELVIEW);
}

void spinCube()
{
	// Solo giramos si el interruptor está encendido
	if (animacionActiva) {
		theta[0] += 1.0f; // Gira en X
		theta[1] += 0.5f; // Gira en Y un poco más lento
		if (theta[0] > 360.0) theta[0] -= 360.0;
		if (theta[1] > 360.0) theta[1] -= 360.0;

		glutPostRedisplay(); // ¡Dibuja de nuevo!
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Proyecto Graficas: Sombras + Camara Drone");

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);

	// Registro de controles
	glutKeyboardFunc(keyboard); // Teclas W, S
	glutSpecialFunc(specialKeys); // Flechas
	glutMouseFunc(mouseButton); // Click
	glutMotionFunc(mouseMove);  // Arrastre
	//glutIdleFunc(spinCube); // Esta función se ejecuta siempre que la compu descansa

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}