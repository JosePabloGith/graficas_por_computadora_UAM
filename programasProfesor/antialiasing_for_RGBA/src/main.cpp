/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA - GEMA 1
 * ======================================================================================
 * ARCHIVO: main.cpp (Antialiasing en modo RGBA)
 * ENTORNO: Linux (Ubuntu 24 / Wayland / Hyprland). Código portado de Windows.
 * * * TÉCNICAS DE GRAFICACIÓN EMPLEADAS (Ref. Red Book Cap. 6 - Blending/Antialiasing):
 * 1. BLENDING (MEZCLA): Uso de glEnable(GL_BLEND) y glBlendFunc(GL_SRC_ALPHA, 
 * GL_ONE_MINUS_SRC_ALPHA) para combinar el color de los píxeles de la línea 
 * con el color del fondo, creando un gradiente en los bordes.
 * 2. SMOOTHING (SUAVIZADO): Uso de glEnable(GL_LINE_SMOOTH) e interpelación a 
 * OpenGL sobre la calidad del renderizado con glHint(GL_LINE_SMOOTH_HINT).
 * 3. CONSULTA DE HARDWARE: Uso de glGetFloatv() para preguntarle a la tarjeta 
 * gráfica cuáles son sus límites de grosor de línea (GL_LINE_WIDTH_RANGE).
 * * * RELACIÓN CON OBJETIVOS DEL PROYECTO:
 * - [9] Reflexión/Blending: Se utilizan las bases matemáticas del Blending (Canal Alfa).
 * - [10] Controles: Uso del teclado para manipular la rotación de la matriz.
 * * * MEJORAS APLICADAS (Gema 1):
 * - Eliminación de <stdafx.h> (Incompatible con Linux/GCC).
 * - Implementación de GLUT_DOUBLE y glutSwapBuffers() para soportar Hyprland/Wayland.
 * ======================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> // Sustituye la dependencia de Windows

static float rotAngle = 0.0f;

/* Inicialización del entorno: Antialiasing, Blending y Grosor de líneas */
void init(void) {
    GLfloat values[2];
    
    // Consultamos las capacidades físicas de la Tarjeta Gráfica
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    printf("Granularidad de grosor de linea soportada (Salto minimo): %3.1f\n", values[0]);
    
    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    printf("Rango de grosor de linea soportado (Min - Max): %3.1f - %3.1f\n", values[0], values[1]);
    
    // --- TÉCNICA: ANTIALIASING Y BLENDING ---
    glEnable(GL_LINE_SMOOTH); // Avisa que queremos suavizar las líneas
    glEnable(GL_BLEND);       // Activa la mezcla de colores (Canal Alfa)
    
    // Fórmula de mezcla: Color = (ColorLinea * Alfa) + (ColorFondo * (1 - Alfa))
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Le pedimos a OpenGL que no se preocupe por el rendimiento, solo que suavice
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE); 
    
    glLineWidth(1.5f); // Grosor de las líneas
    glClearColor(0.0, 0.0, 0.0, 0.0); // Fondo Negro
}

/* Dibuja 2 líneas diagonales que forman una "X" */
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Línea 1 (Verde)
    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
    glRotatef(-rotAngle, 0.0, 0.0, 1.0); // Rotación sobre el eje Z (0,0,1)
    glBegin(GL_LINES);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, -0.5);
    glEnd();
    glPopMatrix();
    
    // Línea 2 (Azul)
    glColor3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glRotatef(rotAngle, 0.0, 0.0, 1.0); // Rotación inversa sobre eje Z
    glBegin(GL_LINES);
        glVertex2f(0.5, 0.5);
        glVertex2f(-0.5, -0.5);
    glEnd();
    glPopMatrix();
    
    glutSwapBuffers(); // Parche Wayland (Reemplaza a glFlush)
}

/* Ajusta la cámara cuando la ventana cambia de tamaño */
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Mantiene las proporciones de la ventana para que la X no se aplaste
    if (w <= h)
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    else
        gluOrtho2D(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0);
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* Interacción con el usuario */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'r':
        case 'R':
            rotAngle += 20.0f; // Rota 20 grados la X
            if (rotAngle >= 360.0f) rotAngle = 0.0f;
            glutPostRedisplay();
            break;
        case 27: /* Tecla ESC */
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    // PARCHE HYPRLAND: Cambiamos GLUT_SINGLE por GLUT_DOUBLE
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(300, 300);
    glutCreateWindow("Antialiasing - Presiona 'R' para rotar");
    
    init(); // Inicializar el suavizado
    
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    
    glutMainLoop();
    return 0;
}
