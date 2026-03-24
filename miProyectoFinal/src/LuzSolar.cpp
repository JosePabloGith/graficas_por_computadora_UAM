#include "LuzSolar.h"

// Cumplimiento del Constructor: Inicializa las variables
LuzSolar::LuzSolar() {
    angulo = 60.0f;
    distancia = 100.0f;
    velocidad = 0.01f;
    automatico = true;
}

// Cumplimiento del Destructor
LuzSolar::~LuzSolar() {
    // No hay punteros ni memoria dinámica que liberar por ahora
}

// Lógica para mover el sol en el cielo
void LuzSolar::actualizar() {
    if (automatico) { 
        angulo += velocidad; 
        if(angulo > 360.0f) angulo -= 360.0f; 
    }
    float rad = angulo * (M_PI / 180.0f);
    posX = -cos(rad) * distancia; 
    posY = sin(rad) * distancia; 
    posZ = -20.0f;
}

// Lógica para renderizar la esfera amarilla y posicionar GL_LIGHT0
void LuzSolar::dibujarYIluminar() {
    if (posY > 0) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0); 
    
    glPushMatrix(); 
    glTranslatef(posX, posY, posZ); 
    float pLuz[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, pLuz);
    
    if(posY > 0) {
        glDisable(GL_LIGHTING); 
        glDisable(GL_TEXTURE_2D); // Protege contra el State Leak (El Sol Negro)
        glColor3f(1.0f, 0.9f, 0.5f); 
        glutSolidSphere(4.0, 30, 30); 
        glEnable(GL_LIGHTING); 
    }
    glPopMatrix();
}

// Matemáticas para la proyección de sombras planas
void LuzSolar::aplicarMatrizSombra() {
    float shadowMat[16] = {0};
    shadowMat[0] = posY; shadowMat[10] = posY; shadowMat[15] = posY;
    shadowMat[4] = -posX; shadowMat[6] = -posZ; shadowMat[7] = -1.0f;
    glMultMatrixf(shadowMat);
}
