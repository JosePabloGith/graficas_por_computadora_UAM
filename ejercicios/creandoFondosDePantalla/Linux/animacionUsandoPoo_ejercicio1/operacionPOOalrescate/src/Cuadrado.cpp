/*
Clase Concreta: Cuadrado
Rol: Es el objeto real que veremos en pantalla. Hereda de EntidadFisica.

Equivalencia Java: public class Cuadrado extends EntidadFisica

Lógica Única:

Implementa dibujar() usando glRectf.

Implementa actualizar() sumando velocidad a la posición.

Método Nuevo: checarColision(Cuadrado* otro). Aquí vivirá el algoritmo AABB que discutimos. Retornará true si choca con otro cuadrado.

Método Nuevo: rebotar(). Intercambia velocidades.

Aqui es donde debemos de implementar el algoritmo AABB

*/
#include "Cuadrado.h"

#include <cmath> //para el valor absoluto
#include"glut.h" //la paqueteria de gl-- al final
/*CONSTRUCTORES*/
// defecto
Cuadrado::Cuadrado() {
	//no pasa nada :D
}

/*CONSTRUCTOR COMPLETO*/
// Nota cómo llenamos las variables del padre (x, y, velX...)
Cuadrado::Cuadrado(
	float startX, float startY, float s,
	float vx, float vy,
	float r, float g, float b) {

	x = startX;
	y = startY;
	tamano = s;
	velX = vx;
	velY = vy;
	setColor(r, g, b);
}

/*METODOS HEREDADOS*/
// usamos las variables del padre (protected)
void Cuadrado::dibujar() {
	//usamos las variables del papá, #recuerda protected#
	glColor3f(colorR, colorG, colorB);
	glRectf(x, y, x + tamano, y + tamano);
}

void Cuadrado::actualizar() {
	// 1. Movimiento básico (Esto se queda igual)
	x += velX;
	y += velY;

	// 2. Rebote con PAREDES (¡AQUÍ ESTÁ EL CAMBIO!)

	// --- EJE X (Derecha e Izquierda) ---

	// Pared Derecha (Límite 800)
	if (x + tamano > 800) {
		x = 800 - tamano; // <--- CORRECCIÓN: Lo "teletransportamos" justo al borde
		velX *= -1;       // Y luego invertimos la velocidad
	}

	// Pared Izquierda (Límite 0)
	if (x < 0) {
		x = 0;            // <--- CORRECCIÓN: Lo pegamos al inicio
		velX *= -1;
	}

	// --- EJE Y (Arriba y Abajo) ---

	// Pared Superior (Límite 600)
	if (y + tamano > 600) {
		y = 600 - tamano; // <--- CORRECCIÓN: Lo sacamos del techo
		velY *= -1;
	}

	// Pared Inferior (Límite 0)
	if (y < 0) {
		y = 0;            // <--- CORRECCIÓN: Lo sacamos del suelo
		velY *= -1;
	}
}

/*
Algoritmo AABB
*/

bool Cuadrado::checarColision(Cuadrado* otro) {
// primero buscamos si no hay huecos
// usamos -> porque otro es un puntero
	bool colisionX = (x < otro->x + otro->tamano) && (x + tamano > otro->x);
	bool colisionY = (y < otro->y + otro->tamano) && (y + tamano > otro->y);

	return colisionX && colisionY;
}

void Cuadrado::rebotar(Cuadrado* otro) {
	//1- intercambio de velocidades, efecto villar

	//variables auxiliares para guardar las velocidades anteriores
		float tempVX = velX;
	float tempVY = velY;

	//asignamos las nuevas velocidades
		velX = otro->velX;
	velY = otro->velY;

	//las velocidades anteriores se las asignamos al otro objeto
		otro->velX = tempVX;
	otro->velY = tempVY;

	/*
	Paso 2 : Correccion de posicion (El empujon para no quedarse pegados)
	movemos ambos objetos un pasito pequeño en su nueva direccion
	para asegurarnos de que ya no se estén tocando en el siguiente frame
	*/

	x += velX; //inmediatamente, forzamos a que den un paso extra en ese mismo instante para separarse.
	y += velY;
	otro->x += otro->velX; // Él se mueve un paso YA con su nueva velocidad
	otro->y += otro->velY;
}

/*
Análisis "Javero" de este código:
Cuadrado::: ¿Ves esto antes de cada función? En Java no se usa porque el código está dentro
de la clase. En C++, como el archivo está separado, debes decirle al compilador:
"Oye, esta función dibujar pertenece a la clase Cuadrado".

otro->x: Como recibimos Cuadrado* otro (puntero), 
no usamos punto (.). Usamos flecha (->) para acceder a sus variables.
Es la forma de C++ de decir "Ve a la dirección de memoria de 'otro' y dame su valor X".

*/