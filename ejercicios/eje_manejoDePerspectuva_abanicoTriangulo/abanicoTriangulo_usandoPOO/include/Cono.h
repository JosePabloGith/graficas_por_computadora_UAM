#pragma once
#include <math.h> // Para senos y cosenos
#include "glut.h"

// Definimos PI si no existe (por seguridad)
#ifndef GL_PI
#define GL_PI 3.1415f
#endif

class Cono {
private:
	// Propiedades únicas del objeto
	float xRot;
	float yRot;

public:
	Cono();  // Constructor (Aquí pondremos los valores iniciales)

	void dibujar();              // Aquí va la geometría (Triángulos)
	void rotar(float x, float y); // Para moverlo después
};