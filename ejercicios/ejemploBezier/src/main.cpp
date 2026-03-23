/**
 * ============================================================================
 * PROYECTO: LABORATORIO BÉZIER - CÁMARA HÍBRIDA (DRON + ORBITAL)
 * ============================================================================
 * * NUEVAS CARACTERÍSTICAS:
 * 1. ORBITAL SYSTEM: Clic Derecho rota la cámara ALREDEDOR del objeto (0,0,0).
 * 2. FPS SYSTEM: Clic Izquierdo rota la VISTA desde la posición actual.
 * 3. MATH: Conversión de coordenadas esféricas para la órbita.
 * * * AUTOR: Usuario & Gema 1
 * ============================================================================
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================================
// 1. VARIABLES GLOBALES
// ============================================================================

// --- CÁMARA ---
float camX = 0.0f, camY = 3.0f, camZ = 7.0f;
// Vectores para Modo Dron (Mirar)
float yaw = -90.0f, pitch = -20.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = -1.0f;

// Variables para Modo Orbital (Satélite)
float orbitAngleH = 0.0f; // Ángulo Horizontal
float orbitAngleV = 0.5f; // Ángulo Vertical (Radianes)
float orbitRadius = 8.0f; // Distancia al centro

// --- CONTROL INPUT ---
int lastMouseX = -1, lastMouseY = -1;
int mouseState = 0; // 0: Nada, 1: Mirar (Izq), 2: Orbitar (Der)

// --- TOGGLES ---
bool showWireframe = false;   // F1
bool useAutoNormal = true;    // F2
bool showControlPoints = true;// F3
bool useLighting = true;      // F4

// --- ILUMINACIÓN ---
float lightPos[4] = { 0.0f, 4.0f, 4.0f, 1.0f };

// --- DATOS BÉZIER ---
GLfloat ctrlpoints[4][4][3] = {
   { {-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
   { {-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
   { {-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
   { {-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

// ============================================================================
// 2. MATEMÁTICAS DE CÁMARA
// ============================================================================

// MODO DRON: Calcula hacia dónde miro desde donde estoy
void updateCameraVectors() {
    float radYaw = yaw * (M_PI / 180.0f);
    float radPitch = pitch * (M_PI / 180.0f);

    dirX = cos(radPitch) * cos(radYaw);
    dirY = sin(radPitch);
    dirZ = cos(radPitch) * sin(radYaw);
    
    // Normalizar
    float len = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
    dirX /= len; dirY /= len; dirZ /= len;
}

// MODO ORBITAL: Calcula dónde estoy basado en el centro (0,0,0)
void updateOrbitPosition() {
    // Conversión: Esféricas -> Cartesianas
    // X = r * sin(V) * cos(H)
    // Z = r * sin(V) * sin(H)
    // Y = r * cos(V)
    
    camX = orbitRadius * sin(orbitAngleV) * cos(orbitAngleH);
    camZ = orbitRadius * sin(orbitAngleV) * sin(orbitAngleH);
    camY = orbitRadius * cos(orbitAngleV);

    // En modo orbital, siempre miramos al centro (0,0,0)
    // Así que recalculamos el vector dirección hacia el origen
    dirX = -camX;
    dirY = -camY;
    dirZ = -camZ;
    
    // Normalizamos dirección
    float len = sqrt(dirX*dirX + dirY*dirY + dirZ*dirZ);
    dirX /= len; dirY /= len; dirZ /= len;
    
    // Actualizamos Yaw/Pitch para que al soltar el clic derecho, 
    // el modo Dron no salte bruscamente (Sincronización inversa básica)
    pitch = asin(dirY) * (180.0f / M_PI);
    yaw = atan2(dirZ, dirX) * (180.0f / M_PI);
}

// ============================================================================
// 3. RENDERIZADO
// ============================================================================
void DibujarFoco() {
    glPushMatrix();
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutSolidSphere(0.1, 10, 10);
    if(useLighting) glEnable(GL_LIGHTING);
    glPopMatrix();
}

void DibujarPuntosDeControl() {
    if (!showControlPoints) return;
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(6.0f); // Puntos un poco más grandes
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            glVertex3fv(&ctrlpoints[i][j][0]);
        }
    }
    glEnd();
    if(useLighting) glEnable(GL_LIGHTING);
}

void init(void) {
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE); // Vital para Bezier

    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

    // Iluminación
    GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT, GL_SHININESS, 60);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // La cámara siempre usa la posición (camX,Y,Z) y mira hacia (cam+dir)
    // Esto funciona para ambos modos porque ambos actualizan estas variables.
    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0.0f, 1.0f, 0.0f);

    // Dibujar Luz
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    DibujarFoco();

    // Estados
    if (useLighting) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING);
    if (useAutoNormal) glEnable(GL_AUTO_NORMAL); else glDisable(GL_AUTO_NORMAL);

    // Dibujar Sábana
    glColor3f(0.8f, 0.85f, 0.9f); // Blanco azulado
    if (showWireframe) glEvalMesh2(GL_LINE, 0, 20, 0, 20);
    else glEvalMesh2(GL_FILL, 0, 20, 0, 20);

    DibujarPuntosDeControl();

    glutSwapBuffers();
}

// ============================================================================
// 4. SISTEMA DE INPUT HÍBRIDO
// ============================================================================

void mouseClick(int button, int state, int x, int y) {
    lastMouseX = x;
    lastMouseY = y;

    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) mouseState = 1; // Mirar
        if (button == GLUT_RIGHT_BUTTON) mouseState = 2; // Orbitar
    } else {
        mouseState = 0; // Soltado
    }
}

void mouseMove(int x, int y) {
    if (mouseState == 0) return;

    float xoffset = x - lastMouseX;
    float yoffset = lastMouseY - y;
    lastMouseX = x; lastMouseY = y;

    if (mouseState == 1) { 
        // --- MODO DRON (MIRAR) ---
        // Modifica Yaw/Pitch, no la posición
        yaw += xoffset * 0.2f;
        pitch += yoffset * 0.2f;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        
        updateCameraVectors();
    } 
    else if (mouseState == 2) {
        // --- MODO SATÉLITE (ORBITAR) ---
        // Modifica Ángulos de Órbita, recalcula Posición
        orbitAngleH += xoffset * 0.01f;
        orbitAngleV -= yoffset * 0.01f; // Invertido para naturalidad

        // Limitar ángulo vertical para no dar la vuelta completa (Gimbal lock visual)
        if (orbitAngleV < 0.1f) orbitAngleV = 0.1f;
        if (orbitAngleV > 3.0f) orbitAngleV = 3.0f; // Casi PI (3.14)

        updateOrbitPosition();
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float speed = 0.3f;
    switch (key) {
        case 27: exit(0); break;
        // Solo permitimos mover el dron en modo normal
        // En modo órbita, WASD podría confundir, pero lo dejamos activo por libertad
        case 'w': case 'W': camX += dirX * speed; camZ += dirZ * speed; break;
        case 's': case 'S': camX -= dirX * speed; camZ -= dirZ * speed; break;
        case 'a': case 'A': camX += dirZ * speed; camZ -= dirX * speed; break;
        case 'd': case 'D': camX -= dirZ * speed; camZ += dirX * speed; break;
        case 'q': case 'Q': camY -= speed; break;
        case 'e': case 'E': camY += speed; break;
        case '8': lightPos[1] += 0.5f; break;
        case '2': lightPos[1] -= 0.5f; break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1: showWireframe = !showWireframe; break;
        case GLUT_KEY_F2: useAutoNormal = !useAutoNormal; break;
        case GLUT_KEY_F3: showControlPoints = !showControlPoints; break;
        case GLUT_KEY_F4: useLighting = !useLighting; break;
        case GLUT_KEY_UP:    lightPos[2] -= 0.5f; break;
        case GLUT_KEY_DOWN:  lightPos[2] += 0.5f; break;
        case GLUT_KEY_LEFT:  lightPos[0] -= 0.5f; break;
        case GLUT_KEY_RIGHT: lightPos[0] += 0.5f; break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Laboratorio Bezier: Camara Hibrida");

    updateCameraVectors(); // Iniciar vista
    init();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);

    printf("--- CONTROLES AVANZADOS ---\n");
    printf("[Clic Izq + Arrastrar]: MIRAR (Modo Dron)\n");
    printf("[Clic Der + Arrastrar]: ORBITAR (Modo Satelite)\n");
    printf("[WASD]: Desplazarse\n");
    printf("[F1-F4]: Herramientas de Diagnostico\n");

    glutMainLoop();
    return 0;
}
