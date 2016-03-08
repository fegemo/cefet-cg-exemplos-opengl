#include <GL/glew.h>
#include <GL/freeglut.h>

float tomCinza = .3;
float incrementoNoTom = .01f;

void desenhaCena(void)
{
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Come�a a usar a cor amarela
    glColor3f(tomCinza, tomCinza, tomCinza);

    // Come�a a desenhar um pol�gono com os v�rtices especificados
    glBegin(GL_POLYGON);
        glVertex3f(20, 20, 0);
        glVertex3f(80, 20, 0);
        glVertex3f(80, 80, 0);
        glVertex3f(20, 80, 0);
    glEnd();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glFlush();
}

// Inicia algumas vari�veis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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

void mudaCor(int idx) {
    // altera a cor do quadrado
    tomCinza += incrementoNoTom;
    if (tomCinza > .8 || tomCinza < .3) {
        incrementoNoTom = -incrementoNoTom;
    }

    // Pede ao GLUT para redesenhar a tela, assim que poss�vel
    glutPostRedisplay ();

    // Se registra novamente, para que fique sempre sendo chamada
    glutTimerFunc(33, mudaCor, 0);
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configura��o inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("50 tons de cinza");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    // Registra a fun��o "mudaCor" para executar daqui a 33 milissegundos
    glutTimerFunc(33, mudaCor, 0);

    glutMainLoop();
    return 0;
}
