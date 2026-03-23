/* * ======================================================================================
 * AUDITORÍA TÉCNICA DE GRÁFICAS POR COMPUTADORA
 * ======================================================================================
 * ARCHIVO: main.cpp (Simulación de Vuelo, Sombreado Dinámico Translúcido y Picking)
 * ENTORNO: Linux (Ubuntu 24). VERSIÓN DEFINITIVA (MERGE).
 * * * MEJORAS DE ESTA VERSIÓN:
 * - Fusión de texturas procedurales con sombras translúcidas (Blending).
 * - Avión completo jerarquizado.
 * - Controles de luz dinámica en tiempo real (F5-F12) recalculando la matriz de sombras.
 * ======================================================================================
 */

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

using namespace std;

#define checkImageWidth 256
#define checkImageHeight 256
#define subImageWidth 64
#define subImageHeight 64

// Nombres de Objetos (Para Picking)
#define SHADOW        1
#define JET           2
#define NOSE          20
#define BODY          21
#define WING          22
#define TAIL          23
#define TAILVERTICAL  231
#define TAILHORIZONTAL 232

#define BUFSIZE 512
#define YGROUND -150.0f
#define EX 0.0f
#define EY 100.0f
#define EZ 250.0f
#define CX 0.0f
#define CY YGROUND
#define CZ 0.0f
#define DELTA_C 0.1f
#define DELTA 0.01f
#define DELTA_R 0.01f
#define DELTATRANS 0.05f
#define LIGHT_X 1.5f
#define LIGHT_Y 1.5f
#define LIGHT_Z 1.0f
#define LIGHT_A 1.0f
#define KTRANS 0.05f 

GLuint GroundList, Shadow_Model_List, Jet_Model_List, LightList;
GLuint selectBuf[BUFSIZE];

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte subImage[subImageHeight][subImageWidth][4];
static GLuint texName;

GLfloat norm[3], gnorm[3]; 

GLboolean bCull = true, bDepth = true, bOutline = true, bShade = true;

GLdouble pers[]={60.0, 1.0, 1.5, 3000.0};

GLfloat ex=EX, ey=EY, ez=EZ, cx=CX, cy=CY, cz=CZ, yground = YGROUND,
        deltaC=DELTA_C, delta= DELTA, deltaR= DELTA_R, deltatrans=DELTATRANS;

GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f }, diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f },
        specular[] = { 1.0f, 1.0f, 1.0f, 1.0f}, specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat STARTbackgroundcolor[] = {0.0f, 0.0f, 1.0f, 1.0f }, 
        STARTshadowcolor[]   = {0.1f, 0.1f, 0.1f, 0.6f}, // <-- ALFA 0.6 PARA TRANSLUCIDEZ
        STARTverttailcolor[] = {1.0f, 0.0f, 0.0f};

// shadowcolor AHORA ES DE 4 ELEMENTOS PARA SOPORTAR ALFA
GLfloat backgroundcolor[4], shadowcolor[4], verttailcolor[3];
GLfloat wlightPos[] = { LIGHT_X, LIGHT_Y, LIGHT_Z, LIGHT_A }, lightPos[4];

// Vértices del Suelo
GLfloat gdata[4][3] ={
    {400.0f, yground, -700.0f}, {-400.0f, yground, -700.0f}, 
    {-400.0f, yground, 200.0f}, {400.0f, yground, 200.0f}
};

// Vértices del Jet
GLfloat vdata[13][3] = {
    {0.0f, 0.0f, 25.0f}, {5.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, {-5.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -6.0f}, {-8.0f, 0.0f, 30.0}, {8.0f, 0.0f, 30.0}, {0.0f, 0.0f, 69.0}, 
    {0.0f, 0.0f, 90.0}, {0.0f, 24.0f, 90.0}, {-20.0f, 1.0f, 10.0}, { 20.0f, 1.0f, 10.0}, {0.0f, 1.0f, 5.0}
};

GLfloat trans[] = {0.0f, 0.0f, -1.0f}, ktrans = KTRANS, jetrot = 0.0f;
GLfloat shadowMat[16], vGroundPlane[4];

// --- FUNCIONES MATEMÁTICAS ---
void gltMakeShadowMatrix(GLfloat vPlaneEquation[], GLfloat vLightPos[], GLfloat destMat[]) {
    GLfloat dot = vPlaneEquation[0]*vLightPos[0] + vPlaneEquation[1]*vLightPos[1] +
                  vPlaneEquation[2]*vLightPos[2] + vPlaneEquation[3]*vLightPos[3];

    destMat[0] = dot  - vLightPos[0] * vPlaneEquation[0];
    destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
    destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
    destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];
    destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
    destMat[5] = dot  - vLightPos[1] * vPlaneEquation[1];
    destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
    destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];
    destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
    destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
    destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
    destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];
    destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
    destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
    destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
    destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}

void SetLightPos() {  
    lightPos[0]=wlightPos[0]; lightPos[1]=wlightPos[1]; 
    lightPos[2]=wlightPos[2]; lightPos[3]=wlightPos[3];
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    gltMakeShadowMatrix(vGroundPlane, lightPos, shadowMat);
}

void Buff_Light() {
    wlightPos[0]=lightPos[0]; wlightPos[1]=lightPos[1];
    wlightPos[2]=lightPos[2]; wlightPos[3]=lightPos[3];
}

void initColors() {
    for(int i=0; i<3; i++){
        backgroundcolor[i] = STARTbackgroundcolor[i];
        shadowcolor[i] = STARTshadowcolor[i];
        verttailcolor[i] = STARTverttailcolor[i];
    }
    backgroundcolor[3] = STARTbackgroundcolor[3];
    shadowcolor[3] = STARTshadowcolor[3]; // Preservar el Alfa
}

void makeCheckImages(void) {
   int i, j, c;
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;   checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;   checkImage[i][j][3] = (GLubyte) 255;
      }
   }
   for (i = 0; i < subImageHeight; i++) {
        for (j = 0; j < subImageWidth; j++) {
            c = ((((i&0x4)==0)^((j&0x4))==0))*255;
            subImage[i][j][0] = (GLubyte) c;    subImage[i][j][1] = (GLubyte) 0;
            subImage[i][j][2] = (GLubyte) 0;    subImage[i][j][3] = (GLubyte) 255;
        }
    }
}

void normalize(GLfloat v[3]) {
    GLfloat d = sqrtf(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
    if (d == 0.0) {exit(100); return;} 
    v[0] /= d; v[1] /= d; v[2] /= d;
}

void normcrossprod(GLfloat v1[3], GLfloat v2[3], GLfloat out[3]) {  
    out[0] = v1[1]*v2[2] - v1[2]*v2[1]; out[1] = v1[2]*v2[0] - v1[0]*v2[2];
    out[2] = v1[0]*v2[1] - v1[1]*v2[0]; normalize(out);
}

void gltGetNormalVector(GLfloat v0[3], GLfloat v1[3], GLfloat v2[3], GLfloat norm[3]) {  
    GLfloat d1[3], d2[3];
    for (int j = 0; j < 3; j++) {   
        d1[j] = v0[j] - v1[j];
        d2[j] = v1[j] - v2[j];
    }
    normcrossprod(d1, d2, norm);
}

// --- INICIALIZACIÓN DE ESTADOS ---
void Config_My_Texture() {
    makeCheckImages();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 48, 170, subImageWidth, subImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 48, 48, subImageWidth, subImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 170, 48, subImageWidth, subImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 170, 170, subImageWidth, subImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void Config_Ground_List(){
    GroundList = glGenLists(1);
    glNewList(GroundList, GL_COMPILE);
        glBegin(GL_QUADS);
            gltGetNormalVector(&gdata[0][0], &gdata[1][0], &gdata[2][0], gnorm);
            glNormal3fv(gnorm);
            glColor3f(0.5f, 0.5f, 0.5f);
            glTexCoord2f(0.0, 0.0); glVertex3fv(&gdata[0][0]); 
            glTexCoord2f(0.0, 1.0); glVertex3fv(&gdata[1][0]);
            glTexCoord2f(1.0, 1.0); glVertex3fv(&gdata[2][0]); 
            glTexCoord2f(1.0, 0.0); glVertex3fv(&gdata[3][0]); 
        glEnd();
    glEndList();
}

void Config_Shadow_Model_List(){
    Shadow_Model_List = glGenLists(1);
    glNewList(Shadow_Model_List, GL_COMPILE);
        // Cuerpo y Nariz
        glBegin(GL_TRIANGLE_FAN);
            glVertex3fv(&vdata[0][0]); glVertex3fv(&vdata[1][0]); glVertex3fv(&vdata[2][0]);
            glVertex3fv(&vdata[3][0]); glVertex3fv(&vdata[4][0]); glVertex3fv(&vdata[1][0]);
        glEnd();
        // Alas
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[10][0]); glVertex3fv(&vdata[11][0]); glVertex3fv(&vdata[12][0]);
        glEnd();
        // Cola Horizontal
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[5][0]); glVertex3fv(&vdata[6][0]); glVertex3fv(&vdata[7][0]);
        glEnd();
        // Cola Vertical
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[8][0]); glVertex3fv(&vdata[9][0]); glVertex3fv(&vdata[7][0]);
        glEnd();
    glEndList();
}

void Config_Light_List(){
    LightList = glGenLists(1);
    glNewList(LightList, GL_COMPILE);
        glColor3ub(255,255,0);
        glutSolidSphere(5.0f,10,10);
    glEndList();
}

void SetupRC() {   
    initColors();
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    vGroundPlane[0] = 0.0f; vGroundPlane[1] = 1.0f; 
    vGroundPlane[2] = 0.0f; vGroundPlane[3] = -yground;
  
    Config_My_Texture();
    Config_Ground_List();
    Config_Shadow_Model_List();
    Config_Light_List();

    glFrontFace(GL_CCW);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_BACK,GL_FILL);

    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glEnable(GL_LIGHT0);

    SetLightPos();  
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
    glMateriali(GL_FRONT,GL_SHININESS,128);
    glClearColor(backgroundcolor[0], backgroundcolor[1], backgroundcolor[2], backgroundcolor[3]);
}

// --- RENDERIZADO Y LÓGICA ---
void DrawGround(void) { 
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);
   glCallList(GroundList);
   glDisable(GL_TEXTURE_2D);
}

void MoveJet_or_Shadow(int obj) {   
    GLfloat wtrans[3];
    wtrans[0] = ktrans * trans[0];  
    wtrans[1] = ktrans * trans[1];
    wtrans[2] = ktrans * trans[2];
    
    if(obj == SHADOW) glMultMatrixf((GLfloat *)shadowMat);
    
    glTranslatef(wtrans[0], wtrans[1], wtrans[2]);
    glRotatef(jetrot, 0.0f, 1.0f, 0.0f);
}

void Jet_or_ShadowModel(GLenum mode, int obj) {
    if (obj == SHADOW) {    
        if (mode == GL_SELECT) glLoadName(SHADOW);
        glCallList(Shadow_Model_List);
    }
    if (obj == JET) {   
        if (mode == GL_SELECT) glLoadName(JET);

        // 1. Nariz y Cuerpo
        if (mode == GL_SELECT) glPushName(NOSE);
        glColor3f(0.7f, 0.7f, 0.7f); // Gris metálico
        glBegin(GL_TRIANGLE_FAN);
            glVertex3fv(&vdata[0][0]); glVertex3fv(&vdata[1][0]); glVertex3fv(&vdata[2][0]);
            glVertex3fv(&vdata[3][0]); glVertex3fv(&vdata[4][0]); glVertex3fv(&vdata[1][0]);
        glEnd();
        if (mode == GL_SELECT) glPopName();

        // 2. Alas
        if (mode == GL_SELECT) glPushName(WING);
        glColor3f(0.4f, 0.4f, 0.4f); // Gris oscuro
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[10][0]); glVertex3fv(&vdata[11][0]); glVertex3fv(&vdata[12][0]);
        glEnd();
        if (mode == GL_SELECT) glPopName();

        // 3. Cola Horizontal
        if (mode == GL_SELECT) glPushName(TAILHORIZONTAL);
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[5][0]); glVertex3fv(&vdata[6][0]); glVertex3fv(&vdata[7][0]);
        glEnd();
        if (mode == GL_SELECT) glPopName();

        // 4. Cola Vertical
        if (mode == GL_SELECT) glPushName(TAILVERTICAL);
        gltGetNormalVector(&vdata[8][0], &vdata[9][0], &vdata[7][0], norm);
        glNormal3fv(norm);
        glColor3f(verttailcolor[0], verttailcolor[1], verttailcolor[2]); 
        glBegin(GL_TRIANGLES);
            glVertex3fv(&vdata[8][0]); glVertex3fv(&vdata[9][0]); glVertex3fv(&vdata[7][0]);
        glEnd();
        if (mode == GL_SELECT) glPopName(); 
    }
}

void DrawLight() {   
    glPushMatrix();
        glTranslatef(lightPos[0],lightPos[1], lightPos[2]);
        glCallList(LightList);
    glPopMatrix();
}

void RenderScene(GLenum mode) {   
    if(mode == GL_RENDER) {   
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    }
   
    gluPerspective(pers[0], pers[1], pers[2], pers[3]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (mode == GL_RENDER) DrawGround();
  
    // Sombras (Con Blending Translúcido Integrado)
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Activa la magia del Alfa
    glPushMatrix(); 
        MoveJet_or_Shadow(SHADOW); 
        glColor4fv(shadowcolor); // Usamos 4 colores ahora
        Jet_or_ShadowModel(mode, SHADOW);
    glPopMatrix(); 
    glDisable(GL_BLEND);

    // Jet
    if (bDepth) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_LIGHTING); 
    glPushMatrix(); 
        MoveJet_or_Shadow(JET); 
        Jet_or_ShadowModel(mode, JET);
    glPopMatrix();  

    DrawLight();
   
    if (mode == GL_RENDER) glutSwapBuffers();
}

// --- LÓGICA DE SELECCIÓN (PICKING) ---
void anallizename(int name) {   
    int i;
    if (name == SHADOW) {
        for(i=0; i<3; i++) shadowcolor[i]+= 0.25f*(1.0f-shadowcolor[i]); // Solo cambia RGB
    } else if (name == JET || name == TAIL || name == TAILVERTICAL) {
        for(i=0; i<3; i++) verttailcolor[i]+= 0.25f*(1.0f-verttailcolor[i]);
    }
    glutPostRedisplay();
}

void processHits(GLint hits, GLuint buffer[]) {  
    if (hits == 0) { initColors(); return; }

    unsigned int i, j;
    GLuint names, *ptr;
    ptr = (GLuint *) buffer;
    
    for (i = 0; i < hits; i++) { 
        names = *ptr;
        ptr += 3;
        for (j = 0; j < names; j++) anallizename(*ptr++);
    }
}

void pickPlaneDetails(int button, int state, int x, int y) {
    GLint hits;
    GLint viewport[4];

    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(BUFSIZE, selectBuf);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);
    RenderScene(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);
    processHits(hits, selectBuf);
    glutPostRedisplay();
} 

void ChangeSize(GLsizei w, GLsizei h) {    
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    pers[1] = (GLdouble)w / (GLdouble)h;
}

// --- CONTROLES DE TECLADO ---
void SpecialKeys(int key, int x, int y) {
    GLfloat dx, dy, dz, dx0, dy0, dz0, s, s1;
    
    // Zoom y Navegación
    if(key == GLUT_KEY_UP) { dx=cx-ex; dy=cy-ey; dz=cz-ez; ex-=deltaR*dx; ey-=deltaR*dy; ez-=deltaR*dz; glutPostRedisplay(); return; }
    if(key == GLUT_KEY_DOWN) { dx=cx-ex; dy=cy-ey; dz=cz-ez; ex+=deltaR*dx; ey+=deltaR*dy; ez+=deltaR*dz; glutPostRedisplay(); return; }
    if(key == GLUT_KEY_LEFT) { 
        dx0=cz-ez; dz0= ex - cx; dx=cx-ex; dz=cz-ez;
        s=sqrtf(dx*dx+dz*dz); dx+=delta*dx0; dz+=delta*dz0;
        s1=sqrtf(dx*dx+dz*dz)/s; dx/=s1; dz/=s1; ex=cx-dx; ez=cz-dz;
        glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_RIGHT) {
        dx0=cz-ez; dz0= ex - cx; dx=cx-ex; dz=cz-ez;
        s=sqrtf(dx*dx+dz*dz); dx-=delta*dx0; dz-=delta*dz0;
        s1=sqrtf(dx*dx+dz*dz)/s; dx/=s1; dz/=s1; ex=cx-dx; ez=cz-dz;
        glutPostRedisplay(); return;
    }

    // --- NUEVO: CONTROLES DE LUZ Y MATRIZ DINÁMICA ---
    if(key == GLUT_KEY_F5) { // Rotar cámara
        dy0=cz-ez; dz0= ey - cy; dy=cy-ey; dz=cz-ez;
        s=sqrtf(dy*dy+dz*dz); dy+=delta*dy0; dz+=delta*dz0;
        s1=sqrtf(dy*dy+dz*dz)/s; dy/=s1; dz/=s1; ey=cy-dy; ez=cz-dz;
        glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F6) { 
        dy0=cz-ez; dz0= ey - cy; dy=cy-ey; dz=cz-ez;
        s=sqrtf(dy*dy+dz*dz); dy-=delta*dy0; dz-=delta*dz0;
        s1=sqrtf(dy*dy+dz*dz)/s; dy/=s1; dz/=s1; ey=cy-dy; ez=cz-dz;
        glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F7) { // Mover luz
        Buff_Light(); dx=cx-wlightPos[0]; dy=cy-wlightPos[1]; dz=cz-wlightPos[2];
        dx*=(1.0-deltatrans); dy*=(1.0-deltatrans); dz*=(1.0-deltatrans);
        wlightPos[0]=cx-dx; wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
        SetLightPos(); glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F8) { 
        Buff_Light(); dx=cx-wlightPos[0]; dy=cy-wlightPos[1]; dz=cz-wlightPos[2];
        dx/=(1.0-deltatrans); dy/=(1.0-deltatrans); dz/=(1.0-deltatrans);
        wlightPos[0]=cx-dx; wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
        SetLightPos(); glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F9) { // Rotar luz vertical (+)
        Buff_Light(); dy0=cz-wlightPos[2]; dz0= wlightPos[1] - cy;  
        dy=cy-wlightPos[1]; dz=cz-wlightPos[2];
        s=sqrtf(dy*dy+dz*dz); dy+=delta*dy0; dz+=delta*dz0;
        s1=sqrtf(dy*dy+dz*dz)/s; dy/=s1; dz/=s1; wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
        SetLightPos(); glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F10) { // Rotar luz vertical (-)
        Buff_Light(); dy0=cz-wlightPos[2]; dz0= wlightPos[1] - cy;  
        dy=cy-wlightPos[1]; dz=cz-wlightPos[2];
        s=sqrtf(dy*dy+dz*dz); dy-=delta*dy0; dz-=delta*dz0;
        s1=sqrtf(dy*dy+dz*dz)/s; dy/=s1; dz/=s1; wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
        SetLightPos(); glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F11) { // Rotar luz horizontal
        Buff_Light(); dy=-wlightPos[2]; dz= wlightPos[0];
        s=sqrtf(wlightPos[0]*wlightPos[0]+wlightPos[1]*wlightPos[1]+wlightPos[2]*wlightPos[2]);
        wlightPos[0]+=delta*dy; wlightPos[2]+=delta*dz;
        s1=sqrtf(wlightPos[0]*wlightPos[0]+wlightPos[1]*wlightPos[1]+wlightPos[2]*wlightPos[2])/s;
        wlightPos[0]/=s1; wlightPos[1]/=s1; wlightPos[2]/=s1;
        SetLightPos(); glutPostRedisplay(); return;
    }
    if(key == GLUT_KEY_F12) { // Mover luz horizontal
        Buff_Light(); dy=wlightPos[0]; dz= wlightPos[2];
        wlightPos[0]+=delta*dy; wlightPos[2]+=delta*dz;
        SetLightPos(); glutPostRedisplay(); return;
    }

    // Controles de Estados Clásicos
    if(key == GLUT_KEY_F1) { bCull=!bCull; if(bCull) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE); glutPostRedisplay(); return; }
    if(key == GLUT_KEY_F2) { bDepth=!bDepth; if(bDepth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST); glutPostRedisplay(); return; }
    if(key == GLUT_KEY_F3) { bOutline=!bOutline; if(bOutline) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glutPostRedisplay(); return; }
    if(key == GLUT_KEY_F4) { bShade=!bShade; if(bShade) glShadeModel(GL_FLAT); else glShadeModel(GL_SMOOTH); glutPostRedisplay(); return; }
}

void display(void) {
   RenderScene(GL_RENDER);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600); // Ventana un poco más grande
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Motor Definitivo UAM: Texturas, Sombras Alfa y Picking");
    
    glutDisplayFunc(display);
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutMouseFunc(pickPlaneDetails);
    
    SetupRC();
    glutMainLoop();
    return 0;
}
