/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA - GEMA 1
 * ======================================================================================
 * ARCHIVO: main.cpp (Alteración Dinámica de Menús + Fix Matrices)
 * ENTORNO: Adaptado para Linux (Ubuntu 24 / Wayland / Hyprland).
 * * * TÉCNICAS REPARADAS Y AÑADIDAS:
 * 1. SEPARACIÓN DE MATRICES: Uso estricto de GL_PROJECTION para la cámara y 
 * GL_MODELVIEW para ubicar objetos, evitando deformaciones del "Frustum" (recorte).
 * 2. FALLBACK DE TECLADO: glutKeyboardFunc añadido como alternativa infalible a los 
 * menús pop-up de GLUT que suelen fallar en gestores de ventanas modernos (Wayland).
 * 3. SHADING BÁSICO (Objetivo 10): Se activó GL_COLOR_MATERIAL para permitir que el
 * cubo responda a una iluminación simulada (color de vértices).
 * ======================================================================================
 */

#include <iostream>
#include <GL/glut.h>

using namespace std;

// =====================================================================
// VARIABLES GLOBALES (Estado de la Aplicación)
// =====================================================================
int menu2D, menu3D;
GLsizei ysize;
#define SIZE 300

// --- MÁQUINA DE ESTADOS ---
bool modo3D = false;       
bool mostrarCubo = false;  
bool luzEncendida = true;  

// Prototipos
void opciones3D(GLint opcion_seleccionada);
void opciones2D(GLint opcion_seleccionada);

// =====================================================================
// MANEJO DE EVENTOS (Menús y Teclado)
// =====================================================================

void opciones2D(GLint opcion_seleccionada) {
    switch (opcion_seleccionada) {
        case 1: 
            mostrarCubo = !mostrarCubo;
            cout << (mostrarCubo ? "[2D] Cubo AGREGADO." : "[2D] Cubo REMOVIDO.") << endl;
            break;	
        case 2: 
            cout << "[Info] Modo 2D activo. Click central (rueda) o tecla 'M' para 3D." << endl;		
            break;		
    }
    glutPostRedisplay();
}

void opciones3D(GLint opcion_seleccionada) {
    switch (opcion_seleccionada) {
        case 1: 
            luzEncendida = !luzEncendida;
            cout << (luzEncendida ? "[3D] Luz ENCENDIDA." : "[3D] Luz APAGADA.") << endl;
            break;
        case 2: 
            cout << "[Info] Modo 3D activo. Click central (rueda) o tecla 'M' para 2D." << endl;		
            break;
    }
    glutPostRedisplay();
}

/* --- NUEVO: ALTERNATIVA PARA HYPRLAND/WAYLAND --- */
void teclado(unsigned char key, int x, int y) {
    switch(key) {
        case 'c': // Tecla C: Agregar/Quitar Cubo
        case 'C':
            mostrarCubo = !mostrarCubo;
            cout << (mostrarCubo ? "[Teclado] Cubo ON" : "[Teclado] Cubo OFF") << endl;
            break;
        case 'l': // Tecla L: Luz On/Off
        case 'L':
            if(modo3D) {
                luzEncendida = !luzEncendida;
                cout << (luzEncendida ? "[Teclado] Luz ON" : "[Teclado] Luz OFF") << endl;
            } else {
                cout << "[Teclado] La luz solo funciona en modo 3D" << endl;
            }
            break;
        case 'm': // Tecla M: Cambiar modo
        case 'M':
            modo3D = !modo3D;
            cout << (modo3D ? "[Teclado] Cambio a 3D" : "[Teclado] Cambio a 2D") << endl;
            break;
        case 27: // ESC para salir
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int status, int x, int y) {
    if (button == GLUT_MIDDLE_BUTTON && status == GLUT_DOWN) {
        glutDetachMenu(GLUT_RIGHT_BUTTON);	
        if (glutGetMenu() == menu2D) {		
            glutSetMenu(menu3D);
            modo3D = true;       
            cout << "---> CAMBIO A MODO 3D <---" << endl;
        } else {		
            glutSetMenu(menu2D);
            modo3D = false;      
            cout << "---> CAMBIO A MODO 2D <---" << endl;
        }
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        glutPostRedisplay(); // Forzar dibujado tras el cambio
    }
}

// =====================================================================
// FUNCIONES GRÁFICAS (Matrices y Renderizado)
// =====================================================================

void setupRC() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // Fondo Blanco
    glEnable(GL_DEPTH_TEST);          // Activar Z-Buffer
    
    // Configuración para que glColor3f simule luces de forma básica
    glEnable(GL_COLOR_MATERIAL);      
}

void resize(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;

    // 1. CONFIGURAMOS LA LENTE DE LA CÁMARA (PROJECTION)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, 0.0, h, -500.0, 500.0); // Ampliamos la profundidad Z

    // 2. AJUSTAMOS EL ÁREA DE DIBUJO
    glViewport(0, 0, w, h);

    // 3. ¡EL PARCHE VITAL! REGRESAMOS AL MUNDO DE LOS OBJETOS
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    
    // Asegurarnos de estar trabajando en la matriz de objetos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (mostrarCubo) {
        glPushMatrix(); 
        
        // Movemos el objeto al centro aproximado de la pantalla
        glTranslatef(glutGet(GLUT_WINDOW_WIDTH) / 2.0f, glutGet(GLUT_WINDOW_HEIGHT) / 2.0f, 0.0f);
        
        if (modo3D) {
            glRotatef(45.0f, 1.0f, 1.0f, 0.0f); // Rotación en X e Y
            glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
            
            if (luzEncendida) {
                glColor3f(0.2f, 0.6f, 1.0f); // Azul brillante
            } else {
                glColor3f(0.1f, 0.1f, 0.2f); // Azul oscuro (Apagado)
            }
            glutSolidCube(100.0);
            
            // Contorno negro
            glColor3f(0.0f, 0.0f, 0.0f); 
            glutWireCube(102.0); 
        } else {
            glColor3f(0.0f, 0.0f, 0.0f); // Solo alambre negro
            glutWireCube(100.0); 
        }
        
        glPopMatrix(); 
    }
    
    glutSwapBuffers(); // Para GLUT_DOUBLE (Evita pantalla blanca)
}

// =====================================================================
// PUNTO DE ENTRADA
// =====================================================================
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Vista de Supervisor (Fix Hyprland)");
    
    setupRC();	
    
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse);
    glutKeyboardFunc(teclado); // <--- NUESTRO FALLBACK SEGURO
    
    menu3D = glutCreateMenu(opciones3D);
    glutAddMenuEntry("Apagar/Encender Luz", 1);
    glutAddMenuEntry("Modo 3D", 2);

    menu2D = glutCreateMenu(opciones2D);
    glutAddMenuEntry("Agregar/Quitar Cubo", 1);		
    glutAddMenuEntry("Modo 2D", 2);
        
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    cout << "=== SISTEMA INICIADO ===" << endl;
    cout << "CONTROLES DE TECLADO (Seguro para Hyprland):" << endl;
    cout << " [C] : Agregar / Quitar Cubo" << endl;
    cout << " [M] : Intercambiar Modo (2D <-> 3D)" << endl;
    cout << " [L] : Encender / Apagar Luz (Solo 3D)" << endl;
    cout << " [Click Central] : Tambien cambia de Modo" << endl;

    glutMainLoop(); 

    return 0;
}
