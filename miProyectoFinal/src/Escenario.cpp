#include "Escenario.h"
#include <math.h>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Le avisamos al compilador que esta función global está en main.cpp
extern GLuint CargarTexturaConAlpha(std::string filename, bool esHoja);

// Cumplimiento del Constructor
Escenario::Escenario() {
    radioX = 8.0f;
    radioZ = 5.0f;
    lim = 400.0f;
    segmentos = 50;
    texPasto = 0;
    texAgua = 0;
}

// Cumplimiento del Destructor
Escenario::~Escenario() {
    // Buena práctica: Limpiar las texturas de la memoria de video al cerrar
    if (texPasto != 0) glDeleteTextures(1, &texPasto);
    if (texAgua != 0) glDeleteTextures(1, &texAgua);
}

void Escenario::cargarTexturas() {
    texPasto = CargarTexturaConAlpha("Grass.png", false);
    texAgua = CargarTexturaConAlpha("Water_Po.jpg", false);
}

void Escenario::dibujarPasto() {
    glDisable(GL_CULL_FACE); 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texPasto);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.0f, 1.0f, 1.0f); 

    for(int anillo = 0; anillo < 15; anillo++) {
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= segmentos; i++) {
            float angulo = (i * 2.0f * M_PI) / segmentos;
            float rBx = radioX + sin(angulo * 5.0f) * 2.0f;
            float rBz = radioZ + cos(angulo * 4.0f) * 2.0f;
            float f1 = (float)anillo / 15.0f;
            float f2 = (float)(anillo + 1) / 15.0f;
            float x1 = cos(angulo) * (rBx + f1 * (lim - rBx));
            float z1 = sin(angulo) * (rBz + f1 * (lim - rBz));
            float x2 = cos(angulo) * (rBx + f2 * (lim - rBx));
            float z2 = sin(angulo) * (rBz + f2 * (lim - rBz));
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(x1/10.0f, z1/10.0f); glVertex3f(x1, 0.0f, z1);
            glTexCoord2f(x2/10.0f, z2/10.0f); glVertex3f(x2, 0.0f, z2);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.15f, 0.1f, 0.05f); 
    glBegin(GL_TRIANGLE_FAN); 
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.5f, 0.0f); 
        for(int i = 0; i <= segmentos; i++) {
            float a = (i * 2.0f * M_PI) / segmentos;
            glVertex3f(cos(a) * (radioX + sin(a * 5.0f) * 2.0f), -1.5f, sin(a) * (radioZ + cos(a * 4.0f) * 2.0f));
        }
    glEnd();
    glEnable(GL_CULL_FACE); 
}

void Escenario::dibujarAgua(bool transparente) {
    glDisable(GL_CULL_FACE); 
    if (transparente) {
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, texAgua);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor4f(1.0f, 1.0f, 1.0f, 0.55f); 
    } else {
        glDisable(GL_TEXTURE_2D); glColor4f(0.0f, 0.0f, 0.0f, 1.0f); 
    }
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -0.5f, 0.0f); 
        for(int i = 0; i <= segmentos; i++) {
            float a = (i * 2.0f * M_PI) / segmentos;
            float px = cos(a) * (radioX + sin(a * 5.0f) * 2.0f);
            float pz = sin(a) * (radioZ + cos(a * 4.0f) * 2.0f);
            glTexCoord2f(px/5.0f, pz/5.0f); glVertex3f(px, -0.5f, pz);
        }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    if (transparente) glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE); 
}
