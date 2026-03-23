#pragma once

// Importamos GLUT para tener acceso a los tipos de datos gráficos (GLboolean, GLenum, etc.)
// Nota: Asegúrate de que el nombre coincida con tu archivo (glut.h o GL/glut.h)
#include "Cono.h"
#include "Camara.h"
#include "glut.h" //esto al ultimo de los include

class MotorGrafico {

private:
	// ==========================================
	// 1. ESTADO INTERNO (Lo que antes eran variables globales)
	// ==========================================
	// Al hacerlas privadas, protegemos la configuración. 
	// Ahora pertenecen al objeto Motor, no al "aire".

	GLboolean bCull;      // Controla el "Culling" (Ocultar caras traseras) - Antes cambiaba con F1
	GLboolean bDepth;     // Controla el "Z-Buffer" (Profundidad) - Antes cambiaba con F2
	GLboolean bOutline;   // Controla el modo "Alambre" (Wireframe) - Antes cambiaba con F3
	GLenum    shademode;  // Controla el sombreado (Plano vs Suave) - Antes cambiaba con F4
	Cono* miCono; //Representa al objeto que vamos a dibujar
	Camara* miCamara; //<--- 2. NUEVO: El puntero al objeto cámara

	// Guardamos el tamaño de la ventana por si necesitamos recalcular la cámara
	int anchoVentana;
	int altoVentana;

	// ==========================================
	// 2. EL PUENTE (Instancia Estática)
	// ==========================================
	// GLUT (lenguaje C) no puede entrar directamente a un objeto (lenguaje C++).
	// Usaremos este puntero estático para que las funciones puente sepan 
	// a qué Motor deben avisarle cuando pase algo.
	static MotorGrafico* instanciaActual;

public:
	// ==========================================
	// 3. CONSTRUCTOR Y DESTRUCTOR
	// ==========================================
	MotorGrafico();  // Aquí inicializaremos las variables a sus valores por defecto
	~MotorGrafico(); // Aquí limpiaremos memoria al cerrar

	// ==========================================
	// 4. CONFIGURACIÓN E INICIO
	// ==========================================
	// Configura GLUT, la ventana y las luces. Reemplaza al antiguo main() y SetupRC()
	void inicializar(int argc, char** argv);

	// Arranca el bucle infinito. Una vez llames a esto, el programa no para.
	void iniciarLoop();

	// ==========================================
	// 5. MÉTODOS LÓGICOS (Las Acciones)
	// ==========================================
	// Aquí vivirá el código que rescatamos del profesor.

	void renderizar();                // Reemplaza a la función global 'RenderScene'
	void cambiarTamano(int w, int h); // Reemplaza a la función global 'ChangeSize'
	void procesarTeclas(int key, int x, int y); // Reemplaza a 'SpecialKeys'

	// ==========================================
	// 6. WRAPPERS ESTÁTICOS (El Intermediario)
	// ==========================================
	// GLUT solo acepta funciones globales o estáticas.
	// Estas funciones recibirán la llamada de GLUT y se la pasarán al método correspondiente de arriba.

	static void wrapperDisplay();
	static void wrapperReshape(int w, int h);
	static void wrapperSpecial(int key, int x, int y);
};