#ifndef CAMARA_H
#define CAMARA_H

#include <GL/glut.h>

class Camara {
public:
    // Atributos
    float distancia, rotX, rotY;
    bool isDragging;
    int lastX, lastY;

    // Promesas (Prototipos de funciones)
    Camara();  // Constructor
    ~Camara(); // Destructor (Buena práctica)

    void aplicarVista();
    void mouseClick(int b, int s, int x, int y);
    void mouseMove(int x, int y);
};

#endif
