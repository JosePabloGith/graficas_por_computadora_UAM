#include "Motor.h" // Solo necesitamos conocer al Jefe

// En C++, el main es una función global, no pertenece a ninguna clase.
int main(int argc, char** argv) {

	// 1. INSTANCIACIÓN ESTÁTICA (En el "Stack")
	// ---------------------------------------------------------
	// EN JAVA: Motor juego = new Motor(800, 600);
	// EN C++:  Motor juego(800, 600);
	//
	// Diferencia: Aquí no usamos 'new'. El objeto "juego" vive en la memoria 
	// rápida (Stack) y se destruirá solo cuando el main termine.
	// Es más rápido y seguro para este caso.
	Motor juego(800, 600);

	// 2. CONFIGURACIÓN
	// Le pasamos los argumentos del sistema operativo a GLUT
	juego.inicializarGL(argc, argv);

	// 3. INICIO DEL BUCLE INFINITO
	// Aquí el programa cede el control a GLUT.
	// A partir de esta línea, "juego" toma el control total.
	juego.iniciarJuego();

	return 0;
}