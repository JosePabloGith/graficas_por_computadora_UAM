#pragma once
#include "Entidad.h"
#include "glut.h" // Asegúrate de que glut.h esté en tu include path

/**
 * @class Cubo
 * @brief Actor geométrico que representa un cubo giratorio.
 * * Cumple con el Principio de Encapsulamiento:
 * - Estado interno privado (theta, eje).
 * - Acceso controlado mediante Getters y Setters.
 * - Validación de datos en los Setters.
 */
class Cubo : public Entidad {

private:
	// --- ESTADO INTERNO ---
	float theta[3];       // [0]=X, [1]=Y, [2]=Z
	int ejeSeleccionado;  // 0, 1, o 2

	// --- DATOS ESTÁTICOS (Geometría compartida) ---
	// Son const porque un cubo nunca cambia de forma, solo de posición.
	static const float vertices[8][3];
	static const float colores[8][3];

	// --- MÉTODOS AYUDANTES (Privados) ---
	void polygon(int a, int b, int c, int d);
	void colorcube();

public:
	Cubo();
	virtual ~Cubo() {}

	// --- INTERFAZ ENTIDAD (Overrides) ---
	void iniciar() override;
	void actualizar() override;
	void dibujar() override;

	// --- GETTERS & SETTERS (Encapsulamiento) ---

	/**
	 * @brief Define qué eje rotará automáticamente.
	 * @param nuevoEje: 0(X), 1(Y) o 2(Z).
	 * @note Incluye validación: si pasas un número inválido, no hace nada.
	 */
	void setEje(int nuevoEje);

	/** @brief Obtiene el eje que se está moviendo actualmente. */
	int getEje() const;

	/**
	 * @brief Permite forzar un ángulo específico manualmente.
	 * Útil si quieres resetear la posición (angulo = 0).
	 */
	void setAngulo(int eje, float valor);

	/** @brief Obtiene el ángulo actual de un eje. */
	float getAngulo(int eje) const;
};

/*
¿Notas la diferencia?
Agregué la palabra clave const en los getters (getEje() const).

Significado: "Prometo que al llamar a esta función de lectura, NO modificaré nada dentro del objeto".
Es una protección extra que C++ te permite para evitar bugs accidentales.

*/