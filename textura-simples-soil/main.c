#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <SOIL.h>

GLuint texturaMario;

void init(void)
{
    glClearColor (1, 1, 1, 0);

    texturaMario = SOIL_load_OGL_texture(
        "mario.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

    if (texturaMario == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }
}


void desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, texturaMario);
    glBegin(GL_QUADS);
        // Associamos um canto da textura para cada vértice
        glTexCoord2f(0, 0); glVertex3f(-1, -1,  0);
        glTexCoord2f(1, 0); glVertex3f( 1, -1,  0);
        glTexCoord2f(1, 1); glVertex3f( 1,  1,  0);
        glTexCoord2f(0, 1); glVertex3f(-1,  1,  0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void redimensiona(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho (-2, 2, -2, 2, -1.0, 1.0);
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
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(400, 400);
   glutInitWindowPosition(100, 100);

   glutCreateWindow(argv[0]);
   init();

   glutReshapeFunc(redimensiona);
   glutKeyboardFunc(teclado);
   glutDisplayFunc(desenha);
   glutMainLoop();
   return 0;
}
