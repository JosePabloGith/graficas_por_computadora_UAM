#ifndef LUZSOLAR_H
#define LUZSOLAR_H

#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class LuzSolar {
public:
    // Atributos (El estado del objeto)
    float angulo, distancia, velocidad, posX, posY, posZ;
    bool automatico;

    // Promesas / Interfaz (El comportamiento del objeto)
    LuzSolar();  // Constructor
    ~LuzSolar(); // Destructor

    void actualizar();
    void dibujarYIluminar();
    void aplicarMatrizSombra();
};

#endif
