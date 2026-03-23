#pragma once
// Pragma once se usa para evitar que el compiladorlea mas de una vez

/*
Incluimos las dependencias necesarias, recordando que glut va al final
*/
#include "glut.h"

class MotorGrafico {

private:
	//Atributos
	//La instancia unica
	// el * significa que es un puntero (es como una referencia en java que podría ser null)

	static MotorGrafico* instancia; 

	//Dimenciones de la pantall
	int ancho;
	int alto;

	/*AQUI SEGUIRAN LOS METODOS PRIVADOS 
		LOS CUALES TIENEN MISIONES INTERNAS QUE NADIE DEL EXTERIOR PUEDA TOCAR
	*/

public:
	// constructor y destructor
	MotorGrafico(); //constructor por defecto
	~MotorGrafico(); // destructor por defecto

	/*FASE DE CONFIGURACION*/
	//recibe los argumentos del main original de c
	void inicializar(int argc , char** argv);

	/*FASE DE EJECUCION*/
	void iniciarLoop(); //llamara a glutMainLoop()

	/*LOGICA DEL MOTOR*/
	void renderizar(); //dibuja la esena
	void cambiarTamanio(int w, int h); //reacciona cuando estiran la ventana
	void actualizar(); //para las animaciones

	/*EL PUENTE WRAPPERS ESTATICOS*/
	/*
	Estas son las unicas funciones que GLUT puede ver
	su unico trabajo es llamar a los metodos de arriba
	*/

	static void wrapperDisplay();
	static void wrapperReshare(int w, int h);
	static void wrapperIdle();

	/*
	Debemos de recordar que glut es una libreria vieja escrita en C
	es por ello que static es como una especie de portero en la entrada
	EL CUAL ES PARA QUE EXCCLUSIVAMENTE GLUT SE COMUNIQUE 
	*/

};

