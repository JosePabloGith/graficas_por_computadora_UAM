#include "Cono.h"

// 1. CONSTRUCTOR
Cono::Cono() {
	// Inicializamos la rotación como en el ejemplo del profe
	xRot = 0.0f;
	yRot = 0.0f;
}

// 2. MÉTODO PARA MODIFICAR LA ROTACIÓN
void Cono::rotar(float x, float y) {
	xRot += x;
	yRot += y;
}

// 3. EL DIBUJO (La parte pesada rescatada del profesor)
void Cono::dibujar() {

	// Guardamos la matriz actual antes de rotar (¡Muy importante en 3D!)
	glPushMatrix();

	// Aplicamos la rotación propia de este objeto
	glRotatef(xRot, 1.0f, 0.0f, 0.0f); // Rotar en eje X
	glRotatef(yRot, 0.0f, 1.0f, 0.0f); // Rotar en eje Y

	// --- AQUÍ EMPIEZA LA GEOMETRÍA DEL PROFESOR ---

	GLfloat x, y, angle;
	int iPivot = 1;

	// PARTE 1: El Abanico Superior (El Cono en sí)
	glBegin(GL_TRIANGLE_FAN);
	// La punta del cono (Vértice compartido)
	glVertex3f(0.0f, 0.0f, 75.0f);

	// El ciclo para la base circular
	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f)) {
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);

		// Alternar colores (Verde y Rojo)
		if ((iPivot % 2) == 0) glColor3f(0.0f, 1.0f, 0.0f);
		else                   glColor3f(1.0f, 0.0f, 0.0f);

		iPivot++;
		glVertex2f(x, y); // Dibuja el punto en la base
	}
	glEnd();

	// PARTE 2: La Tapa Inferior (El círculo de abajo)
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f); // Centro de la tapa

	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f)) {
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);

		if ((iPivot % 2) == 0) glColor3f(0.0f, 1.0f, 0.0f);
		else                   glColor3f(1.0f, 0.0f, 0.0f);

		iPivot++;
		glVertex2f(x, y);
	}
	glEnd();
	// ---------------------------------------------

	// Restauramos la matriz para no afectar a otros objetos futuros
	glPopMatrix();
}