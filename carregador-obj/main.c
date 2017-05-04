#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#include "modelo.h"

struct modelo *modelo;

void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    // Desenha um polígono por seus vértices
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(0, 0, -2);
        glScalef(0.25f, 0.25f, 0.25f);
        desenhaModelo(modelo);
    glPopMatrix();

    glFlush();
}

void setup() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // branco
    glCullFace(GL_NONE);
    modelo = carregaModelo("arvore.obj");
}

void redimensionada(int width, int height) {
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, ((float)width)/height, 0.1, 10);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Carregador de Objs");

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);

    setup();

    glutMainLoop();
    return 0;
}
