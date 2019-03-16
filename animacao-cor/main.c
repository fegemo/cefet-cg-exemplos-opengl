#include <GL/glew.h>
#include <GL/freeglut.h>

float tomCinza = .3;
float incrementoNoTom = .01f;

void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor amarela
    glColor3f(tomCinza, tomCinza, tomCinza);

    // Começa a desenhar um polígono com os vértices especificados
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(20, 20, 0);
        glVertex3f(80, 20, 0);
        glVertex3f(80, 80, 0);
        glVertex3f(20, 80, 0);
    glEnd();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Inicia algumas variáveis de estado
void inicializa() {
    // cor para limpar a tela
    glClearColor(1, 1, 1, 1);      // branco
}

// Callback de redimensionamento
void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1);

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

void mudaCor(int periodo) {
    // altera a cor do quadrado
    tomCinza += incrementoNoTom;
    if (tomCinza > .8 || tomCinza < .3) {
        incrementoNoTom = -incrementoNoTom;
    }

    // Pede ao GLUT para redesenhar a tela, assim que poss�vel
    glutPostRedisplay();

    // Se registra novamente, para que fique sempre sendo chamada
    glutTimerFunc(periodo, mudaCor, periodo);
}

// Rotina principal
int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("50 tons de cinza");
    inicializa();

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    // Registra a função "mudaCor" para executar daqui a 0 ms
    glutTimerFunc(0, mudaCor, 33);

    glutMainLoop();
    return 0;
}
