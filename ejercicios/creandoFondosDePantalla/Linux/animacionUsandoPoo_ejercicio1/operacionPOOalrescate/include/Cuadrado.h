/*
Aquí definiremos qué hace un cuadrado, pero ojo al detalle de la herencia:
En Java: class Cuadrado extends EntidadFisica
En C++: class Cuadrado : public EntidadFisica
*/

#pragma once
# include "EntidadFisica.h" // importamos al papá para poder heredar

class Cuadrado : public EntidadFisica {
public:
	// 1.- Constructor personalizado
	// recibe todos los datos para hacer
	Cuadrado(
		float startX,
		float startY,
		float s,
		float vx,
		float vy,
		float r,
		float g,
		float b
	);

	// 2.- constructor vacio #default#
	// necesario porque creamos un arreglo lista[5] 
	// y c++ necesita saber crear cuadrados vacios.
	Cuadrado();

	//3.- sobreescritura de metodos 
	//aqui cumplimos la promesa del padre
	void dibujar();
	void actualizar();

	//4.- metodos exclusivos de cuadrado (colisiones)
	//recibimos un PUNTERO al otro cuadrado para poder modificarlo si chocamos
	bool checarColision(Cuadrado* otro);
	void rebotar(Cuadrado* otro);

};