#include <GL/freeglut.h>
#include <string.h>

void escreveTexto(char *texto, float x, float y) {
    int i;
    glRasterPos2f(x, y);

    for (i = 0; i < strlen(texto); i++) {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[i]);
    }
}
