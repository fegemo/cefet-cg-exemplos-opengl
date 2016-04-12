#include <GL/freeglut.h>

int anguloOmbro = 0, anguloCutuvelo = 0;

void inicializa()
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
}

void desenha()
{
   glClear (GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glTranslatef (-1.0, 0.0, 0.0);
   glRotatef ((GLfloat) anguloOmbro, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.4, 1.0);
   glutWireCube (1.0);
   glPopMatrix();

   glTranslatef (1.0, 0.0, 0.0);
   glRotatef ((GLfloat) anguloCutuvelo, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   glPushMatrix();
   glScalef (2.0, 0.4, 1.0);
   glutWireCube (1.0);
   glPopMatrix();

   glPopMatrix();
   glutSwapBuffers();
}

void redimensiona(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void teclado(unsigned char key, int x, int y)
{
   switch (key) {
      // Tecla ESC
      case 27:
          exit(0);
          break;
      case 'o':
         anguloOmbro = (anguloOmbro + 5) % 360;
         glutPostRedisplay();
         break;
      case 'O':
         anguloOmbro = (anguloOmbro - 5) % 360;
         glutPostRedisplay();
         break;
      case 'c':
         anguloCutuvelo = (anguloCutuvelo + 5) % 360;
         glutPostRedisplay();
         break;
      case 'C':
         anguloCutuvelo = (anguloCutuvelo - 5) % 360;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Braço do Robô");
   inicializa();

   glutDisplayFunc(desenha);
   glutReshapeFunc(redimensiona);
   glutKeyboardFunc(teclado);

   glutMainLoop();
   return 0;
}
