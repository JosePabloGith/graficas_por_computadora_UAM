/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA 
 * ======================================================================================
 * ARCHIVO: main.cpp (Billiard Square 1 - Optimización de Estado)
 * ENTORNO: Linux (Ubuntu 24). Portado desde entorno de macros de Windows (_tmain).
 * * * TÉCNICAS DE GRAFICACIÓN EMPLEADAS:
 * 1. MÁQUINA DE ESTADOS (STATE MACHINE): OpenGL funciona como una máquina de estados. 
 * En este ejercicio se demuestra la optimización de trasladar glColor3f() a la 
 * función de inicialización (SetupRC) en lugar de llamarlo en cada frame dentro de 
 * RenderScene. Como no hay otro objeto que cambie el color, el estado permanece activo.
 * 2. ANIMACIÓN POR TIMERS: Bucle asíncrono usando glutTimerFunc().
 * 3. COLISIONES AABB: Detección de colisiones 2D contra los bordes de la pantalla.
 * * * MEJORAS APLICADAS (Gema 1):
 * - Eliminación de stdafx.h.
 * - Refactorización de _tmain y _TCHAR (exclusivos de MSVC) al estándar C++ (main).
 * ======================================================================================
 */

#include <GL/glut.h>
#include <iostream>

using namespace std;

// Posición inicial y tamaño del cuadrado
GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 25.0f;

// Velocidad (Paso en píxeles) en X e Y
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Limites lógicos de la ventana
GLfloat windowWidth;
GLfloat windowHeight;

// =====================================================================

/* Dibuja la escena - NOTA: Ya no declaramos el color aquí */
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Dibujamos un rectángulo relleno (Hereda el color rojo de SetupRC)
    glRectf(x1, y1, x1 + rsize, y1 - rsize);
    
    glutSwapBuffers();
}

/* Lógica de Física y Game Loop */
void TimerFunction(int value) {
    // Colisión Horizontal
    if(x1 > windowWidth - rsize || x1 < -windowWidth)
        xstep = -xstep;

    // Colisión Vertical
    if(y1 > windowHeight || y1 < -windowHeight + rsize)
        ystep = -ystep;

    // Cinemática
    x1 += xstep;
    y1 += ystep;

    // Safety check para redimensionamiento de ventana
    if(x1 > (windowWidth - rsize + xstep))
        x1 = windowWidth - rsize - 1;
    else if(x1 < -(windowWidth + xstep))
        x1 = -windowWidth - 1;
        
    if(y1 > (windowHeight + ystep))
        y1 = windowHeight - 1;
    else if(y1 < -windowHeight + rsize - ystep)
        y1 = -windowHeight + rsize - 1;

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1); // Bucle a ~30FPS
}

/* Inicialización de estado de OpenGL */
void SetupRC(void) {
    // Estado 1: Color de limpieza (Fondo Azul)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Estado 2: Color de dibujo (Pincel Rojo). 
    // Al declararlo aquí, optimizamos el RenderScene.
    glColor3f(1.0f, 0.0f, 0.0f);
}

/* Manejo de Proporción (Aspect Ratio) al redimensionar la ventana */
void ChangeSize(GLsizei w, GLsizei h) {
    GLfloat aspectRatio;
    if(h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    aspectRatio = (GLfloat)w / (GLfloat)h;
    if (w <= h) {
        windowWidth = 100.0f;
        windowHeight = 100.0f / aspectRatio;
        glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
    } else {
        windowWidth = 100.0f * aspectRatio;
        windowHeight = 100.0f;
        glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* Punto de entrada estándar para C++ */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Usamos el argumento 0 (nombre del ejecutable) como título
    glutCreateWindow("Bounce 1 - Optimizacion de Estado");
    
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);
    
    SetupRC();
    
    glutMainLoop();
    return 0;
}
