/*

=========================================================================================
[EXPEDIENTE TÉCNICO: SISTEMA DE SIMULACIÓN DE VUELO Y SOMBREADO DINÁMICO]
DIRECTOR DEL PROYECTO: Gema (1) - Asesor Gráficas Por Computadora
=========================================================================================

DESCRIPCIÓN GENERAL:
Este archivo constituye el núcleo del motor de renderizado para un modelo articulado (Jet).
Gestiona el ciclo de vida de OpenGL mediante GLUT, integrando iluminación avanzada,
texturizado procedural y un sistema de proyección de sombras sobre un plano.
TÉCNICAS DE GRAFICACIÓN IDENTIFICADAS (Lista de Cotejo):
[X] Modelos articulados: Definición de jerarquías (NOSE, BODY, WING, TAIL) mediante
nombres para el sistema de selección.
[X] Cámara controlable: Implementación de navegación en coordenadas esféricas
(SpecialKeys) utilizando gluLookAt.
[X] Vectores normales: Cálculo de normales por cara mediante producto cruz en
'gltGetNormalVector', esencial para el modelo de iluminación.
[X] Iluminación: Configuración de GL_LIGHT0 con componentes Ambiental, Difusa y
Especular (SetupRC).
[X] Materiales: Uso de reflectividad especular (glMaterialfv) y brillo (shininess).
[X] Sombras (Plano): Implementación de una matriz de proyección de sombras
'gltMakeShadowMatrix' que aplana la geometría sobre el plano del suelo.
[X] Controles: Toggle dinámico de Culling, Depth Test, Shading (Smooth/Flat) y
Polygon Mode (F1-F4).
[X] Picking: Sistema de selección por mouse usando GL_SELECT y la pila de nombres
(glPushName/glPopName) para identificar partes del Jet.
[X] Texturas: Generación de texturas procedurales (Checkerboard) y uso de
glTexSubImage2D para actualizaciones parciales.
SUSTENTO BIBLIOGRÁFICO:
Gráficos: Según el "Red Book" (Cap. 13), el sistema de 'Picking' requiere una matriz
de proyección específica (gluPickMatrix) para restringir el renderizado al área del clic.
Matemáticas: La matriz de sombras se basa en la proyección de un punto sobre un plano
desde una fuente de luz puntual, utilizando álgebra de matrices (Howard Anton, Cap. 3).
Normales: El cálculo del vector normal unitario sigue la ley del producto vectorial
definida en Thomas (Cálculo de Varias Variables, Cap. 12).
ANALOGÍA PARA EL DESARROLLADOR JAVA:
En Java (ej. Swing/JavaFX), el estado es un objeto que encapsulas. En este código de C++,
el "Estado de OpenGL" es una máquina de estados global. Cuando activas glEnable(GL_LIGHTING),
es como si cambiaras una propiedad estática global que afecta a todas las llamadas
de dibujo subsecuentes hasta que la apagues.
NOTA DE SEGURIDAD OPERATIVA (OS CHECK):
El código incluye "stdafx.h" (específico de Visual Studio/Windows). Si se migra a Linux
(Mint/Ubuntu), este encabezado debe eliminarse o condicionarse bajo #ifdef _WIN32.
=========================================================================================
*/

#include "stdafx.h"

#define	checkImageWidth 256
#define	checkImageHeight 256
#define subImageWidth 64
#define subImageHeight 64

// Define object names

#define SHADOW        1
#define JET           2
#define NOSE			20
#define BODY			21
#define WING			22
#define TAIL			23
#define TAILVERTICAL		231
#define TAILHORIZONTAL		232

#define BUFSIZE 512

#define StackMax         12
#define YGROUND         -150.0f
#define EX				0.0f
#define EY				1.0f
#define EZ				3.0f
#define CX				0.0f
#define CY				YGROUND
#define CZ				0.0f
#define DELTA_C			0.1f
#define DELTA			0.01f
#define DELTA_R			0.01f
#define DELTATRANS		0.05f
#define LIGHT_X			1.5f
#define LIGHT_Y			1.5f
#define LIGHT_Z			1.0f
#define LIGHT_A			1.0f
#define KTRANS          0.05f 


GLuint GroundList, Shadow_Model_List, Jet_Model_List, LightList,
Jet_Model_List1, Jet_Model_List2, Jet_Model_List3, Jet_Model_List4;
GLuint selectBuf[BUFSIZE];

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte otherImage[checkImageHeight][checkImageWidth][4];

static GLubyte subImage[subImageHeight][subImageWidth][4];

static GLuint texName;

GLfloat norm[3], 
gnorm[3];  /*gnorm comes in a list*/

GLboolean	bCull = (GLboolean)true, bDepth= (GLboolean)true,	bOutline=(GLboolean)true,
            bShade= (GLboolean)true;

GLdouble pers[]={60.0, 1.0, 1.5, 700.0};

GLfloat ex=EX, ey=EY,  ez=EZ, 		cx=CX,  cy=CY, cz=CZ, yground = YGROUND,
         deltaC=DELTA_C, delta= DELTA, 	 deltaR= DELTA_R,  deltatrans=DELTATRANS,
        /*ex=0.0f, ey=30.0f, ez=-30.0f,    delta= 0.01f, deltaR= 0.01f,*/

         ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f },	 diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f },
         specular[] = { 1.0f, 1.0f, 1.0f, 1.0f},		 specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f },

STARTbackgroundcolor[] = {0.0f, 0.0f, 1.0f, 1.0f }, 
STARTshadowcolor[]	  = {0.3f, 0.3f, 0.3f},         STARTverttailcolor[]   = {1.0f, 0.0f, 0.0f},

backgroundcolor[4],   shadowcolor[3], verttailcolor[3],

		wlightPos[] = {  LIGHT_X,  LIGHT_Y, LIGHT_Z,  LIGHT_A }, 
		lightPos[4],   // = { -75.0f, 150.0f, -50.0f, 1.0f }, //{ -30.f, 4.0f, 15.0f, 1.0f };

//ground vertices
		gdata[4][3] ={
				{400.0f, yground, -700.0f}, {-400.0f, yground, -700.0f}, 
				{-400.0f, yground, 200.0f}, {400.0f, yground, 200.0f}
			},
//jet vertices
         vdata[13][3] = {
			{0.0f, 0.0f, 25.0f},   //Pinnacle of cone is shared vertex for fan, moved up z-axis
			{5.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, {-5.0f, 0.0f, 0.0f}, //to close the fan use {5.0f, 0.0f, 0.0f} twice
			{0.0f, 0.0f, -6.0f},  //Pinnacle of new cone with the same base verteces as above
			// horiz tail
			{-8.0f, 0.0f, 30.0}, {8.0f, 0.0f, 30.0}, {0.0f, 0.0f, 69.0}, 
			//vertical tail
			{0.0f, 0.0f, 90.0},    // far point 
			{0.0f, 24.0f, 90.0},    // up point 
			// {0.0f, 0.0f, 23.0}  // the close point is the same 
			                       // as for horiz. tail
			//wing
			{-20.0f, 1.0f, 10.0},  { 20.0f, 1.0f, 10.0}, {0.0f,  1.0f, 5.0}
			},

        trans[] = {0.0f, 0.0f, -1.0f},   ktrans = KTRANS,   
        jetrot    = 0.0f, deljetrot = 0.1f,

// Transformation matrix to project shadow
        shadowMat[16],   LightPosStack[16], fLightPosMirror[4],
		vGroundPlane[4]; // = {0.0f, 1.0f, 0.0f, -yground};  //= {0.0f, 1.0f, 0.0f, 150.0f};

GLsizei wSize=500, hSize =500;

void gltMakeShadowMatrix(GLfloat vPlaneEquation[], GLfloat vLightPos[], GLfloat destMat[])
    {
    GLfloat dot;

	// Dot product of plane and light position
    dot =   vPlaneEquation[0]*vLightPos[0] + vPlaneEquation[1]*vLightPos[1] +
            vPlaneEquation[2]*vLightPos[2] + vPlaneEquation[3]*vLightPos[3];

    // Now do the projection
    // First column
    destMat[0] = dot  - vLightPos[0] * vPlaneEquation[0];
    destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
    destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
    destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];

    // Second column
    destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
    destMat[5] = dot  - vLightPos[1] * vPlaneEquation[1];
    destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
    destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];

    // Third Column
    destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
    destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
    destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
    destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];

    // Fourth Column
    destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
    destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
    destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
    destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
    }

void SetLightPos()
	{  lightPos[0]=wlightPos[0];  lightPos[1]=wlightPos[1]; 
	   lightPos[2]=wlightPos[2];  lightPos[3]=wlightPos[3];
	   glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	   gltMakeShadowMatrix(vGroundPlane, lightPos, shadowMat);
	}


void Buff_Light()
	{wlightPos[0]=lightPos[0];
	 wlightPos[1]=lightPos[1];
	 wlightPos[2]=lightPos[2];
	 wlightPos[3]=lightPos[3];
	}


void initColors()
{
	for(int i=0; i<3; i++){
		backgroundcolor[i]=STARTbackgroundcolor[i];
		
		shadowcolor[i]     =STARTshadowcolor[i];
		verttailcolor[i]   =STARTverttailcolor[i];
	}
	backgroundcolor[3]=STARTbackgroundcolor[3];
}

void makeCheckImages(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;   checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;   checkImage[i][j][3] = (GLubyte) 255;
         
		 c = ((((i&0x10)==0)^((j&0x10))==0))*255;
         otherImage[i][j][0] = (GLubyte) 0;   otherImage[i][j][1] = (GLubyte) 0;
         otherImage[i][j][2] = (GLubyte) c;   otherImage[i][j][3] = (GLubyte) 255;
      }
   }

   for (i = 0; i < subImageHeight; i++) {
		for (j = 0; j < subImageWidth; j++) {
		c = ((((i&0x4)==0)^((j&0x4))==0))*255;
		subImage[i][j][0] = (GLubyte) c;	subImage[i][j][1] = (GLubyte) 0;
		subImage[i][j][2] = (GLubyte) 0;	subImage[i][j][3] = (GLubyte) 255;
		}
	}
}


void normalize(GLfloat v[3]) {
GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
if (d == 0.0) {exit(100); return;} // wrong input exit
v[0] /= d; v[1] /= d; v[2] /= d;
}
void normcrossprod(GLfloat v1[3], GLfloat v2[3], GLfloat out[3])
{	out[0] = v1[1]*v2[2] - v1[2]*v2[1]; out[1] = v1[2]*v2[0] - v1[0]*v2[2];
	out[2] = v1[0]*v2[1] - v1[1]*v2[0];	normalize(out);
}

void gltGetNormalVector( GLfloat v0[3], GLfloat //build normal 'norm'
	 v1[3], GLfloat v2[3], GLfloat norm[3])	// by the triangle vrtices
	{	GLfloat d1[3], d2[3];
		for (int j = 0; j < 3; j++) {	d1[j] = v0[j] - v1[j];
										d2[j] = v1[j] - v2[j];
			}
		normcrossprod(d1, d2, norm);
     }

void Config_My_Texture()
	{
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

   glTexSubImage2D(GL_TEXTURE_2D, 0, 48, 170, subImageWidth, 
	   subImageHeight, GL_RGBA,	GL_UNSIGNED_BYTE, subImage);
   
   glTexSubImage2D(GL_TEXTURE_2D, 0, 48, 48, subImageWidth, 
	   subImageHeight, GL_RGBA,	GL_UNSIGNED_BYTE, subImage);

   glTexSubImage2D(GL_TEXTURE_2D, 0,  170, 48, subImageWidth, 
	   subImageHeight, GL_RGBA,	GL_UNSIGNED_BYTE, subImage);
   
  glTexSubImage2D(GL_TEXTURE_2D, 0,  170, 170, subImageWidth, 
	   subImageHeight, GL_RGBA,	GL_UNSIGNED_BYTE, subImage);

  
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
    return;
	}

void Config_Ground_List(){
 GroundList = glGenLists(1);
	glNewList (GroundList, GL_COMPILE);
		glBegin(GL_QUADS);

			gltGetNormalVector( &gdata[0][0], &gdata[1][0], &gdata[2][0], gnorm);
							  glNormal3fv(gnorm);
			glColor3f(0.5f, 0.5f, 0.5f);
			glTexCoord2f(0.0, 0.0);  glVertex3fv(& gdata[0][0] /*400.0f, -150.0f, -700.0f*/); 
			glTexCoord2f(0.0, 1.0);  glVertex3fv(& gdata[1][0] /*-400.0f, -150.0f, -700.0f*/);

			glTexCoord2f(1.0, 1.0);   glVertex3fv(& gdata[2][0]/*-400.0f, -150.0f, 200.0f*/); 
			glTexCoord2f(1.0, 0.0);   glVertex3fv(& gdata[3][0] /*400.0f, -150.0f, 200.0f*/); 

		glEnd();
	glEndList (); //end GroundList
}

void Config_Shadow_Model_List(){
	Shadow_Model_List=glGenLists(1);
	glNewList (Shadow_Model_List, GL_COMPILE);
	
		glBegin(GL_TRIANGLES);
	
				// vertic tail
			glVertex3fv( & vdata[8][0] /*{0.0f, 0.0f, 30.0}*/);            // V0
			glVertex3fv( & vdata[9][0] /*{0.0f, 8.0f, 30.0}*/);        // V1
			glVertex3fv( & vdata[7][0] /*0.0f, 0.0f, 23.0*/);        // V2

		glEnd();
	glEndList (); //end Shadow_Model_List
}

void Config_Light_List(){
	
	LightList = glGenLists(1);
		glNewList (LightList, GL_COMPILE);
				
			glColor3ub(255,255,0);
			glutSolidSphere(5.0f,10,10);
			 
	glEndList (); //end LightList
}

void SetupRC()
    {   
	initColors();
    // Set color shading model to flat
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	vGroundPlane[0] = 0.0f, vGroundPlane[1] = 1.0f, 
	vGroundPlane[2] = 0.0f, vGroundPlane[3] = -yground;
  
	Config_My_Texture();  //texture for ground
	
	Config_Ground_List(); /* make GroundList display list */

	Config_Shadow_Model_List();/* make Shadow_Model_List display list */
	Config_Light_List(); //make LightList

    // Clockwise-wound polygons are front facing; this is reversed
    // because we are using triangle fans
    glFrontFace(GL_CCW);

	// Enable lighting
    glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK,GL_FILL);

		// the light source GL_LIGHT0 is enabled:
	
	// Set up and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);

	SetLightPos();	//  wlightPos --> lightPos;
	                        // + glLightfv(GL_LIGHT0,GL_POSITION,lightPos); 
							//+ Calculate projection matrix to draw shadow on the ground:
                             // gltMakeShadowMatrix(vGroundPlane, lightPos, shadowMat);
	
	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	// Front material ambient and diffuse colors track glColor
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	 // Light blue background
    glClearColor( backgroundcolor[0], backgroundcolor[1], 
					backgroundcolor[2], backgroundcolor[3] );

	
    }

void DrawGround(void)
{	
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);

   glCallList(GroundList);
  glutSwapBuffers();
   glDisable(GL_TEXTURE_2D);
}

void MoveJet_or_Shadow(int obj)
{	GLfloat  wtrans[3];
		// move jet or its shadow to a new position
		
	wtrans[0]= ktrans* trans[0];	wtrans[1]= ktrans* trans[1];
	wtrans[2]= ktrans* trans[2];
	if(obj==SHADOW)	// Multiply by shadow projection matrix
		glMultMatrixf((GLfloat *)shadowMat);
	
	glTranslatef(wtrans[0], wtrans[1], wtrans[2]);
	glRotatef(jetrot, 0.0f, 1.0f, 0.0f);
}

void Jet_or_ShadowModel(GLenum mode, int obj)
{
	if (obj==SHADOW)
	{	if (mode == GL_SELECT)  glLoadName (SHADOW);

		glCallList(Shadow_Model_List);
	};
	if (obj==JET)
	{	//GLfloat norm[3];
		if (mode == GL_SELECT)	glLoadName (JET);

		if (mode == GL_SELECT)	glPushName (TAIL);
				if (mode == GL_SELECT)   glPushName (TAILVERTICAL);
				//glCallList(Jet_Model_List3);
				glBegin(GL_TRIANGLES);
					gltGetNormalVector( &vdata[8][0], &vdata[9][0], &vdata[7][0], norm);
							  glNormal3fv(norm);
					glColor3f(verttailcolor[0], verttailcolor[1], verttailcolor[2]);
					glVertex3fv( & vdata[8][0] /*{0.0f, 0.0f, 30.0}*/);            // V0
					glVertex3fv( & vdata[9][0] /*{0.0f, 8.0f, 30.0}*/);        // V1
					glVertex3fv( & vdata[7][0] /*0.0f, 0.0f, 23.0*/);        // V2
				glEnd();
				if (mode == GL_SELECT)   glPopName (); //TAILVERTICAL

		if (mode == GL_SELECT)    glPopName ();   //TAIL

	}
}

void DrawLight()
{    
	glPushMatrix();
		glTranslatef(lightPos[0],lightPos[1], lightPos[2]);
		glCallList(LightList);
    glPopMatrix();

}

void RenderScene(GLenum mode)
    {   if(mode==GL_RENDER)
		{	glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
		}
	    
		gluPerspective(pers[0], pers[1], pers[2], pers[3]);
		glMatrixMode (GL_MODELVIEW);
			// Reset coordinate system
		glLoadIdentity();

		gluLookAt (ex, ey, ez, cx, cy, cz, 0.0, 1.0, 0.0);
 		
		// Clear the window and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (mode == GL_RENDER)	
			DrawGround();
	  
		// disable lighting and save the projection state
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
	
		glPushMatrix();   /* apply shadow matrix multiplication 
						  to moved Jet shadow model*/
			MoveJet_or_Shadow(SHADOW); //MoveJetShadow(); 
			glColor3f(shadowcolor[0], shadowcolor[1], shadowcolor[2]);
		
			Jet_or_ShadowModel(mode, SHADOW);
	  
		glPopMatrix();  /* end drawing jet shadow */

    // restore lighting and save the projection state
		glEnable(GL_DEPTH_TEST);	    
		glEnable(GL_LIGHTING); 
	     
		glPushMatrix(); /* draw moved Jet model*/
		MoveJet_or_Shadow(JET);//MoveJet();	
		
		Jet_or_ShadowModel(mode, JET);
    glPopMatrix(); 	 /* end jet drawing */

    // Draw the light source
	DrawLight();
    
    // Flush drawing commands
	if (mode == GL_RENDER)
     glutSwapBuffers();
    }

void anallizename( int name)
{	int i;
	if (name == SHADOW){
     for( i=0; i<3; i++)
		 shadowcolor[i]+= 0.25f*(1.0f-shadowcolor[i]);
	}
    else if (name ==  JET ){
			for( i=0; i<3; i++)
			{			
			verttailcolor[i]+= 0.25f*(1.0f-verttailcolor[i]);
			}
	}
		 else  if (name ==  NOSE ){
				 
		 }
               else  if (name ==  BODY ){
				   
			   }
					 else  if (name ==   WING){
						
					 }
						   else  if (name ==   TAIL){
								   for( i=0; i<3; i++)
										{
										verttailcolor[i]+= 0.25f*(1.0f-verttailcolor[i]);
										}
						   }
								 else  if (name ==   TAILVERTICAL){
										   for( i=0; i<3; i++)
												{
												verttailcolor[i]+= 0.25f*(1.0f-verttailcolor[i]);
												}
								 }
									   else  if (name ==   TAILHORIZONTAL){
												
									   }
 glutPostRedisplay();
}


/*  processHits explores the contents of the 
 *  selection array.
 */
void processHits (GLint hits, GLuint buffer[])
{  if (hits==0)   {/*restore all colors*/
	initColors();
	return;
   }

   unsigned int i, j;
   GLuint names, *ptr;

   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++) {	/*  for each hit  */
      names = *ptr;
	  ptr+=3;
	                //  SHADOW
	 				 // JET + a jet detail
	               // JET + a jet detail + subdetail
	                      // SHADOW + JET + a jet detail 
	              // SHADOW + JET + a jet detail + subdetail
	  for (j = 0; j < names; j++)  /*  for each name */
		  anallizename( *ptr++);
   }
}

void pickPlaneDetails(int button, int state, int x, int y)
{
   
   GLint hits;
   GLint viewport[4];

   if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
      return;

   glGetIntegerv (GL_VIEWPORT, viewport);

   glSelectBuffer (BUFSIZE, selectBuf);
   (void) glRenderMode (GL_SELECT);

   glInitNames();
   glPushName(0);

   glMatrixMode (GL_PROJECTION);
   glPushMatrix ();
   glLoadIdentity ();
/*  create 5x5 pixel picking region near cursor location	*/
   gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
                  5.0, 5.0, viewport);
   /*gluOrtho2D (0.0, 3.0, 0.0, 3.0);
  */
   RenderScene (GL_SELECT);

   glMatrixMode (GL_PROJECTION);
   glPopMatrix ();
   //glFlush ();

   hits = glRenderMode (GL_RENDER);
   processHits (hits, selectBuf);
   glutPostRedisplay();
} 


void ChangeSize(GLsizei w, GLsizei h)
    {    // Set Viewport to window dimensions
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

		
    }



void SpecialKeys(int key, int x, int y)
    {GLfloat dx, dy, dz, dx0, dy0, dz0;
    
	if(key == GLUT_KEY_UP)
	{//increase distance from camera to origin
		dx=cx-ex; dy=cy-ey; dz=cz-ez; 
		ex-=deltaR*dx; ey-=deltaR*dy; ez-=deltaR*dz; 
		glutPostRedisplay();		return;
	}
    if(key == GLUT_KEY_DOWN)
	{//reduce distance from camera to origin (close up)
		dx=cx-ex; dy=cy-ey; dz=cz-ez; 
		ex+=deltaR*dx; ey+=deltaR*dy; ez+=deltaR*dz; 
       glutPostRedisplay();	return;
	}
    if(key == GLUT_KEY_LEFT)
		//Rotate (+) camera around origin in Oxz plane
	{ dx0=cz-ez;  dz0= ex - cx; 
		dx=cx-ex;  dz=cz-ez;
		GLfloat s=sqrtf(dx*dx+dz*dz);
		dx+=delta*dx0;       dz+=delta*dz0;
		GLfloat s1=sqrtf(dx*dx+dz*dz)/s;
	    dx/=s1;		dz/=s1; 
		ex=cx-dx; ez=cz-dz;
    glutPostRedisplay();return;
	}

    if(key == GLUT_KEY_RIGHT)
		//Rotate (-) camera around origin in Oxz plane
	{dx0=cz-ez;  dz0= ex - cx;  
		dx=cx-ex;  dz=cz-ez;
		GLfloat s=sqrtf(dx*dx+dz*dz);
		dx-=delta*dx0;       dz-=delta*dz0;
		GLfloat s1=sqrtf(dx*dx+dz*dz)/s;
	    dx/=s1;		dz/=s1; 
		ex=cx-dx; ez=cz-dz;
    glutPostRedisplay();return;}
 
	if(key ==  GLUT_KEY_F5)//Rotate camera around scene center in Oyz plane
         { dy0=cz-ez;  dz0= ey - cy;  
		  dy=cy-ey;    dz=cz-ez;
		GLfloat s=sqrtf(dy*dy+dz*dz);
		dy+=delta*dy0;       dz+=delta*dz0;
		GLfloat s1=sqrtf(dy*dy+dz*dz)/s;
	    dy/=s1;		dz/=s1; 
		ey=cy-dy; ez=cz-dz;
		glutPostRedisplay();return;}

	if(key ==  GLUT_KEY_F6)//Rotate camera around scene center in Oyz plane
         {dy0=cz-ez;  dz0= ey - cy;  
		  dy=cy-ey;    dz=cz-ez;
		GLfloat s=sqrtf(dy*dy+dz*dz);
		dy-=delta*dy0;       dz-=delta*dz0;
		GLfloat s1=sqrtf(dy*dy+dz*dz)/s;
	    dy/=s1;		dz/=s1; 
		ey=cy-dy; ez=cz-dz;
	glutPostRedisplay();return;}

	if(key ==  GLUT_KEY_F7)//Move light toward the center of ground 
        {	Buff_Light(); 
			dx=cx-wlightPos[0]; dy=cy-wlightPos[1];    dz=cz-wlightPos[2];
			dx*=(1.0-deltatrans); dy*=(1.0-deltatrans);
			dz*=(1.0-deltatrans);
			wlightPos[0]=cx-dx;  wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
			
			SetLightPos();
			 // Calculate new projection matrix to draw shadow on the ground
			
			glutPostRedisplay();return;
		}
	if(key ==  GLUT_KEY_F8)//Move light outward the center of ground 
        {	Buff_Light();
			dx=cx-wlightPos[0]; dy=cy-wlightPos[1];    dz=cz-wlightPos[2];
			
			dx/=(1.0-deltatrans); dy/=(1.0-deltatrans);
			dz/=(1.0-deltatrans);
			wlightPos[0]=cx-dx;  wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;
			
			SetLightPos();
			 // Calculate new projection matrix to draw shadow on the ground
			
			glutPostRedisplay();return;
		}

	if(key ==  GLUT_KEY_F9)//Rotate (+) light  around ... in the vertical plane 
        {	Buff_Light();
			dy0=cz-wlightPos[2];  dz0= wlightPos[1] - cy;  
		  dy=cy-wlightPos[1];    dz=cz-wlightPos[2];
		GLfloat s=sqrtf(dy*dy+dz*dz);
		dy+=delta*dy0;       dz+=delta*dz0;
		GLfloat s1=sqrtf(dy*dy+dz*dz)/s;
	    dy/=s1;		dz/=s1; 
		wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;

		SetLightPos();
		glutPostRedisplay();return;
		}
		if(key ==  GLUT_KEY_F10)//Rotate (-) light  around ... in the vertical plane 
        {	Buff_Light();
			dy0=cz-wlightPos[2];  dz0= wlightPos[1] - cy;  
		  dy=cy-wlightPos[1];    dz=cz-wlightPos[2];
		GLfloat s=sqrtf(dy*dy+dz*dz);
		dy-=delta*dy0;       dz-=delta*dz0;
		GLfloat s1=sqrtf(dy*dy+dz*dz)/s;
	    dy/=s1;		dz/=s1; 
		wlightPos[1]=cy-dy; wlightPos[2]=cz-dz;

		SetLightPos();
		glutPostRedisplay();return;
		}
			if(key ==  GLUT_KEY_F11)//Rote light  around ... in the horizontal plane 
        {	Buff_Light();
         dy=-wlightPos[2];   dz= wlightPos[0];
     GLfloat s=sqrtf(wlightPos[0]*wlightPos[0]+wlightPos[1]*wlightPos[1]
	                             +wlightPos[2]*wlightPos[2]);
		 wlightPos[0]+=delta*dy;      	wlightPos[2]+=delta*dz;
	GLfloat s1=sqrtf(wlightPos[0]*wlightPos[0]+wlightPos[1]*wlightPos[1]
	                             +wlightPos[2]*wlightPos[2])/s;
	wlightPos[0]/=s1;	wlightPos[1]/=s1;	wlightPos[2]/=s1;
	SetLightPos();
		
       glutPostRedisplay();return;
		}
	if(key ==  GLUT_KEY_F12)//Move light in horiz. plane  
        {	 Buff_Light();
			dy=wlightPos[0];   dz= wlightPos[2];
	        wlightPos[0]+=delta*dy;      	wlightPos[2]+=delta*dz;
		
			   // Calculate new projection matrix to draw shadow on the ground
			SetLightPos();
			glutPostRedisplay();return;
		}
	if(key ==  GLUT_KEY_F1){ bCull=!bCull;
	  // Turn on/off culling on if flag is set
		if(bCull)   glEnable(GL_CULL_FACE);
		else        glDisable(GL_CULL_FACE);
	    glutPostRedisplay();return;
		}
	if(key ==  GLUT_KEY_F2){bDepth=!bDepth;
		 if(bDepth)  glEnable(GL_DEPTH_TEST);
		else        glDisable(GL_DEPTH_TEST);
	    glutPostRedisplay();return;
		}

	if(key ==  GLUT_KEY_F3){bOutline=!bOutline;

    // Draw the back side as a wireframe only, if flag is set
    if(bOutline)glPolygonMode(GL_BACK,GL_LINE);
    else        glPolygonMode(GL_BACK,GL_FILL);
    glutPostRedisplay();	return;
	}
	if(key ==  GLUT_KEY_F4)
		{bShade=!bShade;
		if(bShade){glShadeModel(GL_FLAT);}
		   else {glShadeModel(GL_SMOOTH);} 
	    glutPostRedisplay();  return;
		}
    }

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   RenderScene(GL_RENDER);
   
}

int main(int argc, char** argv)
{
glutInit(&argc, argv);
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
glutInitWindowSize (500, 500);
glutInitWindowPosition (100, 100);
 glutCreateWindow (argv[0]);
 glutDisplayFunc(display);
    glutReshapeFunc(ChangeSize);
	 glutSpecialFunc (SpecialKeys);

	 glutMouseFunc (pickPlaneDetails);
    SetupRC();
    glutMainLoop();
return 0;
}
