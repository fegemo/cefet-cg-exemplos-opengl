#include <GL/glew.h>
#include <GL/freeglut.h>

struct ponto {
    float x, y;
};
struct tamanho {
    float largura, altura;
};

struct ponto posicaoQuadrado;
struct tamanho tamanhoQuadrado;

struct ponto posicaoMouse;

void desenhaCena(void)
{
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Come�a a usar a cor amarela
    glColor3f(1, 0, 0.5f);

    // Come�a a desenhar um pol�gono com os v�rtices especificados
    // ATEN��O: esta n�o � a melhor forma para movimentar um objeto
    //          veremos na aula sobre "Transforma��es" a melhor forma
    //          basicamente, vamos "desenhar o objeto na origem" e translad�-lo para sua posi��o
    glBegin(GL_POLYGON);
        glVertex3f(posicaoQuadrado.x, posicaoQuadrado.y, 0);
        glVertex3f(posicaoQuadrado.x + tamanhoQuadrado.largura, posicaoQuadrado.y, 0);
        glVertex3f(posicaoQuadrado.x + tamanhoQuadrado.largura, posicaoQuadrado.y + tamanhoQuadrado.altura, 0);
        glVertex3f(posicaoQuadrado.x, posicaoQuadrado.y + tamanhoQuadrado.altura, 0);
    glEnd();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glFlush();
}

// Inicia algumas vari�veis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(1, 1, 1, 0);      // branco

    // define o quadrado
    posicaoQuadrado.x = 0;
    posicaoQuadrado.y = 0;
    tamanhoQuadrado.largura = 50;
    tamanhoQuadrado.altura = 50;
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
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

void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}

void atualiza(int indice) {
    // faz o quadrado andar na dire��o do ponteiro
    posicaoQuadrado.x += (posicaoMouse.x - posicaoQuadrado.x)/50.0f;
    posicaoQuadrado.y += (posicaoMouse.y - posicaoQuadrado.y)/50.0f;

    // Pede ao GLUT para redesenhar a tela, assim que poss�vel
    glutPostRedisplay ();

    // Se registra novamente, para que fique sempre sendo chamada (30 FPS)
    glutTimerFunc(33, atualiza, 0);
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configura��o inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Segue Mouse com Atraso");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    // Registra a fun��o "movimentoMouse" para executar sempre que o mouse mexer
    glutPassiveMotionFunc(movimentoMouse);
    // Registra a fun��o "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, atualiza, 0);

    glutMainLoop();
    return 0;
}
