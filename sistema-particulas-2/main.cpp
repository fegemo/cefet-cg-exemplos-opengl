#include <SOIL.h>
#include <gl/freeglut.h>
#include <math.h>
#include "vector"
#include "sistemaparticulas.h"

SistemaParticulas* sp;

void setup() {
    glClearColor(0, 0, 0, 0);

    sp = new SistemaParticulas(Vector3(0, 0, 0), 200);
}

void teclado(unsigned char key, int x, int y) {
    switch(key) {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void desenhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    sp->desenha();

    glutSwapBuffers();
}

void atualiza() {
    sp->atualiza();
    glutPostRedisplay();
}

void timer(int i) {
    atualiza();
    glutTimerFunc(33, timer, 0);
}

void redimensiona(int larg, int alt) {
    float razaoAspecto = larg/((float)alt);
    glViewport(0, 0, larg, alt);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50*(razaoAspecto/2), 50*(razaoAspecto/2), -50, 50, -50, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500,350);
    glutCreateWindow("Sistema de Particulas");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    //glutIdleFunc(atualiza);
    glutTimerFunc(0, timer, 0);
    setup();

    glutMainLoop();
    return 0;
}
