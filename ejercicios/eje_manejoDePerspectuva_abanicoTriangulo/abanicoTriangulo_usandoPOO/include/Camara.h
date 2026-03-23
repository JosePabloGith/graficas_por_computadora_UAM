#pragma once

#include <math.h>
#include "glut.h"

class Camara {
private:
	// Posición del OJO (Donde está el camarógrafo)
	//El profe usa ex,ey,ez
	float ojoX, ojoY, ojoZ;

	// A dónde miramos (El centro del escenario)
	// En este ejemplo siempre miraremos al (0,0,0), pero podríamos cambiarlo
	float centroX, centroY, centroZ;

public:
	Camara(); // Constructor

	// Esta función es la que llama a gluLookAt
	void activar();

	// --- ACCIONES DE MOVIMIENTO ---

	// Zoom: Acercarse o alejarse del centro
	void zoom(float delta);

	// Orbitar: Girar alrededor del centro (izquierda/derecha)
	void orbitar(float delta);
};