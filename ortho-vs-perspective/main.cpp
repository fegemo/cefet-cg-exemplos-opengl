#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float angulo = 0;

// Clears the window and draws the tetrahedron.  The tetrahedron is  easily
// specified with a triangle strip, though the specification really isn't very
// easy to read.
void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a white grid "floor" for the tetrahedron to sit on.
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
            glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
            glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
        }
    glEnd();

    // Draw the tetrahedron.  It is a four sided figure, so when defining it
    // with a triangle strip we have to repeat the last two vertices.
    glPushMatrix();
        glRotatef(angulo, .5, .5, .5);
        glBegin(GL_TRIANGLE_STRIP);
            glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
            glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
            glColor3f(0, 1, 0); glVertex3f(1, 0, 1);
            glColor3f(0, 0, 1); glVertex3f(0, 0, -1.4);
            glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
            glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
        glEnd();
    glPopMatrix();

    glFlush();
}

// Sets up global attributes like clear color and drawing color, enables and
// initializes any needed modes (in this case we want backfaces culled), and
// sets up the desired projection and modelview matrices. It is cleaner to
// define these operations in a function separate from main().
void init() {

  // Set the current clear color to sky blue and the current drawing color to
  // white.
  glClearColor(0.1, 0.39, 0.88, 1.0);
  glColor3f(1.0, 1.0, 1.0);

  // Tell the rendering engine not to draw backfaces.  Without this code,
  // all four faces of the tetrahedron would be drawn and it is possible
  // that faces farther away could be drawn after nearer to the viewer.
  // Since there is only one closed polyhedron in the whole scene,
  // eliminating the drawing of backfaces gives us the realism we need.
  // THIS DOES NOT WORK IN GENERAL.
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Set the camera lens so that we have a perspective viewing volume whose
  // horizontal bounds at the near clipping plane are -2..2 and vertical
  // bounds are -1.5..1.5.  The near clipping plane is 1 unit from the camera
  // and the far clipping plane is 40 units away.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-2, 2, -1.5, 1.5, 1, 40);

  // Set up transforms so that the tetrahedron which is defined right at
  // the origin will be rotated and moved into the view volume.  First we
  // rotate 70 degrees around y so we can see a lot of the left side.
  // Then we rotate 50 degrees around x to "drop" the top of the pyramid
  // down a bit.  Then we move the object back 3 units "into the screen".
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -3);
  glRotatef(50, 1, 0, 0);
  glRotatef(70, 0, 1, 0);
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void projecaoOrtogonal(int w, int h) {
    //glOrtho(-1, 1, -1, 1, -1, 1);
}

void projecaoPerspectiva(int w, int h) {
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    projecaoOrtogonal(w, h);
    projecaoPerspectiva(w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void atualiza(int i) {
    // Atualiza ângulo de rotação
    angulo += 1;

    // Diz ao GLUT para redesenhar assim que der
    glutPostRedisplay();

    // Registra a função de atualização de lógica novamente (~60 fps)
    glutTimerFunc(17, atualiza, 0);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 600);
  glutCreateWindow("A Simple Tetrahedron");
  glutDisplayFunc(desenha);
  glutReshapeFunc(redimensiona);
  glutKeyboardFunc(teclado);
  glutTimerFunc(0, atualiza, 0);
  init();
  glutMainLoop();
}
