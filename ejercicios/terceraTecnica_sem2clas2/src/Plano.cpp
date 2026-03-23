#include "Plano.h"

Plano::Plano() {
	// Constructor vacío (no hay variables que inicializar por ahora)
}

Plano::~Plano() {
	// Destructor vacío (no reservamos memoria dinámica interna)
}

void Plano::iniciar() {
	// Aquí podríamos cargar una textura de pasto o pavimento en el futuro.
}

void Plano::actualizar() {
	// El plano es estático (inmóvil). 
	// No gastamos CPU calculando física aquí.
}

void Plano::dibujar() {
	// Buena práctica POO: Siempre aislar la matriz, aunque sea el suelo.
	glPushMatrix();

	// Vamos a dibujar líneas blancas
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f); // Blanco puro

	// Dibujamos una rejilla de 10x10 unidades
	// i va desde -5 hasta 5, saltando de 1 en 1
	for (float i = -5.0f; i <= 5.0f; i += 1.0f) {

		// Líneas paralelas al eje X (Suelo)
		// Se dibujan en Y = -1.0 para que el cubo (que está en 0,0,0) flote sobre ellas
		glVertex3f(-5.0f, -1.0f, i);
		glVertex3f(5.0f, -1.0f, i);

		// Líneas paralelas al eje Z (Profundidad)
		glVertex3f(i, -1.0f, -5.0f);
		glVertex3f(i, -1.0f, 5.0f);
	}

	glEnd();

	glPopMatrix();
}