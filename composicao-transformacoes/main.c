#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

// Para usar random()
#include <time.h>
#include <stdlib.h>

GLfloat anguloDeRotacao = 0;
GLfloat incrementoAngulo = 15;
GLfloat incrementoPosicao = 0.25;

struct shuriken {
    float x, y;
};

const int LARGURA_ESTRELA = 15;
const int ALTURA_ESTRELA = 15;
const int TOTAL_ESTRELAS = 20;
struct shuriken* estrelinhasNinja;

inicializaEstrelinhasNinja() {
    int i;
    int larguraDaTela = glutGet(GLUT_WINDOW_WIDTH);
    int alturaDaTela = glutGet(GLUT_WINDOW_HEIGHT);
    estrelinhasNinja = malloc(TOTAL_ESTRELAS * sizeof(struct shuriken));

    for (i = 0; i < TOTAL_ESTRELAS; i++) {
      estrelinhasNinja[i].x = rand() % larguraDaTela;
      estrelinhasNinja[i].y = -(rand() % alturaDaTela/2);
    }
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    glClearColor(0, 0, 0, 0);

    // inicia o "random power"
    srand(time(NULL));

    // inicia as estrelinhas
    inicializaEstrelinhasNinja();

    printf("Comandos:\n---------\n\n");
    printf("\t+  Aumenta o incremento no angulo\n");
    printf("\t-  Diminui o incremento no angulo\n\n");
}

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Cor azul, tonalidade: azulDaCorDoMilho
    glColor3f(.5, .5, 1);

    int i;
    for (i = 0; i < TOTAL_ESTRELAS; i++) {
        glPushMatrix();
            // carrega uma Matriz Identidade, só pra garantir
            glLoadIdentity();

            // translada o sistema de coordenadas para (x,y)
            glTranslatef(estrelinhasNinja[i].x, estrelinhasNinja[i].y, 0);
            // rotaciona o sistema de coordenadas no próprio lugar
            glRotatef(anguloDeRotacao, 0, 0, 1);

            glBegin(GL_POLYGON);
                glVertex2f(-LARGURA_ESTRELA/2, -LARGURA_ESTRELA/2);
                glVertex2f( LARGURA_ESTRELA/2, -LARGURA_ESTRELA/2);
                glVertex2f( LARGURA_ESTRELA/2,  LARGURA_ESTRELA/2);
                glVertex2f(-LARGURA_ESTRELA/2,  LARGURA_ESTRELA/2);
            glEnd();
        glPopMatrix();
    }

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
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

void atualiza(int idx) {
    anguloDeRotacao += incrementoAngulo;

    int i;
    for (i = 0; i < TOTAL_ESTRELAS; i++) {
      estrelinhasNinja[i].y += incrementoPosicao;
    }

    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
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
        case '+':
            incrementoAngulo += 0.5;
            break;
        case '-':
            incrementoAngulo -= 0.5;
            break;
        default:
            break;
    }
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("Quadrado com transformação de rotação");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
