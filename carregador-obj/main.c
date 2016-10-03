#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#include "modelo.h"

struct modelo *modelo;

// Rotina de desenho
void desenhaMinhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    // Desenha um polígono por seus vértices
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(0, 0, -2);
        glScalef(0.25f, 0.25f, 0.25f);
        // glCallList(modelo);
        desenhaModelo(modelo);
    glPopMatrix();

    glFlush();
}

// NOVIDADE: uma função que vamos chamar dentro
//    do "main"
// Inicia algumas variáveis de estado do OpenGL
void setup()
{
    // define qual é a cor do fundo
    glClearColor(1.0, 1.0, 1.0, 0.0); // branco
    glCullFace(GL_NONE);
    modelo = carregaModelo("arvore.obj");
}

void redimensionada(int width, int height)
{
   // left, bottom, right, top
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, ((float)width)/height, 0.1, 10);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// NOVIDADE: callback de "keyboard"
void teclaPressionada(unsigned char key, int x, int y)
{
    // vê qual tecla foi pressionada
    switch(key)
    {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    default:
        break;
    }
}

// Função principal
int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);

   glutCreateWindow("Quadrado");

   // Registra callbacks para eventos
   glutDisplayFunc(desenhaMinhaCena);
   glutReshapeFunc(redimensionada);
   glutKeyboardFunc(teclaPressionada);

   // Configura valor inicial de algumas
   // variáveis de estado do OpenGL
   setup();

   glutMainLoop();
   return 0;
}
