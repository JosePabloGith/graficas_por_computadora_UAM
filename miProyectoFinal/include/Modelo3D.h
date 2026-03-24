#ifndef MODELO3D_H
#define MODELO3D_H

#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Modelo3D {
private:
    // Estructura interna (Secreto de la clase)
    struct SubMalla {
        GLuint displayList;
        GLuint texID;
        bool esHoja;
    };
    
    // Lista de mallas que componen el modelo
    std::vector<SubMalla> mallas;

public:
    // Promesas / Interfaz
    Modelo3D();  // Constructor
    ~Modelo3D(); // Destructor para limpiar memoria de video

    void cargar(const char* path);
    void dibujarUnArbol(bool esSombra);
    void dibujar(bool esSombra);
};

#endif
