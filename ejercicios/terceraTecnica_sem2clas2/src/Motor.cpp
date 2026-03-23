#include "Motor.h"
#include "Cubo.h"  // Necesario para que el Motor entienda qué es un Cubo
#include <stdio.h> // Para printf (debug)

// =========================================================================
// 1. INICIALIZACIÓN ESTÁTICA
// =========================================================================
// Reservamos espacio en memoria para el puntero único del Singleton.
Motor* Motor::instancia = 0;

// =========================================================================
// 2. CONSTRUCTOR Y DESTRUCTOR
// =========================================================================

Motor::Motor(int w, int h) {
	this->ancho = w;
	this->alto = h;

	// Registramos ESTE objeto como la autoridad central
	instancia = this;

	printf("Motor: Sistema construido. Dimensiones %dx%d.\n", w, h);
}

Motor::~Motor() {
	// LIMPIEZA DE MEMORIA (Garbage Collection manual)
	// Recorremos la lista de actores y los borramos uno por uno.
	for (size_t i = 0; i < escena.size(); i++) {
		delete escena[i]; // Llama al destructor de la Entidad (y del Cubo)
	}
	escena.clear(); // Vaciamos el vector
	printf("Motor: Memoria liberada. Adios.\n");
}

// =========================================================================
// 3. GESTIÓN DE LA ESCENA
// =========================================================================

void Motor::agregarEntidad(Entidad* nuevaEntidad) {
	// Añadimos el nuevo actor al final de la lista
	escena.push_back(nuevaEntidad);

	// Le damos la oportunidad de configurarse (cargar texturas, etc.)
	nuevaEntidad->iniciar();
}


// =========================================================================
// 4. CONFIGURACIÓN DE OPENGL (El antiguo main)
// =========================================================================

void Motor::initGL(int argc, char** argv) {
	// 1. Inicializar la librería GLUT
	glutInit(&argc, argv);

	// 2. Configurar Buffers
	// GLUT_DOUBLE: Doble buffer (sin parpadeo)
	// GLUT_RGB: Colores reales
	// GLUT_DEPTH: Z-Buffer (para que lo de atrás no tape lo de adelante)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// 3. Crear Ventana
	glutInitWindowSize(ancho, alto);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Proyecto Final: Transmutacion POO");

	// 4. Configuraciones Globales de la Máquina de Estados
	glEnable(GL_DEPTH_TEST); // IMPORTANTE: Activar prueba de profundidad
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fondo Negro

	// 5. CONECTAR LOS CABLES (Callbacks)
	// Le decimos a GLUT que llame a nuestros Wrappers estáticos
	glutDisplayFunc(Motor::drawWrapper);
	glutReshapeFunc(Motor::reshapeWrapper);
	glutMouseFunc(Motor::mouseWrapper);

	// Usamos Timer en lugar de Idle para controlar los FPS (aprox 60 FPS)
	glutTimerFunc(0, Motor::updateWrapper, 0);

	printf("Motor: OpenGL inicializado correctamente.\n");
}

void Motor::start() {
	printf("Motor: Iniciando bucle principal (glutMainLoop)...\n");
	glutMainLoop(); // Cede el control al SO. Nunca regresa de aquí.
}

// =========================================================================
// 5. CICLOS DE LÓGICA (Heartbeat)
// =========================================================================

void Motor::loopDibujo() {
	// A. Limpiar la pizarra (Color y Profundidad)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// B. Resetear la matriz (volver al centro del universo)
	glLoadIdentity();

	// C. CÁMARA (Opcional, pero recomendada)
	// Aunque usemos glOrtho, gluLookAt ayuda a definir desde dónde miramos.
	// Miramos desde Z=5 hacia el origen (0,0,0).
	gluLookAt(0.0, 3.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

	// D. DIBUJAR TODO
	// Polimorfismo en acción: No importa si es Cubo o Robot, llamamos a dibujar().
	for (size_t i = 0; i < escena.size(); i++) {
		escena[i]->dibujar();
	}

	// E. Intercambiar los lienzos (Mostrar lo dibujado)
	glutSwapBuffers();
}

void Motor::loopLogica(int valor) {
	// Actualizar matemáticas de todos los objetos
	for (size_t i = 0; i < escena.size(); i++) {
		escena[i]->actualizar();
	}

	// Pedir a GLUT que vuelva a pintar la pantalla lo antes posible
	glutPostRedisplay();

	// Programar la siguiente actualización en 16ms (aprox 60 FPS)
	glutTimerFunc(16, Motor::updateWrapper, 0);
}

// =========================================================================
// 6. EVENTOS (Input)
// =========================================================================

void Motor::cambiarTamano(int w, int h) {
	// Evitar división por cero
	if (h == 0) h = 1;

	// Actualizar atributos
	this->ancho = w;
	this->alto = h;

	// Ajustar el puerto de vista (Viewport)
	glViewport(0, 0, w, h);

	// Configurar la Lente (Proyección)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// --- LÓGICA DEL PROFESOR (glOrtho) ---	NOTA, ESTO DEFORMA LA PERSPECTIVA
	// SI QUIERES VER LA PROFUNDIDAD ACTIVA :  Y DESACTIVA LO QUE ESTA hasta antes de 	glMatrixMode(GL_MODELVIEW);


	
/**/
// --- CAMBIO IMPORTANTE: DE ORTOGONAL A PERSPECTIVA ---

	// Calculamos la relación de aspecto (ancho / alto)
	float aspecto = (float)w / (float)h;

	// gluPerspective(AnguloFov, Aspecto, Cerca, Lejos);
	// - FOV (60.0f): Campo de visión (como un ojo humano).
	// - Aspecto: Para que no se deforme si estiras la ventana.
	// - Cerca (1.0f): Plano de corte cercano (nariz).
	// - Lejos (100.0f): Plano de corte lejano (horizonte).
	gluPerspective(60.0f, aspecto, 1.0f, 100.0f);


	// Mantiene la proporción para que el cubo no se estire.
	// Definimos un volumen de vista de -2 a 2 en X e Y.


/*
	//BLOQUE DEL PROFESOR
	if (w <= h) {
		glOrtho(-2.0, 2.0,
			-2.0 * (float)h / (float)w, 2.0 * (float)h / (float)w,
			-10.0, 10.0);
	}
	else {
		glOrtho(-2.0 * (float)w / (float)h, 2.0 * (float)w / (float)h,
			-2.0, 2.0,
			-10.0, 10.0);
	}
*/
	// Regresar al modo Modelo (para mover objetos)
	glMatrixMode(GL_MODELVIEW);
}

void Motor::procesarMouse(int boton, int estado, int x, int y) {
	// Lógica portada del profesor: Clic Izq/Der cambia el eje de giro.

	// Obtenemos el cubo principal
	Cubo* c = getCuboPrincipal();

	// Si existe el cubo y el botón fue presionado (DOWN)
	if (c != 0 && estado == GLUT_DOWN) {
		if (boton == GLUT_LEFT_BUTTON)   c->setEje(0); // Eje X
		if (boton == GLUT_MIDDLE_BUTTON) c->setEje(1); // Eje Y
		if (boton == GLUT_RIGHT_BUTTON)  c->setEje(2); // Eje Z

		printf("Mouse: Cambiando eje a %d\n", c->getEje());
	}
}

// =========================================================================
// 7. WRAPPERS ESTÁTICOS (El Puente)
// =========================================================================
// Solo sirven para redirigir la llamada a la instancia actual.

void Motor::drawWrapper() {
	if (instancia) instancia->loopDibujo();
}

void Motor::updateWrapper(int v) {
	if (instancia) instancia->loopLogica(v);
}

void Motor::reshapeWrapper(int w, int h) {
	if (instancia) instancia->cambiarTamano(w, h);
}

void Motor::mouseWrapper(int b, int s, int x, int y) {
	if (instancia) instancia->procesarMouse(b, s, x, y);
}

/**
 * @brief Busca en toda la escena el primer objeto que sea un Cubo.
 * ITERACIÓN POLIMÓRFICA:
 * Recorremos la lista de entidades. A cada una le preguntamos: "¿Eres un Cubo?".
 * Si responde que sí, devolvemos ese puntero. Si no, seguimos buscando.
 */
Cubo* Motor::getCuboPrincipal() {
	// Recorremos toda la lista de actores
	for (size_t i = 0; i < escena.size(); i++) {

		// Intentamos convertir el actor actual en un Cubo
		Cubo* c = dynamic_cast<Cubo*>(escena[i]);

		// Si el cast fue exitoso (no es null), ¡lo encontramos!
		if (c != nullptr) {
			return c;
		}
	}

	// Si terminamos el bucle y no encontramos nada, devolvemos null.
	return nullptr;
}

/*
Consideraciones
Puntos clave para tu Glosario:
glEnable(GL_DEPTH_TEST): Sin esto en initGL, las caras traseras del cubo se pintarían encima 
de las delanteras. Es obligatorio para 3D.

glutTimerFunc vs glutIdleFunc: El profesor usaba idle (que corre a 1000 FPS si puede). Nosotros 
usamos timer (línea 134) para limitarlo a ~60 FPS. Esto es más estable y hace que el cubo gire a 
velocidad constante en cualquier PC.
*/