/*
 *   Ana Karen Reyna Rivas   A01280310
 *   Iker Arbulu Lozano      A01190690
 *   Tercer Avance: Proyecto El Mundo de Verminara
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include "glm.h"
#include "imageloader.h"
#include "imageBMP.h"
#include <assert.h>
#include <queue>

using namespace std;

int	ancho = 600;
int largo = 600;
int iSegundos = 0;                                      // 9000000 = 10min
bool juegoInicio = false;
bool ayuda = true;
bool contar;

bool inicio = true, pausado, comenzado, reiniciar, terminado, jugando;
bool autores, instrucciones,historia, mundos;

// Variables para juego 1
bool juego1 = false, fallo = false, acierto = false;
bool juego1Ganado,juego1Perdido;
int puntosJuego1;
int vidasJuego1 = 3;

// Variables para juego 2
bool juego2, juego2Ganado;
int contJuego2 = 0, auxJuego2 = 0;
int objJ2Cont1 = 0, objJ2Cont2 = 0, objJ2Cont3 = 0, objJ2Cont4 = 0, objJ2Cont5 = 0, objJ2Cont6 = 0;
int objGanados = 0, numTip = 1;
bool mostrandoTip;
const int TEXTURE_COUNT = 8;
int anguloJuegos = 21;

// Para objetos 3D
static GLuint texName[40];                                    // Texturas
GLMmodel model[40];
string fullPath = __FILE__;
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };      //Puntual

int angulo=-1;                                          // Animaci�n de objetos

/////////////////////////////////////////////////////////////////////
///////           Cubos de juego                          ///////////
/////////////////////////////////////////////////////////////////////
struct Cubo{
  char tecla;
  int x;
};
int anchoCubo = 100;
int velocidadCubo = 5;
queue<Cubo> cubos;
int vueltas=0;
int cantCubos=0;
/////////////////////////////////////////////////////////////////////

void aceleraCubos(){
  Cubo aux;
  int cantResta=0;
  for (int i = 0; i<cantCubos; i++) {
    aux = cubos.front();
    if (aux.x+anchoCubo/2.0 < (-anchoCubo/2.0)) {
      cubos.pop();
      cantResta++;
      fallo = true;
      acierto = false;
      vidasJuego1--;
      if (vidasJuego1 == 0) {
        juego1Perdido = true;
      }
    }
    else{
      aux.x -= velocidadCubo;
      cubos.pop();
      cubos.push(aux);
    }
  }
  cantCubos-=cantResta;
}

//Makes the image into a texture, and returns the id of the texture
void loadTexture(Image* image,int k)
{

  glBindTexture(GL_TEXTURE_2D, texName[k]); //Tell OpenGL which texture to edit

  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

  //Map the image to the texture
  glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
               0,                            //0 for now
               GL_RGB,                       //Format OpenGL uses for image
               image->width, image->height,  //Width and height
               0,                            //The border of the image
               GL_RGB, //GL_RGB, because pixels are stored in RGB format
               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
               //as unsigned numbers
               image->pixels);               //The actual pixel data
}

void initRendering()
{
    int i=0;
    /*glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); ///Users/mariaroque/Imagenes*/
    // glEnable(GL_COLOR_MATERIAL);
    glGenTextures(40, texName); //Make room for our texture

    //Iker
    /*Image* image = loadBMP("/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/MenuResized.bmp");
    loadTexture(image,i++);
    image = loadBMP("/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/AutoresResized.bmp");
    loadTexture(image,i++);
    image = loadBMP("/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/InstruccionesResized.bmp");
    loadTexture(image,i++);
    image = loadBMP("/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/HistoriaResized.bmp");
    loadTexture(image,i++);*/
    // Esto ya debe funcionar para todos los casos
    char  ruta[200];
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/MenuResized.bmp");                    // 0
    Image* image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/AutoresResized.bmp");                 // 1
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/InstruccionesResized.bmp");           // 2
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/HistoriaResized.bmp");                // 3
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/ChestOpen.bmp");                      // 4
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/Juego1Resized.bmp");                  // 5
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/Juego2Resized.bmp");                  // 6
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized.bmp");                  // 7
    image = loadBMP(ruta);
    loadTexture(image,i++);

    // Imagenes para tips
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/ComerResized1.bmp");             // 8
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/ComerResized2.bmp");             // 9
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/ComerResized3.bmp");             // 10
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/EjercicioResized1.bmp");         // 11
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/EjercicioResized2.bmp");         // 12
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/EjercicioResized3.bmp");         // 13
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/HigieneResized.bmp");            // 14
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/ActividadSocialResized.bmp");    // 15
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/HabitosToxicosResized.bmp");     // 16
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/DormirResized1.bmp");            // 17
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/DormirResized2.bmp");            // 18
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/tips/DormirResized3.bmp");            // 19
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/Juego2GanadoResized.bmp");            // 20
    image = loadBMP(ruta);
    loadTexture(image,i++);

    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized1.bmp");                 // 21
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized2.bmp");                 // 22
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized3.bmp");                 // 23
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized4.bmp");                 // 24
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized5.bmp");                 // 25
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized6.bmp");                 // 26
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized7.bmp");                 // 27
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/JuegosResized8.bmp");                 // 28
    image = loadBMP(ruta);
    loadTexture(image,i++);

    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/PausadoResized.bmp");                 // 29
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/MundosResized.bmp");                  // 30
    image = loadBMP(ruta);
    loadTexture(image,i++);

    delete image;
}


// Timer para que los objetos roten
static void timer(int i){
  angulo += 10;
  
  if (jugando and juego1 and !juego2 and !juego1Perdido and !juego1Ganado) {
    if (i ==2) {
      Cubo aux;
      aux.x = ancho;
      do{
        aux.tecla = rand()%26 + 97;
      }while (aux.tecla == 'j' or aux.tecla == 'r' or aux.tecla == 'p');
      cubos.push(aux);
      cantCubos++;
      vueltas = 0;
      glutTimerFunc(40, timer, 1);
    }
    else if(i==1){
      if (cantCubos == 0) {
        glutTimerFunc(20,timer, 2);
      }
      else if (vueltas*velocidadCubo >= anchoCubo*2.4) {
        glutTimerFunc(20, timer, 2);
      }
      else{
        aceleraCubos();
        vueltas++;
        glutTimerFunc(80, timer, 1);
      }
    }
  }
  else{

    // Variables para mover los objetos en el juego 2
    if(juego2 and auxJuego2 == 0 and !mostrandoTip and !pausado){
        contJuego2 += 5;
        if(contJuego2 > 420)
            auxJuego2 = 1;
    }
    else if(juego2 and auxJuego2 == 1 and !mostrandoTip and !pausado){
        contJuego2 -= 5;
        if(contJuego2 < 0)
            auxJuego2 = 0;
    }

    glutTimerFunc(80, timer, 1);
  }
  glutPostRedisplay();
}

// Timer para cambiar las imagenes de la pantalla Juegos
void timerPantallaJuegos(int value)
{
    if(jugando){
        anguloJuegos = anguloJuegos + 1;
        if (anguloJuegos == TEXTURE_COUNT + 21) anguloJuegos =21;
        glutPostRedisplay();
        glutTimerFunc(150,timerPantallaJuegos,0);
    }
}

// Objeto 3D
static void manzana(){
    glmDraw(&model[0], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void pesa(){
    glmDraw(&model[1], GLM_TEXTURE|GLM_FLAT);
}

// Objeto 3D
static void jabon(){
    glmDraw(&model[2], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void globos(){
    glmDraw(&model[3], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void cigarro(){
    glmDraw(&model[4], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void cama(){
    glmDraw(&model[5], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void diamondGreen(){
    glmDraw(&model[6], GLM_COLOR|GLM_FLAT);
}

// Objeto 3D
static void diamondLightGreen(){
    glmDraw(&model[7], GLM_COLOR|GLM_FLAT);
}

// Desplegar texto en tama�o peque�o
static void letreroLetraPequena(string texto){
  int yRaster = 0;
  int xRaster = 0;

  glColor3f(0, 0, 0);

  for(int j = 0; j < texto.length(); j++){
    char valor = texto.at(j);
    glRasterPos2i(xRaster, yRaster);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, valor);
    xRaster += 10;
  }
}

// Desplegar texto en menu
static void letreroMenu(string texto){
  int yRaster = 70;
  int xRaster = 70;

  glColor3f(0, 0, 0);

  for(int j = 0; j < texto.length(); j++){
    char valor = texto.at(j);
    glRasterPos2i(xRaster, yRaster);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, valor);
    xRaster += 7;
  }
}

// Desplegar texto
static void letrero(string texto){

  int yRaster = 0;
  int xRaster = 0;

  for(int j = 0; j < texto.length(); j++){
    char valor = texto.at(j);
    glRasterPos2i(xRaster, yRaster);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, valor);
    xRaster += 9;
  }
}

// Dibuja la base que rodea a los autores
static void dibujaBaseAutores(){
  glTranslatef(30,0,0);
  glColor3f(0.098,0.098,0.439);
  glPushMatrix();
  glTranslatef(0,-20,-1);
  glScalef(40,30,10);
  glutSolidDodecahedron();
  glPopMatrix();
  glColor3f(0-117,0.5647,1);
  glPushMatrix();
  glScalef(40,30,10);
  glutSolidDodecahedron();
  glPopMatrix();
  glTranslatef(-50,0,0);
}

// Dibuja la base de Regresar
static void dibujaBaseRegresar(string letreroDesplegar){
  // Dibuja base regresar
    glPushMatrix();
        glTranslatef(((ancho/2)-40),(largo/4)+120,-30);
        glRotatef(angulo,0,1,0);
        glScalef(0.8,1,1);
        glTranslatef(-35,0,0);
        glTranslatef(30,0,0);
            glColor3f(0.098,0.098,0.439);
            glPushMatrix();
                glTranslatef(0,-20,-1);
                glScalef(20,15,5);
                glutSolidDodecahedron();
            glPopMatrix();
            glColor3f(0-117,0.5647,1);
            glPushMatrix();
                glScalef(20,15,5);
                glutSolidDodecahedron();
            glPopMatrix();
            glTranslatef(-50,0,0);
        glTranslatef(0,0,20);
        letreroLetraPequena(letreroDesplegar);
    glPopMatrix();
}

// Funci�n para cargar la imagen textura del fondo
static void cargarImagenFondo(int indice){
  // Background Image Texture
  glPushMatrix();
      glTranslatef(-ancho/2.0,-largo/2.0,-100);
      glPointSize(1);
      glLineWidth(3);
      glBindTexture(GL_TEXTURE_2D, texName[indice]);
      glBegin(GL_QUADS);
      glColor4ub(255, 255, 255,255);       // Color
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(ancho, 0);                 // v0
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(ancho, largo);               // v1
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(0, largo);                 // v2
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(0, 0);                   // v3
      glEnd();
  glPopMatrix();
}

// Deplegar diamante tip de juego 2
static void tipsDiamanteDesplegado(int posX){
    // Mueve el objeto: cigarro
    glPushMatrix();
        glTranslatef(posX, -210, -50);
        glRotatef(angulo, 0, 1, 0);
        glScalef(20,-20,20);
        diamondGreen();
    glPopMatrix();
}

// Deplegar diamante tip de juego 2
static void tipsDiamanteNoDesplegado(int posX){
    // Mueve el objeto: cigarro
    glPushMatrix();
        glTranslatef(posX, -210, -50);
        glRotatef(angulo, 0, 1, 0);
        glScalef(20,-20,20);
        diamondLightGreen();
    glPopMatrix();
}

// Desplegado en pantalla principal
static void pantallaInicial(){
  // Cargar la imagen textura del fondo
  cargarImagenFondo(0);

  // Bot�n Autores
  glPushMatrix();
  glTranslatef(((ancho/2)-60),(largo/4)+80,-45);
  glRotatef(angulo,0,1,0);
  glScalef(0.8,1,1);
  glTranslatef(-35,0,0);
  dibujaBaseAutores();
  glTranslatef(0,0,20);
  letreroLetraPequena("A - Autores");
  glPopMatrix();

  // Objeto 3D
  /*glPushMatrix();
   glTranslatef((ancho/4),100,-45);
   glScalef(40,40,15);
   glRotatef(angulo,0,1,0);
   objeto3D();
   glPopMatrix();*/
}

// Despliega pantalla de autores
static void pantallaAutores(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(1);
    // Dibuja base regresar
    dibujaBaseRegresar("A - Menu");
}

// Despliega pantalla de instrucciones
static void pantallaInstrucciones(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(2);
    // Dibuja base regresar
    dibujaBaseRegresar("I - Menu");
}

// Despliega pantalla de historia
static void pantallaHistoria(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(3);
    // Dibuja base regresar
    dibujaBaseRegresar("H - Menu");
}

// Despliega pantalla de historia
static void pantallaMundos(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(30);
    // Dibuja base regresar
    dibujaBaseRegresar("V - Menu");
}

// Pantalla Juegos
static void pantallaJuegos(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(anguloJuegos);
    // Dibuja base regresar
    dibujaBaseRegresar("J - Menu");
}

float ambiente[][4]={
  {0.19225, 0.19225, 0.19225, 1.0}//plata
};

float difuso[][4]={
  {0.50754, 0.50754, 0.50754, 1.0},//plata
  {0.098, 0.098, 0.439,1},
  {0-117, 0.5647,1,1},
  {0, 0, 0, 1}
};

float especular[][4]={
  {0.508273, 0.508273, 0.508273,1.0}//plata
};

float brillo[]={
  0.4f
};

void lucesMaterial(int i)
{
  //Asigna los apropiados materiales a las superficies
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambiente[i]);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,difuso[i]);
  glMaterialfv(GL_FRONT,GL_SPECULAR,especular[i]);
  glMaterialf(GL_FRONT,GL_SHININESS,brillo[i]*128.0);
  // asigna la apropiada fuente de luz
  GLfloat lightIntensity[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light_position[] = {2.0f, 2.0f, 3.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION,light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,lightIntensity);
  //asigna la c√°mara
  //comienza el dibujo
  
}

static void pantallaJuego1(){
  // Cargar la imagen textura del fondo
  cargarImagenFondo(5);
  // Dibuja base regresar
  dibujaBaseRegresar("J - Menu");
  
  glColor3f(0, 0, 0);
  char puntos[10];
  sprintf(puntos, "%d",puntosJuego1);
  glColor3f(0, 0, 0);
  glRasterPos2i(-ancho/2 + 30, largo/2-40);
  for (GLint k = 0; puntos[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, puntos[k]);
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  lucesMaterial(0);
  int xPos = -50;
  for (int i = 0; i<vidasJuego1; i++) {
    glPushMatrix();
      glTranslatef(xPos, largo/2-50, -25);
      glRotatef(angulo, 1, 1, 1);
      glScalef(20,20,20);
      pesa();
    glPopMatrix();
    xPos+=50;
  }
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);
  
  Cubo aux;
  for (int i = 0; i<cantCubos; i++) {
    lucesMaterial(1);
    glColor4ub(0, 0, 0,0);       // Color
    aux = cubos.front();
    //glRasterPos2f(aux.x, (-largo/2.0)+anchoCubo/2.0+20);
    glPushMatrix();
      glTranslatef(aux.x, (-largo/2.0)+anchoCubo/2.0+20, -45);
      glRotatef(angulo,0,1,0);
      //glScalef(0.8,1,1);
      glColor3f(0.098,0.098,0.439);
        glPushMatrix();
        glTranslatef(0,-20,-1);
        glScalef(20,20,5);
        glutSolidDodecahedron();
      glPopMatrix();
      glColor3f(0-117,0.5647,1);
      glPushMatrix();
        glScalef(20,20,5);
        glutSolidDodecahedron();
      glPopMatrix();
      glTranslatef(0,0,20);
      glColor3f(0, 0, 0);
      glRasterPos2f(0, 0);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, aux.tecla);
    glPopMatrix();
    cubos.pop();
    cubos.push(aux);
  }
  
  if (fallo) {
    glColor3b(255, 0, 0);
    glPushMatrix();
    glTranslatef(0, -largo/2.0+anchoCubo/2.0+20, -45);
    glutSolidCube(anchoCubo);
    glPopMatrix();
  }
  else if (acierto){
    glColor3b(0, 255, 0);
    glPushMatrix();
    glTranslatef(0, -largo/2.0+anchoCubo/2.0+20, -45);
    glutSolidSphere(anchoCubo/2.0, 20, 20);
    glPopMatrix();
  }
  else{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName[4]);
        glTranslatef(0, -largo/2.0+anchoCubo/2.0+20, -80);
        glutWireCube(anchoCubo);
    glPopMatrix();

    // Puse un QUADS en lugar, pero no se si esto sea lo mejor por lo del cubo que quieres manejar
    glPushMatrix();
        glTranslatef(-50, -largo/2.0+anchoCubo/2.0-30, -80);
        glBindTexture(GL_TEXTURE_2D, texName[4]);
        glBegin(GL_QUADS);
            glColor4ub(255, 255, 255,255);       // Color
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(100, 0);                 // v0
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(100, 100);               // v1
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0, 100);                 // v2
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0, 0);                   // v3
        glEnd();
    glPopMatrix();
  }

  acierto = false;
  fallo = false;
}

// Juego 2
static void pantallaJuego2(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(6);

    // Dibuja base regresar
    dibujaBaseRegresar("J - Menu");

    int movY = -145;

    // Mueve el objeto: manzana
    glPushMatrix();
        glTranslatef(-210, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        manzana();
    glPopMatrix();
    // Despliega el numero del objeto: manzana
    glPushMatrix();
        glTranslatef(-220, movY + contJuego2 - 40, 0);
        letrero("1");
    glPopMatrix();

    // Mueve el objeto: pesa
    glPushMatrix();
        glTranslatef(-126, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        pesa();
    glPopMatrix();
    // Despliega el numero del objeto: pesa
    glPushMatrix();
        glTranslatef(-136, movY + contJuego2 - 40, 0);
        letrero("2");
    glPopMatrix();

    // Mueve el objeto: jabon
    glPushMatrix();
        glTranslatef(-42, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        jabon();
    glPopMatrix();
    // Despliega el numero del objeto: jabon
    glPushMatrix();
        glTranslatef(-52, movY + contJuego2 - 40, 0);
        letrero("3");
    glPopMatrix();

    // Mueve el objeto: globos
    glPushMatrix();
        glTranslatef(42, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        globos();
    glPopMatrix();
    // Despliega el numero del objeto: globos
    glPushMatrix();
        glTranslatef(32, movY + contJuego2 - 40, 0);
        letrero("4");
    glPopMatrix();

    // Mueve el objeto: cigarro
    glPushMatrix();
        glTranslatef(126, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        cigarro();
    glPopMatrix();
    // Despliega el numero del objeto: cigarro
    glPushMatrix();
        glTranslatef(116, movY + contJuego2 - 40, 0);
        letrero("5");
    glPopMatrix();

    // Mueve el objeto: cama
    glPushMatrix();
        glTranslatef(210, movY + contJuego2, -25);
        glRotatef(angulo, 1, 1, 1);
        glScalef(20,20,20);
        cama();
    glPopMatrix();
    // Despliega el numero del objeto: cama
    glPushMatrix();
        glTranslatef(200, movY + contJuego2 - 40, 0);
        letrero("6");
    glPopMatrix();
}

// Juego 2 Ganado
static void pantallaJuego2Ganado(){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(20);
    // Dibuja base regresar
    dibujaBaseRegresar("R - Reiniciar");

    // Mueve el objeto: manzana
    glPushMatrix();
        glTranslatef(-270, 100, -25);
        glRotatef(angulo, 0, 1, 0);
        glRotatef(100, -1, 0, 0);
        glScalef(-20,-20,20);
        manzana();
    glPopMatrix();

    // Mueve el objeto: pesa
    glPushMatrix();
        glTranslatef(-180, 170, -25);
        glRotatef(angulo, 0, 1, 0);
        //glRotatef(10, 1, 0, 0);
        glScalef(20,20,20);
        pesa();
    glPopMatrix();

    // Mueve el objeto: jabon
    glPushMatrix();
        glTranslatef(-200, 0, -25);
        glRotatef(angulo, 0, 1, 0);
        glScalef(20,20,20);
        jabon();
    glPopMatrix();

    // Mueve el objeto: globos
    glPushMatrix();
        glTranslatef(-100, 70, -15);
        glRotatef(angulo, 0, 1, 0);
        glRotatef(20, 1, 0, 0);
        glScalef(40,40,40);
        globos();
    glPopMatrix();

    // Mueve el objeto: cigarro
    glPushMatrix();
        glTranslatef(-220, -120, -25);
        glRotatef(angulo, 0, 1, 0);
        glRotatef(-100, 1, 0, 0);
        glScalef(40,40,40);
        cigarro();
    glPopMatrix();

    // Mueve el objeto: cama
    glPushMatrix();
        glTranslatef(-100, -70, -25);
        glRotatef(angulo, 0, 1, 0);
        glRotatef(30, 1, 0, 0);
        glScalef(20,20,20);
        cama();
    glPopMatrix();
}

void validarPresionado(char theKey){
  if (!cubos.empty()) {
    if (cubos.front().x-(anchoCubo/2.0) < anchoCubo/2.0) {
      if (cubos.front().tecla == theKey) {
        acierto = true;
        fallo = false;
        puntosJuego1+=100;
        if (puntosJuego1 >= 1000) {
          juego1Ganado = true;
        }
        cubos.pop();
        cantCubos--;
      }
    }
  }
}

// Toma la ubicaci�n del proyecto
void getParentPath(){
  //Ana
//  for (int i = fullPath.length()-1; i>=0 && fullPath[i] != '\\'; i--) {
//    fullPath.erase(i,1);
//  }
  //Iker
  for (int i = fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
    fullPath.erase(i,1);
  }
}

// Reshape
void reshape(int anchop, int largop)
{
  ancho = anchop;
  largo = largop;
  velocidadCubo = 0.0084 * ancho;
  glViewport(0.0,0.0,(GLdouble) ancho,(GLdouble)  largo);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ancho/2, ancho/2, -largo/2, largo/2, 1, 150);
  gluLookAt(0, 0, 15, 0, 0, 0, 0, 1,0);
}

// Funcion para desplegar el fondo de cada tip y los diamantes que señalan el número de tip desplegado
// Cuando hay 3 tips
static void desplegarMasDeUnTip(int numImagenFondo){
    // Dibuja base regresar
    dibujaBaseRegresar("C-Continuar");

    // Letrero
    glPushMatrix();
        glTranslatef(-((ancho/2)-180),-(largo/4)-80,-45);
        letreroLetraPequena("S - Para ver siguiente tip");
    glPopMatrix();

    // Desplegar el número de tip
    if(numTip == 1){
        cargarImagenFondo(numImagenFondo);
        glColor3f(255,0,0);
        tipsDiamanteDesplegado(-100);
        tipsDiamanteNoDesplegado(-50);
        tipsDiamanteNoDesplegado(0);
    }
    else if(numTip == 2){
        cargarImagenFondo(numImagenFondo + 1);
        glColor3f(255,0,0);
        tipsDiamanteNoDesplegado(-100);
        tipsDiamanteDesplegado(-50);
        tipsDiamanteNoDesplegado(0);
    }
    else if(numTip == 3){
        cargarImagenFondo(numImagenFondo + 2);
        glColor3f(255,0,0);
        tipsDiamanteNoDesplegado(-100);
        tipsDiamanteNoDesplegado(-50);
        tipsDiamanteDesplegado(0);
    }
}

// Funcion para desplegar el fondo del tip y los diamantes que señalan el número de tip desplegado
// Cuando solo hay un tip
static void desplegarUnTip(int numImagenFondo){
    // Dibuja base regresar
    dibujaBaseRegresar("C-Continuar");

    cargarImagenFondo(numImagenFondo);
    glColor3f(255,0,0);
    tipsDiamanteDesplegado(-100);
}

// Display
static void myDisplay(void)
{
  glClearColor(1,1,1,0);                                  // Color del background

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Activa profundidad
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_TEXTURE_2D);

  if(juego2Ganado){
    pantallaJuego2Ganado();
  }
  else if(pausado){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(29);
  }
  else if(juego2 and juego2Ganado){
    // Cargar la imagen textura del fondo
    cargarImagenFondo(5);
  }
  else if(juego2 and objJ2Cont1 == 3 and mostrandoTip){
    // Cargar los tips
    desplegarMasDeUnTip(8);
  }
  else if(objJ2Cont2 == 3 and mostrandoTip){
    // Cargar los tips
    desplegarMasDeUnTip(11);
  }
  else if(objJ2Cont3 == 3 and mostrandoTip){
    // Cargar el tip
    desplegarUnTip(14);
  }
  else if(objJ2Cont4 == 3 and mostrandoTip){
    // Cargar el tip
    desplegarUnTip(15);
  }
  else if(objJ2Cont5 == 3 and mostrandoTip){
    // Cargar el tip
    desplegarUnTip(16);
  }
  else if(objJ2Cont6 == 3 and mostrandoTip){
    // Cargar los tips
    desplegarMasDeUnTip(17);
  }
  else if(jugando and !juego1 and !juego2){             // Menu Juegos
    pantallaJuegos();
  }
  else if(jugando and juego1 and !juego2){              // Despliega pantalla de juego 1
    pantallaJuego1();
  }
  else if(jugando and !juego1 and juego2){               // Despliega pantalla de juego 2
    pantallaJuego2();
  }
  else if(autores){
    pantallaAutores();                                  // Despliega pantalla autores
  }
  else if(instrucciones){
    pantallaInstrucciones();                            // Despliega pantalla instrucciones
  }
  else if(historia){
    pantallaHistoria();                                 // Despliega pantalla historia
  }
  else if (mundos){
    pantallaMundos();                                   // Despliega pantalla mundos
  }
  else if(inicio){
    pantallaInicial();                                  // Despliega pantalla inicial
  }

  glutSwapBuffers();
}

void init(){
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Inicializar Imagenes
    int i = 0;

    glEnable(GL_NORMALIZE);
    // Modelo para sección: comer saludablemente
    string ruta = fullPath+ "imagenes/apple.obj";
    model[0]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[0]);
    glmVertexNormals(&model[0],90.0,GL_TRUE);

    // Modelo para sección: hacer ejercicio
    ruta = fullPath+ "imagenes/weight.obj";
    model[1]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[1]);
    glmVertexNormals(&model[1],90.0,GL_TRUE);

    // Modelo para sección: higiene
    ruta = fullPath+ "imagenes/soap.obj";
    model[2]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[2]);
    glmVertexNormals(&model[2],90.0,GL_TRUE);

    // Modelo para sección: actividad social
    ruta = fullPath+ "imagenes/party.obj";
    model[3]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[3]);
    glmVertexNormals(&model[3],90.0,GL_TRUE);

    // Modelo para sección: hábitos tóxicos
    ruta = fullPath+ "imagenes/cig.obj";
    model[4]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[4]);
    glmVertexNormals(&model[4],90.0,GL_TRUE);

    // Modelo para sección: dormir
    ruta = fullPath+ "imagenes/bed.obj";
    model[5]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[5]);
    glmVertexNormals(&model[5],90.0,GL_TRUE);

    // Modelo diamond Green
    ruta = fullPath+ "imagenes/diamondGreen.obj";
    model[6]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[6]);
    glmVertexNormals(&model[6],90.0,GL_TRUE);

    // Modelo diamond Light Green
    ruta = fullPath+ "imagenes/diamondLightGreen.obj";
    model[7]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[7]);
    glmVertexNormals(&model[7],90.0,GL_TRUE);

  // Objetos 3D
  // Ana
  // // C:/Users/karen_000/Dropbox/6 Semestre/Gr�ficas/MundoDeVerminara3D/Windows/imagenes/beer/beer.obj
  //string ruta = fullPath + "imagenes/hamburger/hamburger.obj";
  // Iker
  //string ruta = "/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/hamburger/hamburger.obj";
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
  if (jugando and juego1) {
    validarPresionado(theKey);
  }
  switch (theKey)
  {
    case 'a':                                                   // Autores
    case 'A':
      if(inicio and !jugando and !pausado and !terminado and !instrucciones and !historia and !mundos){
        autores = !autores;
      }
      break;
    case 'i':                                                   // Instrucciones
    case 'I':
      if(inicio and !jugando and !pausado and !terminado and !autores and !historia and !mundos){
        instrucciones = !instrucciones;
      }
      break;
    case 'h':                                                   // Historia
    case 'H':
      if(inicio and !jugando and !pausado and !terminado and !autores and !instrucciones and !mundos){
        historia = !historia;
      }
      break;
    case 'v':                                                   // Historia
    case 'V':
      if(inicio and !jugando and !pausado and !terminado and !autores and !instrucciones and !historia){
        mundos = !mundos;
      }
      break;
    case 'j':
    case 'J':
      if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones){
        jugando = !jugando;
        juego1 = juego2 = false;
        glutTimerFunc(200,timerPantallaJuegos,0);
      }
      break;
    case 's':
    case 'S':
      if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones and jugando and juego2 and mostrandoTip){
        if(numTip < 3)
            numTip++;
      }
      break;
    case 'p':
    case 'P':
      if((juego1 or juego2) and !juego1Ganado and !juego2Ganado and !mostrandoTip){
        pausado = !pausado;
      }
      break;
    // Continuar juego
    case 'c':
    case 'C':
        if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones and jugando and juego2 and mostrandoTip){
            mostrandoTip = false;
            numTip = 1;

            // Incrementar contador para que ya no despliegue el tip
            if(objJ2Cont1 == 3)
                objJ2Cont1++;
            else if(objJ2Cont2 == 3)
                objJ2Cont2++;
            else if(objJ2Cont3 == 3)
                objJ2Cont3++;
            else if(objJ2Cont4 == 3)
                objJ2Cont4++;
            else if(objJ2Cont5 == 3)
                objJ2Cont5++;
            else if(objJ2Cont6 == 3)
                objJ2Cont6++;

            // Aumenta los tips vistos, cuando llega a 5 el juego 2 termina
            objGanados++;
            if(objGanados == 6){
                juego2Ganado = true;
            }
        }
        break;
    // Reiniciar juego
    case 'r':
    case 'R':
        // Asignar valores default
        iSegundos = 0;
        juegoInicio = false;
        ayuda = true;
        contar = false;

        inicio = true;
        pausado = comenzado = reiniciar = terminado = false;
        autores = instrucciones = historia = mundos = false;

        // Variables para juego 1
        fallo = acierto = juego1Ganado = false;

        // Variables para juego 2
        juego2Ganado = mostrandoTip = false;
        contJuego2 = 0, auxJuego2 = 0;
        objJ2Cont1 = objJ2Cont2 = objJ2Cont3 = objJ2Cont4 = objJ2Cont5 = objJ2Cont6 = 0;
        objGanados = 0;
        numTip = 1;
      
        while (cubos.size()!=0) {
          cubos.pop();
        }
      
        juego1Ganado = juego1Perdido = false;
        puntosJuego1 = 0;
        vidasJuego1 = 3;
      
        angulo=-1;

        anchoCubo = 100;
        velocidadCubo = 0.0084 * ancho;
        vueltas=0;
        cantCubos=0;
        break;
    case '1':
        // Juego 2, seleccionar manzana
        if(inicio and jugando and juego2 and objJ2Cont1 < 3 and !mostrandoTip){
            objJ2Cont1++;
            if(objJ2Cont1 == 3){
                mostrandoTip = true;
            }
        }
        // Juego 1
        else if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones and jugando and !juego2){
            juego1 = !juego1;
        }
        break;
    case '2':
        // Juego 2, seleccionar pesa
        if(inicio and jugando and juego2 and objJ2Cont2 < 3 and !mostrandoTip){
            objJ2Cont2++;
            if(objJ2Cont2 == 3){
                mostrandoTip = true;
            }
        }
        // Juego 2
        else if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones and jugando and !juego1 and objJ2Cont2 < 3 and !mostrandoTip){
            juego2 = !juego2;
        }
        break;
    case '3':
        // Juego 2, seleccionar jabon
        if(inicio and jugando and juego2 and objJ2Cont3 < 3 and !mostrandoTip){
            objJ2Cont3++;
            if(objJ2Cont3 == 3){
                mostrandoTip = true;
            }
        }
        break;
    case '4':
        // Juego 2, seleccionar globos
        if(inicio and jugando and juego2 and objJ2Cont4 < 3 and !mostrandoTip){
            objJ2Cont4++;
            if(objJ2Cont4 == 3){
                mostrandoTip = true;
            }
        }
        break;
    case '5':
        // Juego 2, seleccionar cigarro
        if(inicio and jugando and juego2 and objJ2Cont5 < 3 and !mostrandoTip){
            objJ2Cont5++;
            if(objJ2Cont5 == 3){
                mostrandoTip = true;
            }
        }
        break;
    case '6':
        // Juego 2, seleccionar cama
        if(inicio and jugando and juego2 and objJ2Cont6 < 3 and !mostrandoTip){
            objJ2Cont6++;
            if(objJ2Cont6 == 3){
                mostrandoTip = true;
            }
        }
        break;
    case 27:                                                    // Quit
      exit(-1);
      break;
    default:
      break;		                                            // Do nothing
  }
  glutPostRedisplay();
}


void mouse(int button,int state,int x,int y){
  y = largo - y;                                   // cambiar las coordenadas a coordenadas de openGL
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(ancho, largo);
  glutInitWindowPosition(0, 0);                           // Create window
  glutCreateWindow("El Mundo de Verminara");
  getParentPath();
  init();
  initRendering();
  glutDisplayFunc(myDisplay);
  glutKeyboardFunc(myKeyboard);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutTimerFunc(100,timer,1);
  glutMainLoop();
  return 0;
}
