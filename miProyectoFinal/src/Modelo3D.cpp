#include "Modelo3D.h"
#include <stdio.h>

// Promesas de funciones que viven temporalmente en main.cpp
extern std::string aMinusculas(const std::string& str);
extern std::string ExtraerNombre(const std::string& path);
extern GLuint CargarTexturaConAlpha(std::string filename, bool esHoja);

// Cumplimiento del Constructor
Modelo3D::Modelo3D() {
    // El vector 'mallas' se inicializa vacío automáticamente
}

// Cumplimiento del Destructor (¡POO Profesional para evitar Memory Leaks!)
Modelo3D::~Modelo3D() {
    for (const auto& malla : mallas) {
        // Liberamos las listas de dibujo de OpenGL
        glDeleteLists(malla.displayList, 1);
        // Liberamos las texturas de la tarjeta de video
        if (malla.texID != 0) {
            glDeleteTextures(1, &malla.texID);
        }
    }
    mallas.clear();
}

void Modelo3D::cargar(const char* path) {
    Assimp::Importer imp;
    const aiScene* sc = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    if(!sc) return;

    for(unsigned int i = 0; i < sc->mNumMeshes; i++) {
        aiMesh* mesh = sc->mMeshes[i];
        SubMalla sm;
        sm.texID = 0;
        sm.esHoja = false;
        
        if (sc->HasMaterials()) {
            aiMaterial* mat = sc->mMaterials[mesh->mMaterialIndex];
            if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString texPath;
                mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
                std::string fname = ExtraerNombre(texPath.C_Str());
                std::string fnameMin = aMinusculas(fname);
                
                sm.esHoja = (fnameMin.find("blatt") != std::string::npos || 
                             fnameMin.find("leave") != std::string::npos ||
                             fnameMin.find("branch") != std::string::npos);
                
                sm.texID = CargarTexturaConAlpha(fname, sm.esHoja);
            }
        }

        if(sm.texID == 0) {
            if (i % 2 == 0) {
                sm.esHoja = false;
                sm.texID = CargarTexturaConAlpha("Trunk_diffcol.bmp", false);
                if (sm.texID == 0) sm.texID = CargarTexturaConAlpha("bark_loo.jpg", false);
            } else {
                sm.esHoja = true;
                sm.texID = CargarTexturaConAlpha("Leaves_diffcol.bmp", true);
                if (sm.texID == 0) sm.texID = CargarTexturaConAlpha("blatt1.jpg", true);
            }
        }

        sm.displayList = glGenLists(1);
        glNewList(sm.displayList, GL_COMPILE); 
        glBegin(GL_TRIANGLES); 
        for(unsigned int j = 0; j < mesh->mNumFaces; j++) {
            bool esBaseFea = false;
            for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++) {
                unsigned int idx = mesh->mFaces[j].mIndices[k];
                float vx = mesh->mVertices[idx].x; float vy = mesh->mVertices[idx].y; float vz = mesh->mVertices[idx].z;
                if (vy < 0.5f && (vx*vx + vz*vz) > 2.0f) { esBaseFea = true; break; }
            }
            if (esBaseFea) continue; 

            for(unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++) {
                unsigned int idx = mesh->mFaces[j].mIndices[k];
                if(mesh->HasTextureCoords(0)) glTexCoord2f(mesh->mTextureCoords[0][idx].x, mesh->mTextureCoords[0][idx].y);
                if(mesh->HasNormals()) glNormal3f(mesh->mNormals[idx].x, mesh->mNormals[idx].y, mesh->mNormals[idx].z);
                glVertex3f(mesh->mVertices[idx].x, mesh->mVertices[idx].y, mesh->mVertices[idx].z);
            }
        }
        glEnd(); 
        glEndList();
        mallas.push_back(sm);
    }
}

void Modelo3D::dibujarUnArbol(bool esSombra) {
    for (const auto& malla : mallas) {
        if (malla.texID != 0) {
            glBindTexture(GL_TEXTURE_2D, malla.texID);
            if (esSombra) glColor4f(0.0f, 0.0f, 0.0f, 0.35f); 
            else glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
            if (esSombra) glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
            else {
                if (malla.esHoja) glColor4f(0.3f, 0.8f, 0.3f, 1.0f);
                else glColor4f(0.5f, 0.3f, 0.1f, 1.0f); 
            }
        }
        glCallList(malla.displayList);
    }
}

void Modelo3D::dibujar(bool esSombra) {
    if (mallas.empty()) return;

    glEnable(GL_TEXTURE_2D); 
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f); 
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float posiciones[5][3] = {
        {-12.0f, 0.0f,  0.0f}, { 12.0f, 0.0f,  4.0f}, 
        {  0.0f, 0.0f, -10.0f}, { -8.0f, 0.0f,  -8.0f}, {  8.0f, 0.0f,   8.0f}
    };

    for(int i=0; i<5; i++) {
        glPushMatrix();
        glTranslatef(posiciones[i][0], posiciones[i][1], posiciones[i][2]);
        dibujarUnArbol(esSombra);
        glPopMatrix();
    }
    
    glDisable(GL_ALPHA_TEST); 
    glDisable(GL_TEXTURE_2D); 
}
