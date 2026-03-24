/*
=============================================================================
* Trimestre: 26 I
* Alumno: Jose Pablo Perez
* Asesor: Gema (1) - Asesor Gráficas Por Computadora
*
* TÉCNICAS PROBADAS (VERSIÓN POO + LEGACY GLUT):
* [X] 1-13. Cámara, Stencil, Texturas, Nodos, Sombras, Multi-OS.
* [X] 14. INYECCIÓN ALPHA PRECISA: Emparejamiento manual de máscaras BMP/JPG.
* [X] 15. TWO-SIDED LIGHTING: Iluminación de doble cara para salvar follaje 2D.
* [X] 16. SKYBOX: Solución de inversión nativa del formato TGA (Bottom-Up).
* [X] 17. Z-ORDERING MAGNO: Sombras postergadas y Skybox reflejado en el Stencil.
* [X] 18. DEPTH MASKING STENCIL: El reflejo ya no corta los objetos frontales.
* [X] 19. STATE LEAK FIX: Aislamiento de texturas para restaurar el Sol amarillo.
=============================================================================
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cctype>

// La implementación de STB Image se queda aquí para que se compile una sola vez
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// NUESTRAS CLASES MODULARES PROFESIONALES
#include "Camara.h"
#include "LuzSolar.h"
#include "Skybox.h"
#include "Escenario.h"
#include "Modelo3D.h"

// ========================================================================
// FUNCIONES GLOBALES DE UTILIDAD (Usadas por Escenario y Modelo3D)
// ========================================================================
std::string aMinusculas(const std::string& str) {
    std::string res = str;
    for(char& c : res) c = tolower(c);
    return res;
}

std::string ExtraerNombre(const std::string& path) {
    size_t idx = path.find_last_of("\\/");
    std::string filename = (idx != std::string::npos) ? path.substr(idx + 1) : path;
    filename.erase(filename.find_last_not_of(" \n\r\t") + 1);
    filename.erase(0, filename.find_first_not_of(" \n\r\t"));
    return filename;
}

GLuint CargarTexturaConAlpha(std::string filename, bool esHoja) {
    if (filename.empty()) return 0;
    
    std::string rutaDiffuse = "Textures/" + filename;
    int w, h, c;
    unsigned char *data = stbi_load(rutaDiffuse.c_str(), &w, &h, &c, 4); 
    
    if (!data) {
        std::string filenameMin = aMinusculas(filename);
        rutaDiffuse = "Textures/" + filenameMin;
        data = stbi_load(rutaDiffuse.c_str(), &w, &h, &c, 4);
    }

    if (!data) {
        printf("ADVERTENCIA: Textura perdida -> %s\n", filename.c_str());
        return 0; 
    }

    std::string rutaAlpha = "";
    std::string fMin = aMinusculas(filename);
    
    if (fMin.find("leaves_diffcol") != std::string::npos) rutaAlpha = "Textures/Leaves_trancol.bmp";
    else if (fMin.find("blatt1") != std::string::npos) rutaAlpha = "Textures/blatt1_a.jpg";
    else {
        size_t dot = rutaDiffuse.find_last_of(".");
        if (dot != std::string::npos) rutaAlpha = rutaDiffuse.substr(0, dot) + "_a" + rutaDiffuse.substr(dot);
    }

    bool alphaFusionado = false;
    if (!rutaAlpha.empty()) {
        int aw, ah, ac;
        unsigned char *alphaData = stbi_load(rutaAlpha.c_str(), &aw, &ah, &ac, 1); 
        if (!alphaData) {
            size_t slash = rutaAlpha.find_last_of("\\/");
            std::string alphaName = (slash != std::string::npos) ? rutaAlpha.substr(slash+1) : rutaAlpha;
            rutaAlpha = "Textures/" + aMinusculas(alphaName);
            alphaData = stbi_load(rutaAlpha.c_str(), &aw, &ah, &ac, 1);
        }

        if (alphaData && aw == w && ah == h) {
            for (int i = 0; i < w * h; i++) data[i * 4 + 3] = alphaData[i]; 
            alphaFusionado = true;
            stbi_image_free(alphaData);
        }
    }

    if (!alphaFusionado && esHoja) {
        for (int i = 0; i < w * h; i++) {
            int r = data[i * 4]; int g = data[i * 4 + 1]; int b = data[i * 4 + 2];
            if (r > 240 && g > 240 && b > 240) data[i * 4 + 3] = 0; 
            else if (r < 15 && g < 15 && b < 15) data[i * 4 + 3] = 0; 
            else data[i * 4 + 3] = 255;
        }
    }

    GLuint tid; 
    glGenTextures(1, &tid); 
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tid;
}

// ========================================================================
// INSTANCIAS Y MAIN LOOP
// ========================================================================
Camara miCamara;
LuzSolar miSol;
Escenario miParque;
Modelo3D miArbol;
Skybox miCielo; 

void display() {
    miSol.actualizar();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    
    miCamara.aplicarVista();

    // 1. SKYBOX (Cielo infinito)
    miCielo.dibujar(0.0f, 15.0f, miCamara.distancia);

    // 2. MUNDO FÍSICO
    miSol.dibujarYIluminar();
    miParque.dibujarPasto();
    miArbol.dibujar(false); 

    // 3. MÁSCARA STENCIL PARA EL REFLEJO
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1); glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_DEPTH_TEST); glDepthMask(GL_FALSE); 
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    
    miParque.dibujarAgua(false); 
    
    // 4. MUNDO INVERTIDO (EL ESPEJO)
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE); 
    glEnable(GL_DEPTH_TEST);
    glStencilFunc(GL_EQUAL, 1, 1); glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 

    glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f); glScalef(1.0f, -1.0f, 1.0f); glFrontFace(GL_CW);              
        
        miCielo.dibujar(0.0f, 15.0f, miCamara.distancia); 
        miSol.dibujarYIluminar();
        miArbol.dibujar(false); 
        
        glFrontFace(GL_CCW);
    glPopMatrix();
    glDisable(GL_STENCIL_TEST);

    // 5. TEXTURA DEL AGUA CON OLAS 
    miParque.dibujarAgua(true);

    // 6. SOMBRAS AL FINAL 
    if(miSol.posY > 0.0f) {
        glPushMatrix();
            glDisable(GL_LIGHTING); 
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE); 
            
            glTranslatef(0.0f, 0.1f, 0.0f); 
            miSol.aplicarMatrizSombra();
            miArbol.dibujar(true); 
            
            glDepthMask(GL_TRUE); 
            glDisable(GL_BLEND); 
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 2000.0); 
    glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: exit(0); break;
        case 'w': miCamara.distancia -= 2.0f; break; case 's': miCamara.distancia += 2.0f; break; 
        case 'm': miSol.automatico = !miSol.automatico; break;
        case 'j': if(!miSol.automatico) miSol.angulo -= 2.0f; break;
        case 'l': if(!miSol.automatico) miSol.angulo += 2.0f; break;
    }
}
void mouse(int b, int s, int x, int y) { miCamara.mouseClick(b, s, x, y); }
void motion(int x, int y) { miCamara.mouseMove(x, y); }

void init() {
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    miCielo.cargar(); 
    miParque.cargarTexturas();
    miArbol.cargar("models/tree.obj");
    
    glEnable(GL_DEPTH_TEST); glEnable(GL_CULL_FACE); glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    GLfloat l_diff[] = { 0.9f, 0.9f, 0.8f, 1.0f }; 
    GLfloat l_amb[]  = { 0.5f, 0.5f, 0.5f, 1.0f }; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Trimestre 26 I - Parque Estructurado y Refactorizado");
    init(); 
    glutDisplayFunc(display); glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); glutMouseFunc(mouse); glutMotionFunc(motion);
    glutIdleFunc(idle); glutMainLoop(); 
    return 0;
}

