/*
Archivo 1: Entidad.h
Ubicación: Archivos de encabezado (.h) Concepto: Es el "Padre" de todos. 
En Java esto sería una interface o una abstract class. Define que cualquier cosa que quiera
vivir en tu mundo 3D (un cubo, una esfera, un robot) está obligada a tener 
un método init, update y draw.
*/

#pragma once

/**
 * @class Entidad
 * @brief Clase base abstracta (Interfaz) para todos los objetos del mundo 3D.
 * * Define el contrato que deben firmar todos los actores (Cubo, Esfera, Robot).
 * Al ser abstracta, no se pueden crear objetos de tipo "Entidad" directamente,
 * solo de sus hijos.
 */
class Entidad {
public:

	/**
		 * @brief Destructor Virtual.
		 * * @note Es OBLIGATORIO que sea virtual.
		 * ¿Por qué? Si borras un (Entidad* e = new Cubo()), C++ necesita saber
		 * que primero debe llamar al destructor de Cubo y luego al de Entidad.
		 * Sin 'virtual', solo borraría la parte de Entidad y dejaría basura en RAM.
		 */
	virtual ~Entidad() {} // Destructor virtual importante

	// Métodos virtuales puros (= 0)
	// Obligan a los hijos a implementar su propia versión

	/**
	 * @brief Configuración inicial del objeto.
	 * * Se debe llamar una sola vez al principio.
	 * Úsalo para cargar texturas, definir colores iniciales o posiciones de arranque.
	 */
	virtual void iniciar() = 0;          // Configurar valores iniciales
	
	/**
	 * @brief Ciclo de Lógica y Matemáticas (La Mente).
	 * * Aquí NO se dibuja nada. Aquí se calculan los cambios:
	 * - Aumentar ángulos de rotación.
	 * - Calcular colisiones.
	 * - Mover posiciones (x, y, z).
	 */
	virtual void dibujar() = 0;

	/**
	 * @brief Ciclo de Renderizado (El Pincel).
	 * * Aquí van exclusivamente las llamadas a OpenGL (glBegin, glEnd, glRotate).
	 * Se llama automáticamante cada vez que la pantalla necesita repintarse.
	 */
	virtual void actualizar() = 0;
};