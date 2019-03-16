#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LADOS 30

void inicializa() {
    glClearColor(1, 1, 1, 1);

    // habilita o teste de profundidade para que o OpenGL use as coordenadas
    // z dos objetos para determinar visibilidade (se não habilitar, OpenGL
    // simplesmente ignora as coordenadas z)
    // glEnable(GL_DEPTH_TEST);
}

void desenhaCirculo(float raio, float x, float y, float z) {
    float angulo;

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, z);
        for (int i = 0; i <= MAX_LADOS; ++i) {
            angulo = 2 * M_PI * i / MAX_LADOS;
            glVertex3f(x + cos(angulo) * raio, y + sin(angulo) * raio, z);
        }
    glEnd();
}

// LEMBRETE: OpenGL usa **regra da mão direita** na definição dos objetos,
//           ou seja, Z cresce para fora da tela.
//
void desenhaCena() {
    // glClear(GL_COLOR_BUFFER_BIT | GLUT_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    // Disco vermelho (maior)
    glColor3f (1, 0, 0);
    desenhaCirculo(100, 200, 150, 0);

    // Disco branco (menor)
    glColor3f (1, 1, 1);
    desenhaCirculo(60, 200, 150, 0);
    // desenhaCirculo(60, 200, 150, 1);   // z mais próximo do usuário

    glFlush();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
   }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Discos");
    inicializa();

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenhaCena);

    glutMainLoop();
    return 0;
}
