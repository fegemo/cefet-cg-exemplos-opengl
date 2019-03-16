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

void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor rosa
    glColor3f(1, 0, 0.5f);

    // Começa a desenhar um polígono com os vértices especificados
    // ATENÇÃO: esta não é a melhor forma para movimentar um objeto
    //          veremos na aula sobre "Transformações" a melhor forma
    //          basicamente, vamos "desenhar o objeto na origem" e transladá-lo para sua posição
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(posicaoQuadrado.x, posicaoQuadrado.y, 0);
        glVertex3f(posicaoQuadrado.x + tamanhoQuadrado.largura, posicaoQuadrado.y, 0);
        glVertex3f(posicaoQuadrado.x + tamanhoQuadrado.largura, posicaoQuadrado.y + tamanhoQuadrado.altura, 0);
        glVertex3f(posicaoQuadrado.x, posicaoQuadrado.y + tamanhoQuadrado.altura, 0);
    glEnd();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa() {
    // cor para limpar a tela
    glClearColor(1, 1, 1, 1);      // branco

    // define o quadrado
    posicaoQuadrado.x = 0;
    posicaoQuadrado.y = 0;
    tamanhoQuadrado.largura = 50;
    tamanhoQuadrado.altura = 50;
}

// Callback de redimensionamento
void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // repare que o sistema de coordenadas do mundo foi configurado para
    // ser bem parecido com o sistema da janela: 0 a w, 0 a h, começando
    // do canto esquerdo-superior
    glOrtho(0, w, h, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y) {
   switch(key) {
      // Tecla ESC
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Callback de evento de movimento do mouse
void movimentoMouse(int x, int y) {
    posicaoMouse.x = x;
    posicaoMouse.y = y;
}

// Callback do evento timer
void atualizaCena(int periodo) {
    // faz o quadrado andar na direção do ponteiro
    posicaoQuadrado.x += (posicaoMouse.x - posicaoQuadrado.x)/50.0f;
    posicaoQuadrado.y += (posicaoMouse.y - posicaoQuadrado.y)/50.0f;

    // Pede ao GLUT para redesenhar a tela, assim que possível
    glutPostRedisplay();

    // Se registra novamente, para que fique sempre sendo chamada (30 FPS)
    glutTimerFunc(periodo, atualizaCena, periodo);
}

// Rotina principal
int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Segue Mouse com Atraso");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    // Registra a função "movimentoMouse" para executar sempre que o mouse mexer
    glutPassiveMotionFunc(movimentoMouse);
    // Registra a função "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, atualizaCena, 33);

    glutMainLoop();
    return 0;
}
