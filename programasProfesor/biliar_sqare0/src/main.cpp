/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA 
 * ======================================================================================
 * ARCHIVO: main.cpp (Billiard Square - Rebote 2D)
 * ENTORNO: Linux (Ubuntu 24) / Compatible con Windows.
 * * * TÉCNICAS DE GRAFICACIÓN EMPLEADAS:
 * 1. ANIMACIÓN POR TIMERS: Uso de glutTimerFunc() para crear un bucle de actualización
 * asíncrono (Game Loop). Desacopla la lógica de movimiento de la tasa de refresco.
 * 2. DETECCIÓN DE COLISIONES BÁSICA: Algoritmo "AABB vs Window Bounds". 
 * Verifica los límites de la figura contra los límites del volumen de recorte.
 * 3. PROYECCIÓN CON RELACIÓN DE ASPECTO: Implementación en ChangeSize para 
 * evitar que el cuadrado se deforme al redimensionar la ventana.
 * * * RELACIÓN CON OBJETIVOS DEL PROYECTO:
 * - [2] Movimiento: Establece la base matemática de actualización de coordenadas.
 * ======================================================================================
 */

#include <GL/glut.h>
#include <iostream>

using namespace std;

// Posición inicial y tamaño
GLfloat x1 = 0.0f;
GLfloat y1 = 0.0f;
GLfloat rsize = 25.0f;

// Velocidad (Paso) en X e Y
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

// Limites lógicos de la ventana
GLfloat windowWidth;
GLfloat windowHeight;

// =====================================================================

/* Dibuja la escena */
void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo
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

    // Movimiento
    x1 += xstep;
    y1 += ystep;

    // Prevención de salida de límites (Safety check)
    if(x1 > (windowWidth - rsize + xstep))
        x1 = windowWidth - rsize - 1;
    else if(x1 < -(windowWidth + xstep))
        x1 = -windowWidth - 1;
        
    if(y1 > (windowHeight + ystep))
        y1 = windowHeight - 1;
    else if(y1 < -windowHeight + rsize - ystep)
        y1 = -windowHeight + rsize - 1;

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

/* Inicialización de estado */
void SetupRC(void) {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Azul
}

/* Mantiene la proporción al redimensionar la ventana */
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

/* Punto de entrada adaptado para Linux/C++ */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Bounce - Billiard Square");
    
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(33, TimerFunction, 1);
    
    SetupRC();
    glutMainLoop();
    return 0;
}
