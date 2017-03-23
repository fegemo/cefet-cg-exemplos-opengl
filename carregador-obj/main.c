#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#include "modelo.h"

struct modelo *modelo;
float rotacaoCena = 0;
vetor inicioMovimentoMouse;

// Rotina de desenho
void desenhaMinhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha um polígono por seus vértices
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(0, 0, -2);
        glScalef(0.25f, 0.25f, 0.25f);
        glRotatef(rotacaoCena, 0, 1, 0);

        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor3f(0.0, 0.67f, 0.0);
        desenhaModelo(modelo);

        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);
        glColor3f(0.2f, 0.35f, 0.2f);
        desenhaModelo(modelo);
    glPopMatrix();

    glutSwapBuffers();
}

// Inicia algumas variáveis de estado do OpenGL
void setup()
{
    // define qual é a cor do fundo
    glClearColor(1.0, 1.0, 1.0, 0.0); // branco
    glCullFace(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    modelo = carregaModelo("arvore.obj");

    ponto posicaoDaLuz = (ponto){ { 3.5f, 1, 3 } };
    vetor corDaLuz = (vetor){ {1.0, 1.0, 1.0} };
    glShadeModel(GL_FLAT);

    glLightfv(GL_LIGHT0, GL_POSITION, posicaoDaLuz.v);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corDaLuz.v);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corDaLuz.v);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 2.5f);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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

void movimentoMouse(int x, int y) {
    int deslocX = x - inicioMovimentoMouse.v[0];

    rotacaoCena += ((float)deslocX)/glutGet(GLUT_WINDOW_WIDTH) * 2 * 180.0f;
    inicioMovimentoMouse.v[0] = x;
    inicioMovimentoMouse.v[1] = y;
    glutPostRedisplay();
}

void cliqueMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // pressionou o botão... começar a rotação
            inicioMovimentoMouse.v[0] = x;
            inicioMovimentoMouse.v[1] = y;
        } else {
            // largou o botão... terminar rotação
        }
    }
}

// Função principal
int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   glutInitContextVersion(1, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(500, 500);

   glutCreateWindow("Quadrado");

   // Registra callbacks para eventos
   glutDisplayFunc(desenhaMinhaCena);
   glutReshapeFunc(redimensionada);
   glutKeyboardFunc(teclaPressionada);
   glutMotionFunc(movimentoMouse);
   glutMouseFunc(cliqueMouse);

   // Configura valor inicial de algumas
   // variáveis de estado do OpenGL
   setup();

   glutMainLoop();
   return 0;
}
