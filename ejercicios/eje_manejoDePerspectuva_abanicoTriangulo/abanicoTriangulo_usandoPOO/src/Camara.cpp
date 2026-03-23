#include "Camara.h"

Camara::Camara() {
	// Posición inicial (Lejos, en Z negativo, igual que el profe)
	ojoX = 0.0f;
	ojoY = 0.0f;
	ojoZ = -120.0f;

	// Miramos al origen
	centroX = 0.0f;
	centroY = 0.0f;
	centroZ = 0.0f;
}

void Camara::activar() {
	// Aquí es donde aplicamos el "Tripié"
	// 1. Dónde estoy (ojo)
	// 2. Qué miro (centro)
	// 3. Dónde es arriba (0,1,0 -> Eje Y)
	gluLookAt(ojoX, ojoY, ojoZ, centroX, centroY, centroZ, 0.0, 1.0, 0.0);
}

void Camara::zoom(float delta) {
	// Lógica del profesor: Multiplicar las coordenadas por un factor
	// Si delta es positivo, aumenta la distancia (Alejar)
	// Si delta es negativo, reduce la distancia (Acercar)

	float factor = 1.0f + delta;

	ojoX *= factor;
	ojoY *= factor;
	ojoZ *= factor;
}

void Camara::orbitar(float delta) {
	// Lógica del profesor (Matemática vectorial para rotar en el plano XZ)
	// No te preocupes si no entiendes las formulas exactas ahora,
	// básicamente mueve el punto (x,z) en un arco.

	float dx = -ojoZ;
	float dz = ojoX;

	// Calculamos la distancia actual (Radio)
	float radio = sqrtf(ojoX*ojoX + ojoY * ojoY + ojoZ * ojoZ);

	// Movemos el punto un poquito
	ojoX += delta * dx;
	ojoZ += delta * dz;

	// Corregimos la distancia para que siga siendo un círculo perfecto
	// (Normalización)
	float nuevoRadio = sqrtf(ojoX*ojoX + ojoY * ojoY + ojoZ * ojoZ);
	float correccion = nuevoRadio / radio;

	ojoX /= correccion;
	ojoY /= correccion; // Aunque Y casi no cambia en este giro
	ojoZ /= correccion;
}