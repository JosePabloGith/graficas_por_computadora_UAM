#include "MotorGrafico.h"
#include <stdio.h>

/*
Iniciamos las variables staticas
en este lado reservaremos el espacio en memoria para el puntero "instancia".
Al principio en nulo (0) porque no hemos creado el objeto todavia
*/
MotorGrafico* MotorGrafico::instancia = 0;

/*
2.- CONSTRUCTOR Y DESTRUCTOR
*/

MotorGrafico::MotorGrafico() {
	//aqui configuramos los valores por defecto
	this->ancho = 800;
	this->alto = 600;

	//PATRON SINGLETON (LA CLAVE)
	// yo "this", el objeto que se est creando ahora mismo
	// soy la instancia oficial que controlara todo

	instancia = this;

}

MotorGrafico::~MotorGrafico() {
	// dejamos el puntero limpio
	if (instancia == this) {
		instancia = 0;
	}
}

/*
3.- Fase de configuracion 
*/

void MotorGrafico::inicializar(int argc , char** argv) {
	//1-. iniciar glut
	glutInit(&argc , argv);

	/*
	2.- Configurar memoria de pantalla
	GLUT_DOUBLE : para evitar parpadeo
	GLUT_RGB : colores reales
	GLUT_DEPTH: Z-buffer (es para que no se vean las caras traseras al frente)
	*/

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	//3.- crear la ventana
	glutInitWindowSize(this->ancho, this->alto);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ejemplo cubo");

	/*
	4.- Configuramos las llamadas "conectar los cables"
	basicamente le decimos a GLUT: "cuando nececitamos dibujar, llama al portero wrepper"
	*/

	glutDisplayFunc(MotorGrafico::wrapperDisplay);
	glutReshapeFunc(MotorGrafico::wrapperReshare);
	glutIdleFunc(MotorGrafico::wrapperIdle);

	/*
	Parte 5.- Configuracion global de open gl
	MAQUINA DE ESTADOS
	*/
	glEnable(GL_DEPTH_TEST); //Enciende Z-buffer (vital para 3d)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //fondo negro
}

void MotorGrafico::iniciarLoop() {
	// cedemos el control total al SO
	glutMainLoop();
}

/*LOS WRAPPERS
	porteros estaticos
*/

void MotorGrafico::wrapperDisplay() {
	// El portero verifica si el jefe instancia existe y le pasa el trabajo.
	if(instancia){
		instancia->renderizar();
	}
}

void MotorGrafico::wrapperReshare(int w , int h) {
	if (instancia) {
		instancia->cambiarTamanio(w, h);
	}
}

void MotorGrafico::wrapperIdle() {
	if (instancia) {
		instancia->actualizar();
	}
}

/*
Logica real "aqui programaremos"
*/

void MotorGrafico::renderizar() {
	// 1.- Limpiamos pantalla y z-buffer (borrador nuevo)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//2.- Reiniciar la matriz - ponermos el cursor en el origen 0,0,0
	glLoadIdentity();

	//3.- camara 
	// mientras es fija para probar 
	// ojo : (0,0,5) -> centro (0,0,0) -> Arriba: (0,1,0)
	gluLookAt(0.0,0.0,5.0,
			 0.0, 0.0, 0.0,
			 0.0, 1.0, 0.0);

	//4.- dibujamos
    // (Aquí iría la llamada para dibujar tu cubo o escena)

    // IMPORTANTE: Como usamos GLUT_DOUBLE, necesitamos intercambiar los buffers al final
    glutSwapBuffers();
}

// ==========================================================
// SOLUCIÓN: IMPLEMENTACIÓN DE LAS FUNCIONES FALTANTES
// ==========================================================

void MotorGrafico::cambiarTamanio(int w, int h) {
    // 1. Evitar división por cero si la ventana se hace muy pequeña
    if (h == 0) h = 1;
    float proporcion = w * 1.0f / h;

    // 2. Ajustar el Viewport (el área de dibujo en la ventana)
    glViewport(0, 0, w, h);

    // 3. Cambiar a la matriz de proyección para configurar la perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 4. Configurar la perspectiva (Ángulo de visión, proporción, plano cercano, plano lejano)
    gluPerspective(45.0f, proporcion, 0.1f, 100.0f);

    // 5. Regresar a la matriz de Modelo-Vista para seguir dibujando objetos
    glMatrixMode(GL_MODELVIEW);
}

void MotorGrafico::actualizar() {
    // Aquí irá la lógica de actualización (rotaciones, físicas, etc.)
    
    // Por ahora, solo le indicamos a GLUT que repinte la pantalla en cada frame
    // para mantener activa la animación (Loop)
    glutPostRedisplay();
}
