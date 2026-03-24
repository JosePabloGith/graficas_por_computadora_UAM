#include "Camara.h"

// Cumplimiento del Constructor
Camara::Camara() {
    distancia = 60.0f;
    rotX = 20.0f;
    rotY = 0.0f;
    isDragging = false;
}

// Cumplimiento del Destructor
Camara::~Camara() {
    // Sin memoria dinámica que liberar por ahora
}

// Cumplimiento de la lógica de vista
void Camara::aplicarVista() {
    gluLookAt(0.0, 15.0, distancia, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);       
    glRotatef(rotX, 1.0, 0.0, 0.0); 
    glRotatef(rotY, 0.0, 1.0, 0.0);
}

// Cumplimiento de los clicks del mouse
void Camara::mouseClick(int b, int s, int x, int y) {
    if (b == GLUT_LEFT_BUTTON) { 
        isDragging = (s == GLUT_DOWN); 
        lastX = x; 
        lastY = y; 
    }
    if (s == GLUT_DOWN) { 
        if (b == 3) distancia -= 3.0f; // Scroll Up
        if (b == 4) distancia += 3.0f; // Scroll Down
    }
}

// Cumplimiento del movimiento del mouse
void Camara::mouseMove(int x, int y) {
    if (isDragging) { 
        rotY += (x - lastX) * 0.5f; 
        rotX += (y - lastY) * 0.5f; 
        lastX = x; 
        lastY = y; 
    }
}
