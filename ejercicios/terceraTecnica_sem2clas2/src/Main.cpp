#include "Motor.h"
#include "Cubo.h"
#include "Plano.h" // 1. Incluimos la nueva cabecera

#include <iostream>

/**
 * @file Main.cpp
 * @brief Punto de entrada de la aplicación.
 * * Misión:
 * 1. Instanciar el Sistema (Motor).
 * 2. Crear los objetos (Entidades).
 * 3. Iniciar el bucle infinito.
 */

int main(int argc, char** argv) {
	std::cout << "--- INICIANDO SISTEMA GRAFICO ---" << std::endl;

	// 1. CREAR EL MOTOR (EL DIRECTOR)
	// Lo creamos en la "Pila" (Stack) porque vivirá todo el programa.
	Motor engine(800, 600);

	// 2. Instanciamos los objetos (HEAP MEMORY)
	Cubo* elCubo = new Cubo();
	Plano* elSuelo = new Plano();


	// 3. AGREGAR AL MOTOR
	// El Motor ahora es "dueño" del puntero miCubo.
	// Cuando el Motor muera, él se encargará de hacer 'delete miCubo'.
	engine.agregarEntidad(elSuelo); // Agregamos el suelo primero (para que se dibuje al fondo)
	engine.agregarEntidad(elCubo);  // Agregamos el cubo después

	// 4. CONFIGURACIÓN FINAL (OpenGL/GLUT)
	// Pasamos los argumentos de consola por si GLUT los necesita.
	engine.initGL(argc, argv);

	// 5. ¡ACCIÓN!
	// A partir de aquí, el programa entra en un bucle infinito.
	// Nada escrito debajo de esta línea se ejecutará jamás.
	engine.start();

	return 0;
}

/*

El Concepto: "Burbujas de Realidad"
Imagina que el Motor es el Dios de este universo y hace esto en cada fotograma (loopDibujo):

Reset Universal: glLoadIdentity() (Todo vuelve al centro).

Coloca la Cámara: gluLookAt(...) (El observador se sienta en su silla).

Turno del Cubo: Llama a cubo->dibujar().

El Cubo grita glPushMatrix(): "¡Creo una burbuja temporal!".

El Cubo gira (glRotate). Solo gira el espacio dentro de la burbuja.

El Cubo se dibuja.

El Cubo grita glPopMatrix(): "¡Rompo la burbuja!".

Resultado: El mundo vuelve a estar quieto y sin rotación.

Turno del Plano: Llama a plano->dibujar().

Como la burbuja del cubo ya se rompió, el plano se dibuja en el mundo "limpio" y quieto.
*/