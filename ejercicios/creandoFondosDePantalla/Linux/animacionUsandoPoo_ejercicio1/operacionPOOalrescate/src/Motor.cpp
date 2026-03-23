/*
Clase Gestora: Motor (El Engine)
Rol: Es el "Jefe de Pista". No se dibuja a sí mismo, pero controla a los demás.

Atributos:

listaDeObjetos: Un arreglo (o vector) que contiene a nuestros 5 cuadrados.

Métodos:
inicializar(): Configura glutInit, glutCreateWindow, etc.
renderizarEscena(): Un bucle for que le dice a cada cuadrado: "¡Dibújate!".
calcularLogica(): Un bucle for que le dice a cada cuadrado: "¡Muévete!" y revisa choques.
iniciarJuego(): Llama a glutMainLoop().
aqui implementamos motor.cpp
*/

#include "Motor.h"
#include <iostream>
#include"glut.h" // siempre al final

// iniciamos el puntero estatico en nulo
Motor* Motor::instanciaActual = nullptr;

/*
constructor y destructor

*/
Motor::Motor(float w, float h) {
	anchoVentana = w;
	altoVentana = h;
	instanciaActual = this; //guardamos "quien soy" para que el puente sepa a quien llamar

	//creamos los objetos
	//new Cuadrado(x,y,tam,vx,vy,r,g,b)
	misCuadrados.push_back(new Cuadrado(10, 10, 50, 4.2f, 3.1f, 1, 0, 0)); //rojo
	misCuadrados.push_back(new Cuadrado(200,200,60,-3.5f,2.2f,0,1,0)); //verde
	misCuadrados.push_back(new Cuadrado(400,100,40,5.1f,-4.0f,0,0,1)); //azul
	misCuadrados.push_back(new Cuadrado(500,400,80,-2.0f,-6.0f,1,1,0)); //amarillo
	misCuadrados.push_back(new Cuadrado(300,300,30,6.0f,1.0f,1,0,1)); // Magenta

}

Motor::~Motor() {
	//Limpieza de memoria Garbage collection manual
	std::cout << "Limpiando memoria..." << std::endl;
	for (size_t i = 0; i < misCuadrados.size(); i++) {
		delete misCuadrados[i]; //matamos cada cuadrado
	}
	misCuadrados.clear(); //vaciamos la lista
}

void Motor::inicializarGL(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize((int)anchoVentana, (int)altoVentana);
	glutCreateWindow("Sistema de coliciones con POO");
	glClearColor(0.0,0.0,0.0,1.0);

	// configuración de cámara 2D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0 , anchoVentana , 0.0, altoVentana);
	glMatrixMode(GL_MODELVIEW);

	//Registro de callbacks usando el puente
	glutDisplayFunc(wrapperDibujo);
	glutTimerFunc(0,wrapperUpdate,0);
}

void Motor::iniciarJuego() {
	glutMainLoop();
}

/*
Logica del juego
*/
void Motor::actualizarTodo(int valor) {
	//1.- mover todos
	for (size_t i = 0; i < misCuadrados.size();i++) {
		misCuadrados[i]->actualizar();
	}

	//2.- Checar colisiones (todos contra todos - AABB)
	for (size_t i = 0; i < misCuadrados.size();i++) {
		
		for (size_t j = i + 1; j < misCuadrados.size();j++) {
			//si el cuadrado i choca con el cuadrado j
			if (misCuadrados[i]->checarColision(misCuadrados[j])) {
				misCuadrados[i]->rebotar(misCuadrados[j]);
			}

		}
	
	}

	//3.- volver a llamar al timer
	glutPostRedisplay();
	glutTimerFunc(16,wrapperUpdate,0);
}

void Motor::renderizarTodo() {
	glClear(GL_COLOR_BUFFER_BIT);
	//polimorfismo : cada quien se dibuja

	for (size_t i = 0; i < misCuadrados.size(); i++) {
		misCuadrados[i]->dibujar();
	}

	glutSwapBuffers();
}

//El puente wrappers estaticos
void Motor::wrapperDibujo() {
	//el metodo estatico llama a la instancia viva
	if (instanciaActual != nullptr) {
		instanciaActual->renderizarTodo();
	}
}

void Motor::wrapperUpdate(int valor) {
	if (instanciaActual != nullptr) {
		instanciaActual->actualizarTodo(valor);
	}
}

/*
Análisis para tu aprendizaje:
std::vector vs Arreglos:

Usé std::vector en lugar de Cuadrado lista[5]. 
Es la forma moderna de hacer listas dinámicas (como ArrayList en Java).

push_back es como .add().

.size() es como .size().

El Puente (wrapperDibujo):

GLUT llama a wrapperDibujo() (que no necesita objeto).

wrapperDibujo() dice: 
"¿Dónde está el objeto Motor (instanciaActual)? 
¡Ah, aquí estás! Ejecuta renderizarTodo()".

Así logramos que C puro hable con C++ Orientado a Objetos.

El Destructor (~Motor):

En el bucle for, usamos delete misCuadrados[i]. 
Esto libera la RAM que ocupó cada new Cuadrado(...).

En Java esto no lo hacías, pero aquí es vital para ser un buen ingeniero.
*/