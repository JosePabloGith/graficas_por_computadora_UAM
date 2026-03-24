/*
 * En este archivo implementamos las promezas realizadas en la cabecera
 * aqui construimos la caja 
 * */

#include "Skybox.h"
#include "stb_image.h"

// Cumplimiento del Constructor
Skybox::Skybox() {
    for(int i = 0; i < 6; i++) {
        tex[i] = 0; // Inicializamos los 6 tickets en cero (vacíos)
    }
}

// Cumplimiento del Destructor
Skybox::~Skybox() {
    // Cuando el Skybox muere, le devolvemos la memoria a la Tarjeta Gráfica
    glDeleteTextures(6, tex);
}

// Nuestro método secreto (privado) para cargar imágenes TGA
GLuint Skybox::cargarTexturaSimple(std::string rutaRelativa) {
    int w, h, c;
    unsigned char *data = stbi_load(rutaRelativa.c_str(), &w, &h, &c, 3); 
    if (!data) {
        printf("ADVERTENCIA: No se encontro la textura del Skybox: %s\n", rutaRelativa.c_str());
        return 0;
    }
    GLuint tid; 
    glGenTextures(1, &tid); 
    glBindTexture(GL_TEXTURE_2D, tid);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tid; // Retornamos el "Ticket"
}

// Cargamos las 6 caras (Del 0 al 5)
// considera que estamos haciendo uso de las imagenes encontradas 
// dentro de internet, para poder simular el cielo
void Skybox::cargar() {
    tex[0] = cargarTexturaSimple("Textures/Skybox/hills_rt.tga"); 
    tex[1] = cargarTexturaSimple("Textures/Skybox/hills_lf.tga"); 
    tex[2] = cargarTexturaSimple("Textures/Skybox/hills_up.tga"); 
    tex[3] = cargarTexturaSimple("Textures/Skybox/hills_dn.tga"); 
    tex[4] = cargarTexturaSimple("Textures/Skybox/hills_bk.tga"); 
    tex[5] = cargarTexturaSimple("Textures/Skybox/hills_ft.tga"); 
}

// Dibujamos el cubo gigante
void Skybox::dibujar(float camX, float camY, float camZ) {
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_LIGHTING);   
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE); 
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(camX, camY, camZ);
    float s = 500.0f; 

    // Derecha
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s, -s,  s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s,  s, -s);
    glEnd();

    // Izquierda
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s,  s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s,  s);
    glEnd();

    // Arriba
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s,  s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s,  s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s,  s);
    glEnd();

    // Abajo
    glBindTexture(GL_TEXTURE_2D, tex[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s, -s,  s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s,  s);
    glEnd();

    // Atrás
    glBindTexture(GL_TEXTURE_2D, tex[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -s,  s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s,  s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s,  s,  s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( s,  s,  s);
    glEnd();

    // Frente
    glBindTexture(GL_TEXTURE_2D, tex[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( s,  s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s,  s, -s);
    glEnd();

    glPopMatrix();
    glPopAttrib(); 
}
