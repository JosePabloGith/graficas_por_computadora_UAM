/*
Clase Abstracta: EntidadFisica (ES EL PAPÁ) 
Rol: Es la "Plantilla Maestra". Define qué tienen en común todos los objetos del juego.

Equivalencia Java: public abstract class EntidadFisica
Atributos (Datos):
x, y: Posición en el mundo.
tamano: Ancho/Alto.
velocidadX, velocidadY: Dirección de movimiento.
r, g, b: Color.
Métodos (Acciones):
dibujar() (Virtual Puro): Obliga a los hijos a definir cómo pintarse.
actualizar() (Virtual Puro): Obliga a los hijos a definir cómo moverse.
setPosicion(...), getVelocidad(...): Setters y Getters clásicos para encapsulamiento.
*/

#pragma once
// #pragma once es un truco de c++ para evitar que el archivo se lea 2 veces por error.

class EntidadFisica {
protected:
	//"protected" es como private, pero permite que los hijos
	//accedan a estas variables directamente. Es util en herencia.
	float x, y;
	float tamano;
	float velX, velY;
	float colorR, colorG, colorB;
public:
	//constructor base
	// su chamba es inicializar todo en cero por seguridad
	EntidadFisica() {
		x = y = 0;
		tamano = 0;
		velX = velY = 0;
		colorR = colorG = colorB = 1.0; //blanco por defecto
	}

	//Metodos virtuales puros (la interface)
	// el 0 significa: yo no se hacerlo pero mi hijo si :D

	virtual void dibujar() = 0;
	virtual void actualizar() = 0;

	//geters y seters para acceso seguro desde el motor
	//dentro de c++ se suelen definir aquí mismo si son cortitos (Inline)

	void setPosicion(float _x , float _y) {
		x = _x;
		y = _y;
	}
	void setVelocidad(float _vx, float _vy) {
		velX = _vx;
		velY = _vy;
	}

	void setColor(float r , float g, float b) {
		colorR = r;
		colorG = g;
		colorB = b;
	}


	float getX() { return x; }
	float getY() { return y; }
	float getTamano() { return tamano; }
	float getVelX() { return velX; }
	float getVelY() { return velY; }

};