/*
 * Creador: Jose Pablo Perez De Anda
 * Descripción: Ejemplo del uso de la tecnica HUB 2D sobre objetos 3D.
 * Tecnica de mapeo de letras : Bitmap
 *
 * Consideraciones: Para poder realizar este ejmplo
 * hicimos pruebas de obtencion de fuentes bitmap dentro de la red
 * Del mismo descubrimos que se puede extrer la tipografia y el estilo
 * mediante una matriz enorme de bytes, cada letra se representa con 16 bytes (8x16 pixeles)
 * y cada bit representa un pixel encendido o apagado, con esto se puede crear cualquier letra o simbolo, incluso animaciones
 * si se cambia la matriz en cada frame.
 *
 * El objetivo es usar esta tipografia dentro de nuestro proyecto de entrega, es por ello que nos vimos
 * en la nececidad de implementar este ejemplo en concreto usando de fondo una tetera, ademas
 * de agragar una funcionalidad
 * Es por ello que se mapea una tetera en 3D y se le agrega un texto en 2D encima, el cual se puede modificar con el teclado, ademas de agregar una animacion a la tetera para mostrar que el texto se mantiene fijo sobre el mundo 3D, es decir, que no se ve afectado por la rotacion de la tetera.
 */


#include <iostream>
#include <string>
#include <cctype>
#include <GL/glut.h>

// --- 1. ESTADO GLOBAL ---
std::string buffer_texto = "ESCRIBE AQUI SOBRE EL 3D"; // Texto inicial
int ANCHO_VENTANA = 600;
int ALTO_VENTANA = 400;

const int ANCHO_LETRA_BASE = 8;
const int ALTO_LETRA_BASE = 16;
float ESCALA = 2.0f;
int ANCHO_LETRA = ANCHO_LETRA_BASE * ESCALA;
int ALTO_LETRA = ALTO_LETRA_BASE * ESCALA;

// Variable para animar nuestro mundo 3D
float angulo_tetera = 0.0f;

// --- 2. LA TIPOGRAFÍA (A-Z 8x16) ---
GLubyte mi_fuente[26][16] = {
    {0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0xFF,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x00,0x00}, // A
    {0x00,0x00,0x00,0x7F,0xC3,0xC3,0xC3,0x7F,0xC3,0xC3,0xC3,0xC3,0x7F,0x00,0x00,0x00}, // B
    {0x00,0x00,0x00,0x3E,0x63,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x63,0x3E,0x00,0x00,0x00}, // C
    {0x00,0x00,0x00,0x7E,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x7E,0x00,0x00,0x00}, // D
    {0x00,0x00,0x00,0xFF,0xC0,0xC0,0xC0,0xFC,0xC0,0xC0,0xC0,0xC0,0xFF,0x00,0x00,0x00}, // E
    {0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xFC,0xC0,0xC0,0xC0,0xC0,0xFF,0x00,0x00,0x00}, // F
    {0x00,0x00,0x00,0x3E,0x63,0xC3,0xC3,0xC0,0xC0,0xC0,0xC0,0x63,0x3E,0x00,0x00,0x00}, // G
    {0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // H
    {0x00,0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00}, // I
    {0x00,0x00,0x00,0x3E,0x63,0xC3,0xC3,0xC3,0xC3,0x03,0x03,0x03,0x0F,0x00,0x00,0x00}, // J
    {0x00,0x00,0x00,0xC3,0xC6,0xCC,0xD8,0xF0,0xD8,0xCC,0xC6,0xC3,0xC3,0x00,0x00,0x00}, // K
    {0x00,0x00,0x00,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00}, // L
    {0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0xDB,0xFF,0xFF,0xE7,0xC3,0xC3,0x00,0x00,0x00}, // M
    {0x00,0x00,0x00,0xC3,0xC3,0xC7,0xCF,0xDB,0xF3,0xE3,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // N
    {0x00,0x00,0x00,0x3E,0x63,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x63,0x3E,0x00,0x00,0x00}, // O
    {0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0x7F,0xC3,0xC3,0xC3,0xC3,0x7F,0x00,0x00,0x00}, // P
    {0x00,0x00,0x00,0x3D,0x6B,0xC7,0xC3,0xC3,0xC3,0xC3,0xC3,0x63,0x3E,0x00,0x00,0x00}, // Q
    {0x00,0x00,0x00,0xC3,0xC6,0xCC,0xD8,0x7F,0xC3,0xC3,0xC3,0xC3,0x7F,0x00,0x00,0x00}, // R
    {0x00,0x00,0x00,0x3E,0x63,0x03,0x03,0x3E,0x60,0xC0,0xC0,0x63,0x3E,0x00,0x00,0x00}, // S
    {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xFF,0x00,0x00,0x00}, // T
    {0x00,0x00,0x00,0x3E,0x63,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // U
    {0x00,0x00,0x00,0x18,0x3C,0x66,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // V
    {0x00,0x00,0x00,0xC3,0xC3,0xE7,0xFF,0xFF,0xDB,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // W
    {0x00,0x00,0x00,0xC3,0xC3,0x66,0x3C,0x18,0x3C,0x66,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // X
    {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x3C,0x66,0xC3,0xC3,0xC3,0x00,0x00,0x00}, // Y
    {0x00,0x00,0x00,0xFF,0xC0,0xC0,0x60,0x30,0x18,0x0C,0x06,0x03,0xFF,0x00,0x00,0x00}  // Z
};

void init(void) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Un fondo ligeramente azul noche
    glEnable(GL_DEPTH_TEST); // ¡IMPORTANTE PARA EL 3D! Habilita la física de oclusión
}

// --- 3. EL MÓDULO AISLADO DE TEXTO (Lo que usarás en tu proyecto) ---
void renderizarHUD() {
    // A. GUARDAR ESTADO 3D Y CAMBIAR A 2D (EL "CRISTAL")
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // Guardamos el lente de perspectiva 3D
    glLoadIdentity();
    glOrtho(0, ANCHO_VENTANA, 0, ALTO_VENTANA, -1.0, 1.0); // Lente Plano 2D

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // Guardamos la cámara 3D
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST); // Apagamos la profundidad para que las letras se dibujen SOBRE el 3D

    // B. DIBUJAR TEXTO (Nuestra lógica de salto de línea)
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glPixelZoom(ESCALA, ESCALA);
    int cursor_x = 0;
    int cursor_y = ALTO_VENTANA - ALTO_LETRA;

    for (size_t i = 0; i < buffer_texto.length(); i++) {
        char letra = buffer_texto[i];
        if (cursor_x + ANCHO_LETRA > ANCHO_VENTANA) {
            cursor_x = 0;
            cursor_y -= ALTO_LETRA;
        }
        if (cursor_y < 0) {
            buffer_texto = "";
            cursor_x = 0;
            cursor_y = ALTO_VENTANA - ALTO_LETRA;
            break;
        }
        if (isalpha(letra)) {
            letra = toupper(letra);
            int indice = letra - 'A';
            glRasterPos2i(cursor_x, cursor_y);
            glBitmap(ANCHO_LETRA_BASE, ALTO_LETRA_BASE, 0.0, 0.0, ANCHO_LETRA, 0.0, mi_fuente[indice]);
            cursor_x += ANCHO_LETRA;
        }
        else if (letra == ' ') { cursor_x += ANCHO_LETRA; }
    }

    // C. RESTAURAR ESTADO 3D
    glEnable(GL_DEPTH_TEST); // Volvemos a encender la profundidad 3D

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Recuperamos el lente 3D

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix(); // Recuperamos la cámara 3D
}

// --- 4. FUNCIÓN PRINCIPAL DE DIBUJO ---
void display(void) {
    // Limpiamos los colores y el "buffer de profundidad" (Z-Buffer) para el 3D
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // DIBUJAMOS EL MUNDO 3D (Ejemplo: Una Tetera Rotando)
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,  // Posición de la Cámara 3D
              0.0, 0.0, 0.0,  // A dónde mira
              0.0, 1.0, 0.0); // Vector "Arriba"

    glPushMatrix();
        glRotatef(angulo_tetera, 1.0f, 1.0f, 0.0f); // Rotamos el mundo 3D
        glColor3f(1.0f, 0.5f, 0.0f); // Tetera Naranja
        glutWireTeapot(1.5);         // Dibujamos la tetera 3D
    glPopMatrix();

    // DIBUJAMOS LA INTERFAZ (HUD) ENCIMA DEL 3D
    renderizarHUD();

    glutSwapBuffers(); // Usamos SwapBuffers para evitar parpadeos en animación
}

// --- 5. ANIMACIÓN Y CONTROLES ---
void idle() {
    angulo_tetera += 0.5f; // Hacemos que la tetera gire en cada frame
    if (angulo_tetera > 360.0f) angulo_tetera -= 360.0f;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    else if (key == 8 || key == 127) { if (!buffer_texto.empty()) buffer_texto.pop_back(); }
    else if (key == 13) {
        int espacios_faltantes = (ANCHO_VENTANA - (buffer_texto.length() * ANCHO_LETRA) % ANCHO_VENTANA) / ANCHO_LETRA;
        buffer_texto.append(espacios_faltantes, ' ');
    }
    else {
        if (isalpha(key) || key == ' ') buffer_texto += key;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1; // Evitar división por cero
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // AHORA USAMOS PERSPECTIVA 3D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0); // Lente 3D real

    glMatrixMode(GL_MODELVIEW);

    ANCHO_VENTANA = w;
    ALTO_VENTANA = h;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // Cambiamos a DOUBLE buffer (animación fluida) y agregamos DEPTH para el 3D
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(ANCHO_VENTANA, ALTO_VENTANA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("UAM_Fase3_HUD_3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle); // <-- ¡Encendemos la animación 3D!

    glutMainLoop();
    return 0;
}