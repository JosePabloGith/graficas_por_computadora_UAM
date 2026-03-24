#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <GL/glut.h>

class Escenario {
public:
    // Atributos
    GLuint texPasto, texAgua;
    float radioX, radioZ, lim;
    int segmentos;

    // Promesas / Interfaz
    Escenario();  // Constructor
    ~Escenario(); // Destructor

    void cargarTexturas();
    void dibujarPasto();
    void dibujarAgua(bool transparente);
};

#endif
