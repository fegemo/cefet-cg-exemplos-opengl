#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>

void escreveTexto(char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, s[i]);
    }
}
