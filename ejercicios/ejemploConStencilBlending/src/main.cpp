/**
 * ============================================================================
 * PROYECTO: MOTOR GRÁFICO UAM - FASE 3 (REFLEJO + STENCIL + BLENDING + LUZ)
 * ============================================================================
 * * DESCRIPCIÓN:
 * Motor gráfico con efectos de espejo, sombras dinámicas y superficies translúcidas.
 * * CORRECCIONES APLICADAS:
 * 1. Fix Compilación: Unificación de variable 'radioSeguridad'.
 * 2. Fix Visual: Activación de ILUMINACIÓN (GL_LIGHT0) para dar vida 3D.
 * 3. Fix Color: Agua más clara para mejorar visibilidad.
 * 4. Fix Materiales: GL_COLOR_MATERIAL activado para que los colores no se vean negros.
 * * AUTOR: Pablo & Gema 1
 * SISTEMA: Ubuntu 24.04 (Linux)

	Consideraciones, con la nueva actualización podemos encontrar la caracteristica de iluminado dinamico
	el cual se puede observar cuando movemos el foco, dependiendo de la cercania del foco con el cubo
	la cara mas cercana al foco aumentara su brillo simulando proximidad con la fuente de luz
	puntual
	** se logra gracias a la combinacion de 3 conceptos 
		-- Iluminacion puntual pointLight
		-- ley de lambert (calculo difuso)
		-- atenuación por distancia
 */

#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h> // Necesario para exit(0) en Linux
#include <GL/glut.h> // Cabecera estándar para Linux

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

// CÁMARA
float camDist = 25.0f;
float camAngleX = 0.0f;
float camAngleY = 20.0f;
int lastMouseX, lastMouseY;
int interactionMode = 0;

// OBJETOS
float escalaCubo = 1.0f;
bool bMostrarEsfera = false;
bool animacionActiva = false;
static GLfloat theta[] = { 0.0f, 0.0f, 0.0f };

// ILUMINACIÓN
// Posición [x, y, z, w]. w=1.0 es luz posicional (foco), w=0.0 es direccional (sol)
float lightPos[4] = { -4.0f, 4.0f, 4.0f, 1.0f };
float alturaLuz = 4.0f;

// PISO (Plano Y = -2.0)
GLfloat floorPlane[4] = { 0.0f, 1.0f, 0.0f, 2.0f };

// DEBUG FLAGS
bool bCull = false;      // F1
bool bDepth = true;      // F2
bool bWireframe = false; // F3
bool bSmooth = false;    // F4

// ============================================================================
// SECCIÓN DE MODELADO
// ============================================================================

void cara0() {
    // Si suavizado está activo, usamos normales y colores por vértice
    if (bSmooth) {
        glShadeModel(GL_SMOOTH);
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(1., 1., 0);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(1., -1., 0);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(-1., -1., 0);
        glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-1., 1., 0);
        glEnd();
    } else {
        glShadeModel(GL_FLAT);
        glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1., 1., 0);
        glVertex3f(1., -1., 0);
        glVertex3f(-1., -1., 0);
        glVertex3f(-1., 1., 0);
        glEnd();
    }
}

void cara1() { glPushMatrix(); glTranslatef(0., 0., 1.); cara0(); glPopMatrix(); }
void cara2() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara1(); glPopMatrix(); }
void cara3() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara2(); glPopMatrix(); }
void cara4() { glPushMatrix(); glRotatef(90., 1., 0., 0.); cara3(); glPopMatrix(); }
void cara5() { glPushMatrix(); glRotatef(90., 0., 1., 0.); cara1(); glPopMatrix(); }
void cara6() { glPushMatrix(); glRotatef(-90., 0., 1., 0.); cara1(); glPopMatrix(); }

void colorcube(void) {
    glColor3f(0.5, 0., 0.); cara1();
    glColor3f(0., 0.5, 0.); cara2();
    glColor3f(0., 0., 0.5); cara3();
    glColor3f(0., 0.5, 0.5); cara4();
    glColor3f(0.5, 0.5, 0.); cara5();
    glColor3f(0.5, 0., 0.5); cara6();
}

void colorcube_sin_color(void) {
    cara1(); cara2(); cara3(); cara4(); cara5(); cara6();
}

// ============================================================================
// FUNCIONES AUXILIARES
// ============================================================================

void DibujarPiso() {
    // FIX DE VIDA: Color más brillante para el agua (Cian/Azul Claro)
    // Alpha 0.6 para transparencia
    glColor4f(0.2f, 0.4f, 0.8f, 0.6f); 
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal hacia arriba para que la luz rebote
    glVertex3f(-30.0f, -2.0f, -30.0f);
    glVertex3f(-30.0f, -2.0f, 30.0f);
    glVertex3f(30.0f, -2.0f, 30.0f);
    glVertex3f(30.0f, -2.0f, -30.0f);
    glEnd();
}

void DibujarFoco() {
    glPushMatrix();
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    
    // Desactivamos iluminación para que el foco brille por sí mismo (emisivo)
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo Puro
    glutSolidSphere(0.2, 10, 10);
    glEnable(GL_LIGHTING); // Reactivamos para el resto
    
    glPopMatrix();
}

void DibujarCuboEnPosicion() {
    glPushMatrix();
    glRotatef(theta[0], 1.0, 0.0, 0.0);
    glRotatef(theta[1], 0.0, 1.0, 0.0);
    glScalef(escalaCubo, escalaCubo, escalaCubo);
    colorcube();
    glPopMatrix();
}

void gltMakeShadowMatrix(GLfloat vPlaneEquation[], GLfloat vLightPos[], GLfloat destMat[]) {
    GLfloat dot;
    dot = vPlaneEquation[0] * vLightPos[0] + vPlaneEquation[1] * vLightPos[1] +
        vPlaneEquation[2] * vLightPos[2] + vPlaneEquation[3] * vLightPos[3];

    destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
    destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
    destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
    destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];

    destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
    destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
    destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
    destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];

    destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
    destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
    destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
    destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];

    destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
    destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
    destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
    destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}

// ============================================================================
// CONFIGURACIÓN INICIAL (El secreto de la "Vida")
// ============================================================================
void SetupRC() {
    // 1. Configurar Luz Ambiental y Difusa
    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Luz base suave
    GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f }; // Luz direccional fuerte
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // 2. Habilitar "Materiales de Color"
    // Esto hace que glColor() funcione como propiedades del material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 3. Encender el interruptor principal
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // 4. Otros ajustes
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fondo Negro
}

// ============================================================================
// CONTROLES
// ============================================================================

void keyboard(unsigned char key, int x, int y) {
    // FIX COMPILACIÓN: Variable unificada 'radioSeguridad'
    float radioSeguridad = 1.8f * escalaCubo;

    switch (key) {
    case 'w': case 'W': if (camDist > 5.0f) camDist -= 0.5f; break;
    case 's': case 'S': camDist += 0.5f; break;
    case 27: exit(0); break;

    case '8': { // Subir Luz
        float fA = alturaLuz + 0.5f;
        float dF = sqrt(pow(lightPos[0], 2) + pow(fA, 2) + pow(lightPos[2], 2));
        if (dF > radioSeguridad) { 
            alturaLuz = fA;
            lightPos[1] = alturaLuz;
            // Actualizar la posición de la luz en OpenGL
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        }
        break;
    }

    case '2': // Bajar Luz
    {
        float fA = alturaLuz - 0.5f;
        float dF = sqrt(pow(lightPos[0], 2) + pow(fA, 2) + pow(lightPos[2], 2));
        if (dF > radioSeguridad && fA > -2.0f) {
            alturaLuz = fA;
            lightPos[1] = alturaLuz;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        }
        break;
    }

    case 'f': case 'F': bMostrarEsfera = !bMostrarEsfera; break;
    case '+': escalaCubo += 0.1f; break;
    case '-': if (escalaCubo > 0.2f) escalaCubo -= 0.1f; break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_F1) bCull = !bCull;
    if (key == GLUT_KEY_F2) bDepth = !bDepth;
    if (key == GLUT_KEY_F3) bWireframe = !bWireframe;
    if (key == GLUT_KEY_F4) bSmooth = !bSmooth;

    float rS = 1.8f * escalaCubo;
    float nX = lightPos[0], nZ = lightPos[2];
    bool mov = false;
    
    if (key == GLUT_KEY_UP) { nZ -= 0.5f; mov = true; }
    if (key == GLUT_KEY_DOWN) { nZ += 0.5f; mov = true; }
    if (key == GLUT_KEY_LEFT) { nX -= 0.5f; mov = true; }
    if (key == GLUT_KEY_RIGHT) { nX += 0.5f; mov = true; }

    if (mov) {
        if ((nX*nX + alturaLuz*alturaLuz + nZ*nZ) > rS*rS) {
            lightPos[0] = nX; lightPos[2] = nZ;
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos); // Actualizar luz
        }
    }
    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) { lastMouseX = x; lastMouseY = y; }
    if (button == GLUT_LEFT_BUTTON) interactionMode = (state == GLUT_DOWN) ? 1 : 0;
    else if (button == GLUT_RIGHT_BUTTON) interactionMode = (state == GLUT_DOWN) ? 2 : 0;
}

void mouseMove(int x, int y) {
    if (interactionMode == 0) return;
    int dx = x - lastMouseX, dy = y - lastMouseY;
    if (interactionMode == 1) {
        camAngleX += dx * 0.4f; camAngleY += dy * 0.4f;
        if (camAngleY > 89.0f) camAngleY = 89.0f;
        if (camAngleY < -89.0f) camAngleY = -89.0f;
    } else if (interactionMode == 2) {
        theta[1] += dx * 0.4f; theta[0] += dy * 0.4f;
    }
    lastMouseX = x; lastMouseY = y;
    glutPostRedisplay();
}

// ============================================================================
// RENDERIZADO (DISPLAY)
// ============================================================================

void display(void) {
    // 1. LIMPIEZA
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    // 2. DEBUG STATES
    if (bCull) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (bDepth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (bWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (bSmooth) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);

    // 3. CÁMARA
    float rX = camAngleX * 0.01745f, rY = camAngleY * 0.01745f;
    gluLookAt(camDist * sin(rX) * cos(rY), camDist * sin(rY), camDist * cos(rX) * cos(rY),
              0, 0, 0, 0, 1, 0);

    // Actualizar posición de luz en cada cuadro (por si la cámara se mueve)
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    DibujarFoco();

    // =========================================================
    // FASE 1: STENCIL (MÁSCARA PISO)
    // =========================================================
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    DibujarPiso(); 
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // =========================================================
    // FASE 2: REFLEJO
    // =========================================================
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glPushMatrix();
        glTranslatef(0.0f, -2.0f, 0.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        glTranslatef(0.0f, 2.0f, 0.0f);
        glFrontFace(GL_CW); 
        DibujarCuboEnPosicion();
        glFrontFace(GL_CCW);
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);

    // =========================================================
    // FASE 3: AGUA
    // =========================================================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DibujarPiso();
    glDisable(GL_BLEND);

    // =========================================================
    // FASE 4: SOMBRAS (CORREGIDAS)
    // =========================================================
    glEnable(GL_STENCIL_TEST); // Recorte sombra
    glStencilFunc(GL_EQUAL, 1, 1);
    
    GLboolean lightingOn = glIsEnabled(GL_LIGHTING);
    GLboolean cullingOn = glIsEnabled(GL_CULL_FACE);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING); // Sombra plana sin luz
    glDisable(GL_CULL_FACE); // Ver ambas caras de la sombra

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
        GLfloat sM[16];
        gltMakeShadowMatrix(floorPlane, lightPos, sM);
        glMultMatrixf(sM);
        glRotatef(theta[0], 1.0, 0.0, 0.0);
        glRotatef(theta[1], 0.0, 1.0, 0.0);
        glScalef(escalaCubo, escalaCubo, escalaCubo);
        
        if (bWireframe) glColor3f(1.0f, 1.0f, 1.0f);
        else glColor4f(0.0f, 0.0f, 0.0f, 0.4f); // Sombra suave

        colorcube_sin_color();
    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    if (lightingOn) glEnable(GL_LIGHTING);
    if (cullingOn) glEnable(GL_CULL_FACE);
    glDisable(GL_STENCIL_TEST);

    // =========================================================
    // FASE 5: OBJETO REAL
    // =========================================================
    if (bMostrarEsfera) {
        glPushMatrix();
            glRotatef(theta[0], 1.0, 0.0, 0.0);
            glRotatef(theta[1], 0.0, 1.0, 0.0);
            glDisable(GL_LIGHTING); glColor3f(1.0f, 1.0f, 1.0f);
            glutWireSphere(1.8f * escalaCubo, 15, 15);
            if (lightingOn) glEnable(GL_LIGHTING);
        glPopMatrix();
    }
    DibujarCuboEnPosicion();

    glutSwapBuffers();
}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLfloat)w / (GLfloat)h, 1.0f, 300.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fase 3: Iluminacion + Stencil + Blending");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    // CONFIGURACIÓN DE LUCES Y MATERIALES
    SetupRC(); 

    glutMainLoop();
    return 0;
}
