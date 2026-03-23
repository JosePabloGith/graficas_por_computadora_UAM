#pragma once
#include <vector>       // Librería estándar para listas dinámicas (arrays inteligentes)
#include "Entidad.h"    // Para saber qué es una Entidad
#include "glut.h"       // La herramienta de ventanas

// Forward Declaration:
// Le decimos al compilador: "Existe una clase Cubo, confía en mí".
// Esto evita problemas de dependencias circulares.
class Cubo;

/**
 * @class Motor
 * @brief Clase Singleton que administra la ventana, los eventos y la escena.
 * * RESPONSABILIDADES:
 * 1. Inicializar GLUT y OpenGL.
 * 2. Mantener la lista de objetos (la escena).
 * 3. Servir de puente entre las funciones C de GLUT y nuestro mundo C++.
 */
class Motor {

private:
	// --- PATRÓN SINGLETON ---
	/** @brief Puntero a la única instancia permitida del Motor. */
	static Motor* instancia;

	// --- PROPIEDADES DE LA VENTANA ---
	int ancho;
	int alto;

	// --- LA ESCENA ---
	/** * @brief Lista polimórfica de objetos.
	 * Gracias a que es vector<Entidad*>, podemos guardar Cubos, Esferas o Robots
	 * mezclados en la misma lista y tratarlos a todos por igual.
	 */
	std::vector<Entidad*> escena;

public:
	/**
	 * @brief Constructor.
	 * Configura el tamaño deseado de ventana, pero NO la abre todavía.
	 * Registra esta instancia en el puntero estático 'instancia'.
	 */
	Motor(int w, int h);

	/** @brief Destructor: Limpia la memoria de todos los objetos de la escena. */
	~Motor();

	// --- GESTIÓN DE LA ESCENA ---

	/** @brief Agrega un nuevo actor a la obra de teatro. */
	void agregarEntidad(Entidad* nuevaEntidad);

	/** * @brief Busca el primer Cubo de la escena.
	 * Útil para controles rápidos (mouse/teclado) sobre el objeto principal.
	 */
	Cubo* getCuboPrincipal();

	// --- CICLO DE VIDA (Fases del Programa) ---

	/**
	 * @brief Configura GLUT, la Memoria Gráfica y las luces.
	 * Equivale a todo el código que antes tenías en 'main()'.
	 */
	void initGL(int argc, char** argv);

	/** @brief Inicia el bucle infinito de GLUT (glutMainLoop). */
	void start();

	// --- LÓGICA INTERNA (Lo que GLUT manda llamar) ---

	/** @brief Borra la pantalla y pide a cada entidad que se dibuje. */
	void loopDibujo();

	/** @brief Pide a cada entidad que actualice sus matemáticas. */
	void loopLogica(int valor);

	/** @brief Procesa clics del ratón para controlar el cubo. */
	void procesarMouse(int boton, int estado, int x, int y);

	/** @brief Recalcula la perspectiva si el usuario estira la ventana. */
	void cambiarTamano(int w, int h);

	// --- WRAPPERS ESTÁTICOS (El Puente C <-> C++) ---
	// Estas son las únicas funciones que GLUT puede ver.
	// Simplemente redirigen la llamada a la instancia del Motor.
	static void drawWrapper();
	static void updateWrapper(int v);
	static void mouseWrapper(int b, int s, int x, int y);
	static void reshapeWrapper(int w, int h);
};