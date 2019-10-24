#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>

void escreveTexto(char *s, float x, float y, float z) {
    glRasterPos3f(x, y, z);

    for (int i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
    }
}
