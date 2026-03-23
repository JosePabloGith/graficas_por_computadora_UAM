/*
Aquí viene un reto interesante para un programador de Java que pasa a C++ 
con OpenGL: El Puente entre C y C++.
El Problema Técnico (Contexto)
GLUT (la librería gráfica) fue escrita en C (un lenguaje viejo, sin clases). 
Tú estás escribiendo en C++ (con clases y objetos).

GLUT dice: "Dame una función suelta para llamarla cuando quiera dibujar" (glutDisplayFunc).

Tú tienes: "Tengo un método dibujar() pero vive DENTRO de un objeto Motor".

GLUT dice: "No sé qué es un objeto. No puedo entrar ahí".

La Solución: Usaremos un método static como puente. 
En Java, static significa "pertenece a la clase, no al objeto". En C++ es igual.
Al ser estático, GLUT puede verlo y usarlo para entrar a tu objeto.

Paso 3: El Orquestador (Motor.h)
Vamos a crear el archivo Motor.h en la carpeta de Archivos de encabezado.

Este será nuestro "Scene Manager". Controlará los 5 cuadrados y la cámara.
*/

#pragma once
#include "Cuadrado.h"
#include <vector> //usaremos vectores

class Motor {
private:
	//Aqui usamos punteros porque necesitamos polimorfismo
	//std::vector es el equivalente a ArrayList<Cuadrado> de java

	std::vector<Cuadrado*> misCuadrados;
	float anchoVentana;
	float altoVentana;
	
	//puntero estático a la instancia activa "el truco del puente"
	static Motor* instanciaActual;
public:
	Motor(float w, float h);
	~Motor(); // destructor (importante)

	void inicializarGL(int argc , char** argv);
	void iniciarJuego(); //arrancamos el loop

	//metodos logicos
	void actualizarTodo(int valor);
	void renderizarTodo();

	//El puente wrappers estaticos
	// GLUT llamará a estos, y estos llamarán a los métodos de arriba
	static void wrapperDibujo();
	static void wrapperUpdate(int valor);

};