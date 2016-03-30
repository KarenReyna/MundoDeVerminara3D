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

bool inicio = true, pausado, comenzado, reiniciar, terminado, jugando, fallo = false,acierto = false;
bool autores, instrucciones,historia;

// Para objetos 3D
static GLuint texName[36];                                    // Texturas
GLMmodel model[6];
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
    glGenTextures(6, texName); //Make room for our texture

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
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/MenuResized.bmp");
    Image* image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/AutoresResized.bmp");
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/InstruccionesResized.bmp");
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/HistoriaResized.bmp");
    image = loadBMP(ruta);
    loadTexture(image,i++);
    sprintf(ruta,"%s%s", fullPath.c_str() , "imagenes/ChestOpen.bmp");
    image = loadBMP(ruta);
    loadTexture(image,i++);
    delete image;
}


// Timer para que los objetos roten
static void timer(int i){
  if (jugando) {
    if (i ==2) {
      Cubo aux;
      aux.x = ancho;
      do{
        aux.tecla = rand()%26 + 97;
      }while (aux.tecla == 'j');
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
    angulo += 10;
    glutTimerFunc(80, timer, 1);
  }
  glutPostRedisplay();
}

static void objeto3D(){
  glmDraw(&model[0], GLM_TEXTURE| GLM_COLOR | GLM_FLAT);
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

// Funci�n para cargar la imagen textura del fondo
static void cargarImagenFondo(int indice){
  // Background Image Texture
  glPushMatrix();
  glTranslatef(-300,-300,-45);
  glPointSize(1);
  glLineWidth(3);
  glBindTexture(GL_TEXTURE_2D, texName[indice]);
  glBegin(GL_QUADS);
  glColor4ub(255, 255, 255,255);       // Color
  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(600, 0);                 // v0
  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(600, 600);               // v1
  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0, 600);                 // v2
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0, 0);                   // v3
  glEnd();
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

  // Bot�n Quitar autores
  /*glPushMatrix();
   glTranslatef(0,-150,-45);
   glRotatef(angulo,0,1,0);
   glScalef(0.8,1,1);
   dibujaBaseAutores();
   glTranslatef(0,0,20);
   letreroLetraPequena("A - Home");
   glPopMatrix();*/
}

// Despliega pantalla de instrucciones
static void pantallaInstrucciones(){
  // Cargar la imagen textura del fondo
  cargarImagenFondo(2);
}

// Despliega pantalla de historia
static void pantallaHistoria(){
  // Cargar la imagen textura del fondo
  cargarImagenFondo(3);
}

static void pantallaJugando(){
  glColor3f(0, 0, 0);
  if (fallo) {
    glColor3b(255, 0, 0);
    glPushMatrix();
    glTranslatef(0, -largo/2.0+anchoCubo/2.0+10, 0);
    glutSolidCube(anchoCubo);
    glPopMatrix();
  }
  else if (acierto){
    glColor3b(0, 255, 0);
    glPushMatrix();
    glTranslatef(0, -largo/2.0+anchoCubo/2.0+10, 0);
    glutSolidSphere(anchoCubo/2.0, 20, 20);
    glPopMatrix();
  }
  else{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texName[4]);
        glTranslatef(0, -largo/2.0+anchoCubo/2.0+10, 0);
        glutWireCube(anchoCubo);
    glPopMatrix();

    // Puse un QUADS en lugar, pero no se si esto sea lo mejor por lo del cubo que quieres manejar
    glPushMatrix();
        glTranslatef(-50, -largo/2.0+anchoCubo/2.0-40, 0);
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

  Cubo aux;
  for (int i = 0; i<cantCubos; i++) {
    glColor4ub(0, 0, 0,0);       // Color
    aux = cubos.front();
    glRasterPos2f(aux.x, (-largo/2.0)+anchoCubo/2.0+10);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, aux.tecla);
    glPushMatrix();
    glTranslatef(aux.x, (-largo/2.0)+anchoCubo/2.0+10, 0);
    glutWireCube(anchoCubo);
    glPopMatrix();
    cubos.pop();
    cubos.push(aux);
  }
  acierto = false;
  fallo = false;
}

void validarPresionado(char theKey){
  if (!cubos.empty()) {
    if (cubos.front().x-(anchoCubo/2.0) < anchoCubo/2.0) {
      if (cubos.front().tecla == theKey) {
        acierto = true;
        cubos.pop();
        cantCubos--;
      }
    }
  }
}

// Toma la ubicaci�n del proyecto
void getParentPath(){
  for (int i = fullPath.length()-1; i>=0 && fullPath[i] != '\\'; i--) {
    fullPath.erase(i,1);
  }
}

// Reshape
void reshape(int ancho, int largo)
{
  glViewport(0.0,0.0,(GLdouble) ancho,(GLdouble)  largo);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-ancho/2, ancho/2, -largo/2, largo/2, 1, 150);
  gluLookAt(0, 0, 15, 0, 0, 0, 0, 1,0);
}

// Display
static void myDisplay(void)
{
  glClearColor(1,1,1,0);                                  // Color del background

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Activa profundidad
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_TEXTURE_2D);

  if(jugando){
    pantallaJugando();
  }
  else if(autores){
    pantallaAutores();                                  // Despliega pantalla autores
  }
  else if(instrucciones){
    pantallaInstrucciones();                            // Despliega pantalla instrucciones
  }
  else if(historia){
    pantallaHistoria();
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

  // Objetos 3D
  // Ana
  // // C:/Users/karen_000/Dropbox/6 Semestre/Gr�ficas/MundoDeVerminara3D/Windows/imagenes/beer/beer.obj
  //string ruta = fullPath + "imagenes/hamburger/hamburger.obj";
  // Iker
  //string ruta = "/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/hamburger/hamburger.obj";
/*
  model[0]= *glmReadOBJ(ruta.c_str());
  glmUnitize(&model[0]);
  glmVertexNormals(&model[0],90.0,GL_TRUE);*/
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
  if (jugando) {
    validarPresionado(theKey);
  }
  switch (theKey)
  {
    case 'a':                                                   // Autores
    case 'A':
      if(inicio and !jugando and !pausado and !terminado and !instrucciones){
        autores = !autores;
      }
      break;
    case 'i':                                                   // Instrucciones
    case 'I':
      if(inicio and !jugando and !pausado and !terminado and !autores and !historia){
        instrucciones = !instrucciones;
      }
      break;
    case 'h':                                                   // Historia
    case 'H':
      if(inicio and !jugando and !pausado and !terminado and !autores and !instrucciones){
        historia = !historia;
      }
      break;
    case 'j':
    case 'J':
      if(inicio and !historia and !pausado and !terminado and !autores and !instrucciones){
        jugando = !jugando;
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
