/*
Objetivo General: Entender cómo la tarjeta gráfica (GPU) 
decide qué píxeles pintar y cómo pintarlos para lograr realismo y velocidad.
*/
#include "MotorGrafico.h"

int main(int argc, char** argv) {
	// 1. Crear el Motor
	MotorGrafico motor;

	// 2. Configurar
	motor.inicializar(argc, argv);

	// 3. Arrancar
	motor.iniciarLoop();

	return 0;
}