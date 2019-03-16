#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

GLfloat anguloDeRotacao = 0;
GLfloat incrementoAngulo = 0.5;

void desenhaCena(void)
{
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor azul
    glColor3f(.5, .5, 1);

    glPushMatrix();
        // Multiplica a matriz corrente (topo da stack MODELVIEW) pela matriz de rotação
        glRotatef(anguloDeRotacao, 0, 0, 1);

        // Começa a desenhar um polígono com os vértices especificados
        glBegin(GL_POLYGON);
            glVertex2f(-30, -30);
            glVertex2f( 30, -30);
            glVertex2f( 30,  30);
            glVertex2f(-30,  30);
        glEnd();
    glPopMatrix();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa(void)
{
    // cor para limpar a tela
    glClearColor(0, 0, 0, 0);      // preto

    printf("Comandos:\n---------\n\n");
    printf("\t+  Aumenta o incremento no angulo\n");
    printf("\t-  Diminui o incremento no angulo\n\n");
}

// Callback de redimensionamento
void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-100, 100, -100, 100, -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void atualiza(int idx) {
    anguloDeRotacao += incrementoAngulo;

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
