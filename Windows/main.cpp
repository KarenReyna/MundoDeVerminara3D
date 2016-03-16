/*
 *   Ana Karen Reyna Rivas   A01280310
 *   Iker Arbulu Lozano      A01190690
 *   Segundo Avance: Proyecto El Mundo de Verminara
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

using namespace std;

int	ancho = 600;
int largo = 600;
int iSegundos = 0;                                      // 9000000 = 10min
bool juegoInicio = false;
bool ayuda = true;
bool contar;

bool inicio = true, pausado, comenzado, reiniciar, terminado, jugando;
bool autores, instrucciones,historia;

// Para objetos 3D
GLuint texName[36];                                     // Texturas
GLMmodel model[6];
string fullPath = __FILE__;
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };      //Puntual

int angulo=-1;                                          // Animación de objetos

// Timer para que los objetos roten
static void timer(int i){
    angulo += 10;
    glutTimerFunc(80, timer, 1);
    glutPostRedisplay();
}

static void estrellas(){
    glmDraw(&model[0],GLM_COLOR|GLM_FLAT);
}

// Desplegar texto en tamaño pequeño
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

// Desplegado en pantalla principal
static void pantallaInicial(){
    // Botón Autores
    glPushMatrix();
        glTranslatef(-((ancho/2)-50),(largo/4)+80,-45);
        glRotatef(angulo,0,1,0);
        glScalef(0.8,1,1);
        glTranslatef(-35,0,0);
        dibujaBaseAutores();
        glTranslatef(0,0,20);
        letreroLetraPequena("A - Autores");
    glPopMatrix();

    // Titulo
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(-180,220,-45);
        glRotatef(-30,1,1,0);
        glScalef(2.5,1,2);
        letrero("El Mundo");
        glTranslatef(45,0,-50);
        letrero("de Verminara");
    glPopMatrix();

    // Separador
    glPushMatrix();
        glColor3f(0.098,0.098,0.439);
        glTranslatef(0,0,2);
        glRotatef(1,0,0,1.5);
        glLineWidth(1);
        glTranslatef(0,0,1);
        for(int i=0;i<3;i++){
            glBegin(GL_LINES);
            glVertex2f(-(ancho/2),((largo/4)-(1.5*i)));
            glVertex2f((ancho/2),((largo/4 + 20)-(1.5*i)));
            glEnd();
        }
    glPopMatrix();

    /* Estrellas
    glPushMatrix();
        glTranslatef((ancho/4),100,-45);
        glRotatef(95,1,1,0);
        glScalef(20,20,15);
        estrellas();
    glPopMatrix();*/

    // Instrucciones
    glPushMatrix();
        glColor3f(0,1,0);
        glTranslatef(-320,0,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("I - Instrucciones");
    glPopMatrix();

    // Historia
    glPushMatrix();
        glColor3f(1,0.27,0);
        glTranslatef(-320,-50,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("H - Historia");
    glPopMatrix();

    // Comenzar partida
    glPushMatrix();
        glColor3f(0,0,0.545);
        glTranslatef(-320,-100,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("C - Comenzar");
    glPopMatrix();

    // Pausar partida
    glPushMatrix();
        glColor3f(0.501, 0, 0);
        glTranslatef(-320,-150,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("P - Pausar");
    glPopMatrix();

    // Reiniciar partida
    glPushMatrix();
        glColor3f(1,0,1);
        glTranslatef(-320,-200,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("R - Reiniciar");
    glPopMatrix();

    // Quitar música
    glPushMatrix();
        glColor3f(0,0.749,1);
        glTranslatef(-320,-250,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("M - Quitar musica");
    glPopMatrix();

    // Ver Niveles
    glPushMatrix();
        glColor3f(1,0.843,0);
        glTranslatef(-320,-300,-45);
        glRotatef(-30,1,1,0);
        glPushMatrix();
            glScalef(2.5,1,2);
            glRectf(20, 50, 90, 90);
        glPopMatrix();
        glTranslatef(0,0,20);
        letreroMenu("N - Ver Niveles");
    glPopMatrix();
}

// Despliega pantalla de autores
static void pantallaAutores(){
    // Titulo
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(-180,220,-45);
        glRotatef(-30,1,1,0);
        glScalef(2.5,1,2);
        letrero("Autores");
    glPopMatrix();

    // Separador
    glPushMatrix();
        glColor3f(0.098,0.098,0.439);
        glTranslatef(0,0,2);
        glRotatef(1,0,0,1.5);
        glLineWidth(1);
        glTranslatef(0,0,1);
        for(int i=0;i<3;i++){
            glBegin(GL_LINES);
            glVertex2f(-(ancho/2),((largo/4)-(1.5*i)));
            glVertex2f((ancho/2),((largo/4 + 20)-(1.5*i)));
            glEnd();
        }
    glPopMatrix();

    char nombre1[200]="";
    char nombre2[200]="";
    sprintf(nombre1, "%s", "Ana Karen Reyna Rivas   Matricula: A01280310");
    sprintf(nombre2, "%s", "Iker Arbulu Lozano   Matricula: A01190690");
    int xRaster = 5;
    // Dibuja autores
    glRasterPos2i(-ancho * .4, largo *.05);
    for (GLint k = 0; nombre1[k]!='\0'; k++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, nombre1[k]);
    }
    glRasterPos2i(-ancho * .4, largo *-.05);
    for (GLint k = 0; nombre2[k]!='\0'; k++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, nombre2[k]);
    }

    // Botón Quitar autores
    glPushMatrix();
        glTranslatef(0,-150,-45);
        glRotatef(angulo,0,1,0);
        glScalef(0.8,1,1);
        dibujaBaseAutores();
        glTranslatef(0,0,20);
        letreroLetraPequena("A - Home");
    glPopMatrix();
}

// Despliega pantalla de instrucciones
static void pantallaInstrucciones(){
    // Titulo
    glPushMatrix();
        glColor3f(0,0,0);
        glTranslatef(-180,220,-100);
        glRotatef(-30,1,1,0);
        glScalef(2.5,1,2);
        letrero("Instrucciones");
    glPopMatrix();

    // Separador
    glPushMatrix();
        glColor3f(0.098,0.098,0.439);
        glTranslatef(0,0,2);
        glRotatef(1,0,0,1.5);
        glLineWidth(1);
        glTranslatef(0,0,1);
        for(int i=0;i<3;i++){
            glBegin(GL_LINES);
            glVertex2f(-(ancho/2),((largo/4)-(1.5*i)));
            glVertex2f((ancho/2),((largo/4 + 20)-(1.5*i)));
            glEnd();
        }
    glPopMatrix();

    // Texto
  glPushMatrix();
    glColor3f(0, 0, 0);
    char lineaInstruccion[200]="";
    float yRaster = largo*.17;
    sprintf(lineaInstruccion, "%s", "El juego consiste de varios retos que debes superar para mantener la");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "salud de tu avatar, al ir cumpliendo con los retos que te pone el");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "mundo de verminara la velocidad del juego ira avanzando.");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "Una vez cumpliendo cada reto iras subiendo de nivel que se sumara a");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "tu puntaje final.");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "Cada juego tendra las instrucciones a seguir en la parte baja de la");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    sprintf(lineaInstruccion, "%s", "pantalla, demuestra quien es el mejor dominando el mundo de verminara.");
    glRasterPos2f(-ancho * .5, yRaster);
    for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
    }
    yRaster-=25;
    //
  glPopMatrix();

}

// Despliega pantalla de historia
static void pantallaHistoria(){
  // Titulo
  glPushMatrix();
  glColor3f(0,0,0);
  glTranslatef(-180,220,-100);
  glRotatef(-30,1,1,0);
  glScalef(2.5,1,2);
  letrero("Historia");
  glPopMatrix();

  // Separador
  glPushMatrix();
  glColor3f(0.098,0.098,0.439);
  glTranslatef(0,0,2);
  glRotatef(1,0,0,1.5);
  glLineWidth(1);
  glTranslatef(0,0,1);
  for(int i=0;i<3;i++){
    glBegin(GL_LINES);
    glVertex2f(-(ancho/2),((largo/4)-(1.5*i)));
    glVertex2f((ancho/2),((largo/4 + 20)-(1.5*i)));
    glEnd();
  }
  glPopMatrix();

  // Texto
  glPushMatrix();
  glColor3f(0, 0, 0);
  char lineaInstruccion[200]="";
  float yRaster = largo*.17;
  sprintf(lineaInstruccion, "%s", "Verminara es un mundo que cuenta con ciudadanos irresponsables y ");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "necesita de la ayuda de un conocedor responsable para poder ");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "rescatarlos de su fallas, ahi es donde entra el jugador.");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "El jugador viene a ayudar con diversos problemas del dia a");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "dia a los habitantes que pueden ser resueltos por simples");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "acciones. Para esto el jugador debe de contar con su");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "habilidad para rapidamente ayudar a todos los habitantes");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  sprintf(lineaInstruccion, "%s", "de Verminara.");
  glRasterPos2f(-ancho * .5, yRaster);
  for (GLint k = 0; lineaInstruccion[k]!='\0'; k++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, lineaInstruccion[k]);
  }
  yRaster-=25;
  //
  glPopMatrix();

}

// Toma la ubicación del proyecto
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

    if(jugando){

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
    getParentPath();
    glEnable(GL_NORMALIZE);

    // Objetos 3D
    // Ana
    string ruta = fullPath + "imagenes/beer.obj";
    // Iker
    // string ruta = "/Users/ikerarbululozano/Google Drive/Noveno Semestre/Graficas Computacionales/MundoDeVerminara3D/Mac/ProyectoFinalGraficas/ProyectoFinalGraficas/imagenes/beer.obj";

    model[0]= *glmReadOBJ(ruta.c_str());
    glmUnitize(&model[0]);
    glmVertexNormals(&model[0],90.0,GL_TRUE);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
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
    init();
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutTimerFunc(100,timer,1);
    glutMainLoop();
    return 0;
}
