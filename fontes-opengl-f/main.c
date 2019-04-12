#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>

// Matriz de pixels que descreve a letra "F".
// Cada item do array é um byte
GLubyte matrizDePixels[24] = {
   0xc0, 0x00,
   0xc0, 0x00,
   0xc0, 0x00,
   0xc0, 0x00,
   0xc0, 0x00,
   0xff, 0x00,
   0xff, 0x00,
   0xc0, 0x00,
   0xc0, 0x00,
   0xc0, 0x00,
   0xff, 0xc0,
   0xff, 0xc0
};

void inicializa() {
    // Cor de fundo: branco
    glClearColor (1, 1, 1, 0);
    // Diz ao OpenGL que vamos definir o bitmap da letra de 1 em 1 byte
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void desenha() {
    // Limpa a tela
    glClear(GL_COLOR_BUFFER_BIT);

    // Define a cor de pintura como verde
    glColor3f(.5, 1, .3);

    // Define a posição em que vamos começar a desenhar
    glRasterPos2i(20, 20);
    // Desenha o "F" que está representado na variável matrizDePixels
    // Após desenhar, move para a direita 11 pixels
    glBitmap(10, 12, 0, 0, 11, 0, matrizDePixels);
    glBitmap(10, 12, 0, 0, 11, 0, matrizDePixels);
    glBitmap(10, 12, 0, 0, 11, 0, matrizDePixels);

    glFlush();
}

void redimensiona(int w, int h) {
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho (0, w, 0, h, -1.0, 1.0);

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
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(100, 100);
   glutInitWindowPosition(100, 100);

   glutCreateWindow("F");
   inicializa();

   glutReshapeFunc(redimensiona);
   glutKeyboardFunc(teclado);
   glutDisplayFunc(desenha);
   glutMainLoop();

   return 0;
}
