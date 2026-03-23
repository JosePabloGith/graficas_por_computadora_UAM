/*
Archivo 3: Cubo.cpp (La Implementación)
Aquí es donde ocurre la magia real. Vamos a pegar la lógica matemática de tu profesor, pero observa
estos 3 detalles clave que son diferentes a Java:

El apellido Cubo::: En cada función debemos escribir void Cubo::dibujar(). Si no lo hacemos, 
C++ pensará que es una función huérfana y no tendrá acceso a las variables privadas theta o eje.

Inicialización Estática: Los arrays de vértices y colores son static. En C++, no se pueden rellenar 
dentro del .h. Se deben rellenar al principio del .cpp, fuera de cualquier función.

Matemáticas encapsuladas: Fíjate en los métodos setEje y actualizar. Ahí verás cómo protegemos los
datos para que nadie rompa el cubo poniendo un eje "500".

*/

#include "Cubo.h"
#include <stdio.h> // Para printf si quisieras depurar

// -------------------------------------------------------------------------
// 1. INICIALIZACIÓN DE DATOS ESTÁTICOS (La Geometría)
// -------------------------------------------------------------------------
// Estos datos son compartidos por TODOS los cubos. Se definen una sola vez.


/**
 * @var vertices
 * @brief Mapa de coordenadas geométricas de un cubo de radio 1.
 * * TEORÍA:
 * Un cubo tiene 8 esquinas (v0 a v7).
 * El centro del cubo está en (0,0,0).
 * Cada fila representa {x, y, z}.
 */
const float Cubo::vertices[8][3] = {
	{-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
	{-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0}, {1.0,1.0, 1.0}, {-1.0,1.0, 1.0}
};

/**
 * @var colores
 * @brief Paleta de colores asignada 1 a 1 con los vértices.
 * * TEORÍA:
 * OpenGL usa el modelo RGB (Red, Green, Blue) con valores de 0.0 a 1.0.
 * La fila '0' de colores corresponde al vértice '0'.
 * * Si el vértice 0 es NEGRO {0,0,0} y el vértice 1 es ROJO {1,0,0},
 * la línea que los une será un degradado de negro a rojo.
 */
const float Cubo::colores[8][3] = {
	{0.0,0.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
	{0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}
};

// -------------------------------------------------------------------------
// 2. CONSTRUCTOR Y DESTRUCTOR
// -------------------------------------------------------------------------
Cubo::Cubo() {
	// Estado inicial: quieto en todos los ángulos
	this->theta[0] = 0.0f;
	this->theta[1] = 0.0f; /////////////////////////////////////////////////////////////////////this->
	this->theta[2] = 0.0f;

	// Por defecto gira en el eje Z (Como en el ejemplo del profesor)
	ejeSeleccionado = 2;
}

// -------------------------------------------------------------------------
// 3. MÉTODOS DE LA INTERFAZ ENTIDAD
// -------------------------------------------------------------------------

void Cubo::iniciar() {
	// Por ahora no necesitamos cargar texturas ni nada complejo.
	// Dejamos este método vacío pero cumplimos el contrato.
}

void Cubo::actualizar() {
	// Lógica de animación: Aumentar el ángulo del eje seleccionado
	theta[ejeSeleccionado] += 0.5f; // Velocidad de giro

	// Mantener los ángulos dentro de 0-360 para evitar desbordamientos
	if (theta[ejeSeleccionado] > 360.0f) {
		theta[ejeSeleccionado] -= 360.0f;
	}
}

void Cubo::dibujar() {
	// [IMPORTANTE]: Guardamos la matriz actual antes de mover nada.
	// Si no hacemos esto, ¡el cubo movería también a la cámara y al resto del mundo!
	glPushMatrix();

	// 1. Aplicar rotaciones (En orden X -> Y -> Z)
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	// 2. Dibujar la geometría
	colorcube();

	// [IMPORTANTE]: Restauramos la matriz para dejar todo limpio.
	glPopMatrix();
}

// -------------------------------------------------------------------------
// 4. GETTERS Y SETTERS (Encapsulamiento)
// -------------------------------------------------------------------------

void Cubo::setEje(int nuevoEje) {
	// Validación: Solo permitimos ejes 0, 1 o 2.
	if (nuevoEje >= 0 && nuevoEje <= 2) {
		this->ejeSeleccionado = nuevoEje;
	}
	// Si mandan un número inválido, simplemente lo ignoramos (Protección).
}

int Cubo::getEje() const {
	return this->ejeSeleccionado;
}

void Cubo::setAngulo(int eje, float valor) {
	if (eje >= 0 && eje <= 2) {
		this->theta[eje] = valor;
	}
}

float Cubo::getAngulo(int eje) const {
	if (eje >= 0 && eje <= 2) {
		return this->theta[eje];
	}
	return 0.0f;
}

// -------------------------------------------------------------------------
// 5. MÉTODOS AYUDANTES (Privados)
// -------------------------------------------------------------------------

/**
 * @brief Dibuja una cara cuadrada (Polígono) conectando 4 vértices.
 * * TEORÍA DE LA MÁQUINA DE ESTADOS:
 * OpenGL es secuencial. Para pintar un punto coloreado, primero debes
 * mojar el pincel (glColor) y luego tocar el lienzo (glVertex).
 * Repetimos esto 4 veces para los 4 puntos de la cara.
 * * CONSIDERACIÓN IMPORTANTE (Winding Order):
 * El orden en que pasas los índices (a, b, c, d) IMPORTA.
 * Deben pasarse en sentido ANTI-HORARIO (Counter-Clockwise) respecto
 * a la cámara para que OpenGL sepa que esa es la cara "frontal".

 * @param a Índice del primer vértice en el array 'vertices'.
 * @param b Índice del segundo vértice.
 * @param c Índice del tercer vértice.
 * @param d Índice del cuarto vértice.
 */
void Cubo::polygon(int a, int b, int c, int d) {
	// Dibujamos un cuadrado (GL_POLYGON) conectando 4 vértices
	glBegin(GL_POLYGON);// Iniciamos el dibujo de una figura rellena

	//nodo 1
	glColor3fv(colores[a]);  // 1. Mojar pincel con color 'a'
	glVertex3fv(vertices[a]); // 2. Poner vértice 'a'

	//nodo 2
	glColor3fv(colores[b]);
	glVertex3fv(vertices[b]);

	//nodo 3
	glColor3fv(colores[c]);
	glVertex3fv(vertices[c]);

	//nodo 4
	glColor3fv(colores[d]);
	glVertex3fv(vertices[d]);
	
	
	glEnd(); // terminamos la figura
}

/**
 * @brief Construye el cubo completo llamando a polygon() 6 veces.
 * * ESPECIFICACIÓN:
 * Un cubo tiene 6 caras. Este método "cose" las caras usando los índices
 * de los 8 vértices disponibles.
 * * MAPA DE CARAS (Según los índices usados):
 * - (0,3,2,1): Cara Trasera (Z negativo)
 * - (2,3,7,6): Cara Derecha (X positivo)
 * - (0,4,7,3): Cara Izquierda (X negativo) ... espera, aquí hay un truco visual.
 * - ... etc.
 * * CONSIDERACIÓN DE EFICIENCIA:
 * Estamos enviando 24 vértices a la GPU (4 vértices * 6 caras).
 * Aunque el cubo solo tiene 8 esquinas físicas, OpenGL inmediato necesita
 * que le repitas los vértices si pertenecen a caras diferentes.
 */
void Cubo::colorcube() {
	// Definimos las 6 caras del cubo usando los índices de los vértices
	polygon(0, 3, 2, 1); // Cara frontal
	polygon(2, 3, 7, 6); // Cara derecha
	polygon(0, 4, 7, 3); // Cara inferior
	polygon(1, 2, 6, 5); // Cara superior
	polygon(4, 5, 6, 7); // Cara trasera
	polygon(0, 1, 5, 4); // Cara izquierda
}