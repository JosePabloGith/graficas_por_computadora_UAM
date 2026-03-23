/*
 * =========================================================================================
 * [EXPEDIENTE TÉCNICO: CONTROL DE VELOCIDADES INDEPENDIENTES Y GESTIÓN DE BUFFERS]
 * DIRECTOR DEL PROYECTO: Gema (1) - Asesor Gráficas Por Computadora
 * AUTOR: Jose Pablo Perez De Anda
 * =========================================================================================
 * * DESCRIPCIÓN GENERAL:
 * Este sistema implementa un entorno multi-ventana para contrastar el rendimiento entre 
 * Single y Double Buffering. Introduce una arquitectura de "fábrica" para la creación de 
 * contextos y una lógica de "Caja de Cambios" para el control cinemático independiente 
 * de los objetos renderizados.
 * * * TÉCNICAS DE GRAFICACIÓN IDENTIFICADAS (Lista de Cotejo):
 * 10. [X] Controles: Gestión de eventos de mouse para la modificación del estado interno 
 * (velocidad) y selección de contexto con 'glutGetWindow'.
 * 13. [X] Buffer Management: Comparación directa de 'glFlush' (Single) vs 'glutSwapBuffers' 
 * (Double) para la eliminación del parpadeo (flicker).
 * Animación: Uso de 'glutIdleFunc' para la actualización física global del sistema.
 * Arquitectura: Implementación de estructuras de datos (EstadoVentana) para el manejo de 
 * múltiples estados de memoria en C++.
 * * * SUSTENTO BIBLIOGRÁFICO:
 * - Doble Buffer: Según el "Red Book" (Cap. 1: "Motion = Redraw + Swap"), el Double 
 * Buffering es esencial para animaciones suaves, ya que evita que el usuario vea el 
 * proceso de borrado y redibujado de la escena.
 * - Estructuras: El uso de 'struct' y punteros para mapear IDs de ventanas a estados 
 * específicos sigue las prácticas de organización de datos de Balaguruswamy (Cap. 1).
 * - Transformaciones: La rotación sobre el eje Z ($0,0,1$) en 'dibujarCuadrado' aplica 
 * la matriz de rotación estándar para el espacio 2D (Howard Anton, Cap. 3).
 * * * ANALOGÍA PARA EL DESARROLLADOR JAVA:
 * En Java AWT/Swing, para evitar el parpadeo se suele recurrir a la clase 'BufferStrategy'. 
 * Aquí, 'glutSwapBuffers' hace exactamente lo mismo: intercambia el "Back Buffer" (donde 
 * dibujas oculto) con el "Front Buffer" (lo que ve el usuario). La función 'actualizarFisica' 
 * es el equivalente a un 'Thread' de juego o un 'Timer', pero ejecutándose en el tiempo 
 * de inactividad del CPU.
 * * * NOTA TÉCNICA (BITÁCORA):
 * Se aborda el "Bug del Turbo Fantasma". Al ser un motor basado en 'IdleFunc', la 
 * velocidad de rotación es "Frame-Dependent". Según Millington (Game Physics), para una 
 * simulación profesional se debería multiplicar la velocidad por un 'deltaTime' (tiempo 
 * transcurrido) para que el movimiento sea constante sin importar la potencia del CPU.
 * =========================================================================================
 */

#include<iostream>
#include "glut.h"

 /* ==========================================
  * VARIABLES GLOBALES Y ESTRUCTURAS
  * ========================================== */

  //IMPLEMENTAMOS LA IDEA DE LA CAJA DE CAMBIOS
const float VELOCIDADES[] = { -100.0f , -10.0f ,-1.0f , 0.0f , 1.0f , 10.0f , 100.0f };
//dentro de las posiciones condiderando que hay 7 elementos, el que esta 
// en la posicion 3 corresponde a 0.0f, que seria cuando esta quieto

const int TOTAL_VELOCIDADES = 7;
const int INDICE_PARADO = 3;

struct EstadoVentana {
	float anguloActual;
	int indiceVelocidad; //para determinar en que posicion sacara la velocidad
};


/* ==========================================
 * Estado Global
 * ========================================== */

EstadoVentana estadoSingle = { 0.0f , INDICE_PARADO };
EstadoVentana estadoDouble = { 0.0f , INDICE_PARADO };
int idVentanaSingle;
int idVentanaDouble;

/*
** #################################################
** Logica de configuracion
** #################################################
*/
/**
 * Configura los parámetros iniciales de OpenGL para el contexto actual.
 * Define color de fondo, color de pincel y modelo de sombreado.
 */
void configurarAmbiente() {
	glClearColor(0.0, 0.0, 0.0, 1.0); // Fondo Negro
	glColor3f(1.0, 1.0, 1.0);         // Pincel Blanco
	glShadeModel(GL_FLAT);            // Sombreado plano (eficiente)
}

/**
 * Recalcula la proyección cuando el usuario cambia el tamaño de la ventana.
 * Mantiene la relación de aspecto para evitar deformaciones.
 */
void ajustarProyeccion(int ancho, int alto) {
	glViewport(0, 0, ancho, alto);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Lógica para mantener Aspect Ratio
	if (ancho <= alto) {
		glOrtho(-50.0, 50.0,
			-50.0 * (GLfloat)alto / (GLfloat)ancho,
			50.0 * (GLfloat)alto / (GLfloat)ancho,
			-1.0, 1.0);
	}
	else {
		glOrtho(-50.0 * (GLfloat)ancho / (GLfloat)alto,
			50.0 * (GLfloat)ancho / (GLfloat)alto,
			-50.0, 50.0,
			-1.0, 1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/* ==========================================
 * 4. LÓGICA DE CONTROL (MOUSE & FISICA)
 * ========================================== */
void cambiarMarcha(EstadoVentana* estado, int direccion) {
	//direccion 1 : para subir marcha (izq) , -1 para bajar marcha (der)

	//calculamos el nuevo indice
	int nuevoIndice = estado->indiceVelocidad + direccion;

	//validamos los limites del array para no crashear
	if (nuevoIndice >= 0 && nuevoIndice < TOTAL_VELOCIDADES) {
		estado->indiceVelocidad = nuevoIndice;
		std::cout << "cambio de velocidad aplicado. Nueva V: "
			<< VELOCIDADES[nuevoIndice] << std::endl;
	}
	else return; ///////////////////////////////////////---------
}

void gestionarMouse(int boton, int estado, int x, int y) {

	if (estado != GLUT_DOWN) return; // esto evita que se ingrese cuando no se ha pulsado ningun clic
	int ventanaActual = glutGetWindow();
	//gluGetWindows se utiliza para obtener el id de la ventana glut actual

	EstadoVentana* estadoSeleccionado = nullptr;

	//Mapeamos ID de ventana -> Estructura de datos 
	if (ventanaActual == idVentanaSingle) estadoSeleccionado =&estadoSingle;
	else if (ventanaActual == idVentanaDouble) estadoSeleccionado =&estadoDouble;
	else return; //<- seria por si las moscas

	//aplicamos la logica de cambio usamos la funcion cambiarMarcha
	if (boton == GLUT_LEFT_BUTTON) cambiarMarcha(estadoSeleccionado, 1);
	if (boton == GLUT_RIGHT_BUTTON) cambiarMarcha(estadoSeleccionado, -1);
}

void actualizarFisica() {
	// Fisica para la ventana simple
	float velS = VELOCIDADES[estadoSingle.indiceVelocidad];
	if (velS != 0.0f) {
		//implica que se esta moviendo
		estadoSingle.anguloActual = estadoSingle.anguloActual + velS;


	}
	glutSetWindow(idVentanaSingle);
	glutPostRedisplay(); // esto evita que cambien sus velocidades por ocupacion de gpu

	//fisica para ventana double
	float velD = VELOCIDADES[estadoDouble.indiceVelocidad];
	if (velD != 0.0f) {
		//indica que la ventana double buffer se esta moviendo
		estadoDouble.anguloActual = estadoDouble.anguloActual + velD;

	}
	glutSetWindow(idVentanaDouble); // estos siempre fuera del if
	glutPostRedisplay(); // esto hace que siempre se redibuje el cuadrado
}


/* ==========================================
 * 5.- LÓGICA DE RENDERIZADO (CORE)
 * ========================================== */

 // Funcion auxiliar privada para dibujar el cuadrado
void dibujarCuadrado(float angulo) {
	//glLoadIdentity() es una funcion de openGL, el cual
	// reinicia la matriz activa actual a la matriz
	// identidad, eliminando cualquier tranformacion previa
	//"traslacion, rotacion , escalado"
	glLoadIdentity();

	/*glRotatef genera una matriz de rotacion y aplica
	al sistema de coordenadas actual, el angulo rota el
	sistema la cantidad de grados especificados en "angulo"
	- se espera que el cuadrado gire sobre su centro exacto
	*/
	glRotatef(angulo, 0.0, 0.0, 1.0);

	/*glRectf() dibuja un rectangulo solido definido
	por dos esquinas opuestas
	*/
	glRectf(-25.0, -25.0, 25.0, 25.0);

}

/**
 * Renderiza el cuadro usando SINGLE BUFFER.
 * Se dibuja directo en pantalla (causa parpadeo).
 */
void renderizarSingle() {
	// aseguramos contexto por seguridad
	if (glutGetWindow() != idVentanaSingle) glutSetWindow(idVentanaSingle);

	glClear(GL_COLOR_BUFFER_BIT);
	dibujarCuadrado(estadoSingle.anguloActual);
	glFlush(); // Fuerza el dibujado inmediato
}

/**
 * Renderiza el cuadro usando DOUBLE BUFFER.
 * Dibuja en memoria oculta y hace el "cambio de lienzo".
 */
void renderizarDouble() {
	//aseguramos contexto
	if (glutGetWindow() != idVentanaDouble) glutSetWindow(idVentanaDouble);
	glClear(GL_COLOR_BUFFER_BIT);
	dibujarCuadrado(estadoDouble.anguloActual);
	glutSwapBuffers(); // Intercambio suave de buffers
}

/* ==========================================
 * 6. Costructor generico "FABRICA"
 * ========================================== */

 /**
  * Función centralizada para crear ventanas y eliminar redundancia en Main.
  * @param titulo: Texto de la barra superior
  * @param posX: Posición X en pantalla
  * @param modoDisplay: Flags de GLUT (GLUT_SINGLE o GLUT_DOUBLE)
  * @param funcionRender: Puntero a la función que dibujará en esta ventana
  * @return int: El ID de la ventana creada
  */
int crearVentanaPersonalizada(const char* titulo, int posX, unsigned int modoDisplay, void(*funcionRender)(void)) {
	glutInitDisplayMode(modoDisplay | GLUT_RGB);
	glutInitWindowPosition(posX, 100);
	glutInitWindowSize(400, 400);

	int id = glutCreateWindow(titulo);

	//configuracion comun para cualquier ventana
	configurarAmbiente();
	glutReshapeFunc(ajustarProyeccion);
	glutMouseFunc(gestionarMouse);
	glutDisplayFunc(funcionRender); //asignamos la funcion especifica recibida

	return id;
}

/* ==========================================
 * 7. MAIN (DIRECTOR DE ORQUESTA)
 * ========================================== */

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	// 1.-creador de ventanas usando la fabrica generica
	idVentanaSingle = crearVentanaPersonalizada(
		"Single Buffer (independiente)",
		50,
		GLUT_SINGLE,
		renderizarSingle
	);

	idVentanaDouble = crearVentanaPersonalizada(
		"Double buffer (independiente)",
		500,
		GLUT_DOUBLE,
		renderizarDouble
	);

	// 2.- configuracion global
	glutIdleFunc(actualizarFisica);

	//3.- INICIO
	std::cout << "=== SISTEMA INICIADO ===" << std::endl;
	std::cout << "Ventanas independientes listas." << std::endl;

	glutMainLoop();

	return 0;
}

/*

📝 Bitácora de Ingeniería: El Bug del "Turbo Fantasma"
void actualizarFisica() {

// Fisica para la ventana simple

float velS = VELOCIDADES[estadoSingle.indiceVelocidad];

if (velS != 0.0f) {

//implica que se esta moviendo

estadoSingle.anguloActual = estadoSingle.anguloActual + velS;

glutSetWindow(idVentanaSingle);

glutPostRedisplay();
}





//fisica para ventana double

float velD = VELOCIDADES[estadoDouble.indiceVelocidad];

if (velD != 0.0f) {

//indica que la ventana double buffer se esta moviendo

estadoDouble.anguloActual = estadoDouble.anguloActual + velD;

glutSetWindow(idVentanaDouble);
glutPostRedisplay();

}



}

¿Por qué pasó esto? (La explicación matemática simple)
Imagina que tu código es un corredor 🏃.

Antes: Tenía que correr cargando dos costales (Ventana 1 y Ventana 2). Daba una vuelta a la pista en 10 segundos.

Con el Bug: Cuando parabas una ventana, le quitabas un costal. Ahora corre más ligero y da la vuelta en 5 segundos.

El problema: Tu código dice "Avanza 1 metro por cada vuelta". Como ahora da más vueltas en el mismo tiempo, ¡avanza más metros!


*/

