#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LADOS 30

void init(void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);
}

void desenhaDisco(float raio, float x, float y, float z) {
    float angulo;
    int i;

    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(x, y, z);
        for (i = 0; i <= MAX_LADOS; ++i) {
            angulo = 2 * M_PI * i / MAX_LADOS;
            glVertex3f(x + cos(angulo) * raio, y + sin(angulo) * raio, z);
        }
    glEnd();
}

void desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Mesmo com valores 'Z' diferentes, o disco branco não aparece
    // até que ativemos o teste de profundidade do OpenGL
    // Descomente a linha abaixo para ativá-lo
//    glEnable(GL_DEPTH_TEST);


    // Disco branco (menor)
    glColor3f (1, 1, 1);
    desenhaDisco(60, 200, 150, 1);  // coordenada z "trazendo pra frente"

    // Disco vermelho (maior)
    glColor3f (1, 0, 0);
    desenhaDisco(100, 200, 150, 0);

    glDisable(GL_DEPTH_TEST);
    glFlush();
}

void redimensiona(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
   }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Discos");
    init();

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);

    glutMainLoop();
    return 0;
}
