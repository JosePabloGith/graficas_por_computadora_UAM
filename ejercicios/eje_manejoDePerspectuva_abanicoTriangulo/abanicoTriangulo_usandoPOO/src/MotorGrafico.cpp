#include <iostream>
#include "MotorGrafico.h"
#include "glut.h" // siempre al final

// Inicializamos el puntero estático en nulo
MotorGrafico* MotorGrafico::instanciaActual = nullptr;

// ==========================================
// 1. CONSTRUCTOR (Nace el objeto)
// ==========================================
MotorGrafico::MotorGrafico() {
	// Guardamos "quién soy" para que los wrappers me encuentren
	instanciaActual = this;

	// Inicializamos las variables que antes eran globales
	// Estos valores son los mismos que tenía el profesor al inicio
	bCull = false;      // GL_FALSE
	bDepth = false;     // GL_FALSE
	bOutline = true;    // GL_TRUE
	shademode = GL_FLAT;

	miCono = new Cono();// < -- - AGREGAR ESTO : Creamos el cono en memoria
	miCamara = new Camara(); // <--- AGREGAR ESTO: Nace el camarógrafo

	anchoVentana = 200; // Valores por defecto
	altoVentana = 200;
}

// ==========================================
// 2. DESTRUCTOR (Muere el objeto)
// ==========================================
MotorGrafico::~MotorGrafico() {
	// No hay punteros dinámicos que limpiar por ahora
}

// ==========================================
// 3. INICIALIZACIÓN (Configuración Técnica)
// ==========================================
void MotorGrafico::inicializar(int argc, char** argv) {

	// A. Configuración de GLUT (Lo que estaba en el main)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ¡Agregamos GLUT_DEPTH para 3D!
	glutInitWindowSize(anchoVentana, altoVentana);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Laboratorio 3D - Version POO");

	// B. Configuración de OpenGL (Lo que estaba en SetupRC)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fondo Negro
	glColor3f(0.0f, 1.0f, 0.0f);          // Color base verde

	// AGREGAR ESTO AQUÍ:
	glFrontFace(GL_CW); // <--- ¡EL TRUCO! (Sentido del Reloj)
	/* Esto le dice a OpenGL: "Si los puntos del triángulo giran como las
	   manecillas del reloj, esa es la cara de FRENTE".
	   Sin esto, la tecla F1 (Culling) ocultaría la cara equivocada.
	*/

	// Conectamos los Wrappers
	glutDisplayFunc(wrapperDisplay);
	glutReshapeFunc(wrapperReshape);
	glutSpecialFunc(wrapperSpecial);
}

void MotorGrafico::iniciarLoop() {
	glutMainLoop();
}

// ==========================================
// 4. LÓGICA GRÁFICA (El Corazón)
// ==========================================

void MotorGrafico::cambiarTamano(int w, int h) {
	// Actualizamos las variables internas
	anchoVentana = w;
	altoVentana = h;

	// (Código rescatado de ChangeSize del profesor)
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Usamos perspectiva en lugar de ortogonal (porque es 3D)
	gluPerspective(60.0, 1.0, 1.5, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

void MotorGrafico::renderizar() {
	// (Código rescatado de RenderScene, pero simplificado por ahora)

	// 1. Limpiamos Buffer de Color y de Profundidad (Z-Buffer)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// --- APLICAR CONFIGURACIONES GLOBALES (NUEVO) ---

	// Configuración de Culling (F1)
	if (bCull) glEnable(GL_CULL_FACE);
	else       glDisable(GL_CULL_FACE);

	// Configuración de Profundidad (F2)
	if (bDepth) glEnable(GL_DEPTH_TEST);
	else        glDisable(GL_DEPTH_TEST);

	// Configuración de Relleno vs Alambre (F3)
	if (bOutline) glPolygonMode(GL_BACK, GL_LINE); // Solo el reverso como linea
	else          glPolygonMode(GL_BACK, GL_FILL);

	// Configuración de Sombreado (F4)
	glShadeModel(shademode);

	// ---------------------------------------------


	glLoadIdentity();

	// AQUI DIBUJAREMOS EL CONO MÁS ADELANTE...
	/*
	Colocamos la camara (usando gluLookAt) fijo por ahora para 
	probar.
	Ponemos el ojo un poco lejos para poder verl el cono
	los primeros 3 parametros de glutLookAt
	son las coordenadas dentro del mundo
	en nuestro ejemplo z=-200, eso quiere decir que el cono 
	"esta metido apuntando hacia adentro de la pantalla"

	Los siguientes 3 indican
	hacia adonde esas mirando, es el punto exacto donde
	pones la mirada, con 0,0,0 queda 
	perfectamente centrada dentro del objeto
		-Si pones 50,0,0 la camara girara el cuello a la derecha
		El cono se vería a la izquierda de la pantalla

	Los ultimos 3: 
		indican a donde se encuentra el cielo
		nosotros pusimos 0,1,0 lo cual significa
			-el eje y apunta al cielo
				-si pusieras (1,0,0) estarias acostado de lado
				- si pones (0,-1,0) estarias de cabeza "cono invertido"

	*/
	// gluLookAt(0,0,-200, 0,0,0, 0,1,0); -> este es el tripie fijo

	// AHORA:
	if (miCamara) {
		miCamara->activar(); // <--- La cámara se acomoda sola
	}
	//Dibujamos el cono (la llamada mágica)
	if (miCono) {
		miCono->dibujar(); //agregamos esto :D
	}

	glutSwapBuffers();
}

/*
Para entender esta parte, debes de imaginar que el objeto
esta atravezado por 2 brochetas no visibles
x o diffX
	-Es cúando quieres incrementar el giro sobre el eje x
	-el eje X es horizontal
		-si giras sobre el eje x, el cono se inclina hacia adelante 
		 o hacia atras "como si dijeras sí con la cabeza"
	- En el codigo: al puchar arriba, enviamos -5.0f
		-indicamos "inclinate 5 grados hacia atrás"
y o diffY
Aqui estamos pensando en un giro sobre el eje y, 
colmo en el mamitas club, al pagarle a la chica del pool dance
	-si giras sobre el eje Y, el cono gira sobre su base
		-- como si dijeras no con la cabeza
	* Dentro del codigo: enviamos 0.0f cuando presionamos arriba, porque 
	no queremos que gire como trompo, solo queremos que se incline

	si observas en el codigo usamos += , de modo que al dejar puchado la tecla se vaya 
	acomulando, de este modo vamos haciendo una animacion mas fluida
*/
void MotorGrafico::procesarTeclas(int key, int x, int y) {
	// (Aquí meteremos la lógica de la cámara más adelante)
	/*
	Lo que sigue es para probar la rotacion
	nececitas entender esto: 
	si quieres mirar hacia adelante "camara +Z", la computadora lo que hace es en realidad
	es "empujar todo el mundo hacia atras"  (mundo -Z)
	- si quieres girar la cabeza a la derecha, "la computadora gira todo el mundo a la derecha"
		-Cuando haces miCono->rotar(...), estamos afectando SOLO AL OBJETO.
			-Es como si el cono estuviera en una plataforma giratoria
			-Tú estas quieto mirandolo
			- El cono gira. La pared de atrás NO gira

		-- Cuando usamos gluLookAt(...) "o movemos las variables ex, ey, ez" estás adectando 
		a TODO EL UNIVERSO
			-* Como si tuvieras un drone y volaras alrededor del cono
			-* si te mueves a la izquierda, el cono y la pared de atrás se mueven a la derecha
				de tu pantalla

		** Resumen: 
		-- gluLookAt (La vista): Mueve todo el esenario en dirección contraria para 
		   simular que TÚ te moviste.
		-- glRotate | glTranslate (El modelo): mueve solo la pieza actual dentro del esenario

		--* Por esa razon gluLookAt se pone al principio de renderizar();
			- Primero acomodamos el mundo ("movemos la cámara").
			- Luego dibujamos a los objetos en sus posiciones relativas
	*/


	if (key == 27) exit(0);// 27 es la tecla Escape (ASCII)
/*
	ESTO DE AQUI ES PARA ENTENDER LAS ROTACIONES DEL OBJETO
	DENTRO DEL EJEMPLO DEL PROFESOR SE MOVIA LA CAMARA 

	// IMAGINA QUE PRIMERO COLOCAMOS LA CAPARA,
	//POSTERIORMENTE PONEMOS A BAILAR EL CONO LOCO

	// Si puchamos flechas el cono es afectado 
	if (key == GLUT_KEY_UP) miCono->rotar(-5.0f,0.0f); 
	//con (-5.0f,0.0f); el cono gira como pollo al azador "eje x de barra"
	// pero el pollo gira hacia tí 
	if (key == GLUT_KEY_DOWN)  miCono->rotar(5.0f, 0.0f);
	// el pollo gira girando hacia adentro de la pantalla
	if (key == GLUT_KEY_LEFT)  miCono->rotar(0.0f, -5.0f);
	// aqui imagina que el cono se mueve como una puta en el tubo
	//desde tu vista, gira hacia adentro de la pantalla
	if (key == GLUT_KEY_RIGHT) miCono->rotar(0.0f, 5.0f);
	// para este, imagina que la puta gira, pero ahora en direccion
	//hacia afuera de la pantalla
*/
	// CONTROLES DE CÁMARA (Lógica del Profesor portada a POO)
	// ahora la vista sera como una especie de dron, moveremos la camara
	//la figura quedara pegada al suelo

	// Flechas Arriba/Abajo: ZOOM (Alejarse/Acercarse)
	if (key == GLUT_KEY_UP)    miCamara->zoom(0.02f);
	if (key == GLUT_KEY_DOWN)  miCamara->zoom(-0.02f);

	// Flechas Izquierda/Derecha: ORBITAR (Girar alrededor)
	if (key == GLUT_KEY_LEFT)  miCamara->orbitar(0.02f);
	if (key == GLUT_KEY_RIGHT) miCamara->orbitar(-0.02f);


	// aqui agregamos la funcionalidad de las teclas especiales 
	// F1: Culling (Ocultar caras traseras)
	if (key == GLUT_KEY_F1) bCull = !bCull; // Interruptor ON/OFF

	// F2: Depth Test (Z-Buffer - Profundidad)
	if (key == GLUT_KEY_F2) bDepth = !bDepth;

	// F3: Outline (Modo Alambre)
	if (key == GLUT_KEY_F3) bOutline = !bOutline;

	// F4: Shading (Suave vs Plano)
	if (key == GLUT_KEY_F4) {
		if (shademode == GL_FLAT) shademode = GL_SMOOTH;
		else shademode = GL_FLAT;
	}

	// Redibujar
	glutPostRedisplay(); // ¡Importante! Avisar que algo cambió para repintar
}

// ==========================================
// 5. LOS PUENTES (Wrappers Estáticos)
// ==========================================

void MotorGrafico::wrapperDisplay() {
	if (instanciaActual) instanciaActual->renderizar();
}

void MotorGrafico::wrapperReshape(int w, int h) {
	if (instanciaActual) instanciaActual->cambiarTamano(w, h);
}

void MotorGrafico::wrapperSpecial(int key, int x, int y) {
	if (instanciaActual) instanciaActual->procesarTeclas(key, x, y);
}