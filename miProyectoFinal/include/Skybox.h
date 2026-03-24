/*
 * Este archivo es la cabecera de la caja magica la cual simula el cielo
 * consideramos que en realidad es una caja de 6 caras
 * la cual usamos para simular el cielo :D
 * */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glut.h>
#include <string>

class Skybox {
private:
    // Atributos privados (El estado oculto)
    GLuint tex[6];
    
    // Método privado (Solo el Skybox puede usar esto, es su secreto)
    GLuint cargarTexturaSimple(std::string rutaRelativa);

public:
    // Promesas / Interfaz pública
    Skybox();  // Constructor
    ~Skybox(); // Destructor

    void cargar();
    void dibujar(float camX, float camY, float camZ);
};

#endif
