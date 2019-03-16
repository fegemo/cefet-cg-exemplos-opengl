#include <GL/glew.h>
#include <GL/freeglut.h>


// estruturas e variáveis globais da cena
struct ponto {
    float x, y;
};
float tomCinza = .3;
float incrementoNoTom = .01f;

#define QUANTIDADE_QUADRADOS 1000
struct ponto mouse;
struct ponto quadrado[QUANTIDADE_QUADRADOS];


// variáveis sobre as janelas
enum janelas {
    SINGLE,
    DOUBLE,
    QUANTIDADE_JANELAS
};
int idJanela[QUANTIDADE_JANELAS];


// início das callbacks
void desenhaCena() {
    // Limpa a tela (com a cor definida por glClearColor(r,g,b)) para que
    // possamos desenhar
    glClear(GL_COLOR_BUFFER_BIT);

    // Começa a usar a cor amarela
    glColor3f(tomCinza, tomCinza, tomCinza);

    // Começa a desenhar um polígono com os vértices especificados
    for (int c = 0; c < QUANTIDADE_QUADRADOS; c++) {
        glPushMatrix();
            glTranslatef(quadrado[c].x, quadrado[c].y, 0);
            glBegin(GL_TRIANGLE_FAN);
                glVertex3f(-20, -20, 0);
                glVertex3f( 20, -20, 0);
                glVertex3f( 20,  20, 0);
                glVertex3f(-20,  20, 0);
            glEnd();
        glPopMatrix();
    }
}

void desenhaCenaSingle() {
    desenhaCena();
    glFlush();
}

void desenhaCenaDouble() {
    desenhaCena();
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

void movimentoMouse(int x, int y) {
    mouse.x = x;
    mouse.y = y;
}

void segueMouse() {
    for (int c = 0; c < QUANTIDADE_QUADRADOS; c++) {
        float porcaoDistanciaPercorrida = c / 100.0 * 40.0 + 10;
        quadrado[c].x += (mouse.x - quadrado[c].x) / c;
        quadrado[c].y += (mouse.y - quadrado[c].y) / c;
    }
}

void mudaCor() {
    // altera a cor do quadrado
    tomCinza += incrementoNoTom;
    if (tomCinza > .8 || tomCinza < .3) {
        incrementoNoTom = -incrementoNoTom;
    }
}

void atualizaCena(int periodo) {
    segueMouse();
    mudaCor();

    // Se registra novamente, para que fique sempre sendo chamada
    glutTimerFunc(periodo, atualizaCena, periodo);

    // Pede ao GLUT para redesenhar a tela, assim que possível
    for (int i = 0; i < QUANTIDADE_JANELAS; i++) {
        glutSetWindow(idJanela[i]);
        glutPostRedisplay();
    }
}

// Rotina principal
int main(int argc, char **argv) {
    int tamanho = 900;
    // quadrado.x = 0;
    // quadrado.y = 0;

    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(tamanho, tamanho);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    idJanela[SINGLE] = glutCreateWindow("50 tons de cinza - single");
    glutPassiveMotionFunc(movimentoMouse);
    glutDisplayFunc(desenhaCenaSingle);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    inicializa();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(tamanho + 100, 0);
    idJanela[DOUBLE] = glutCreateWindow("50 tons de cinza - double");
    glutPassiveMotionFunc(movimentoMouse);
    glutDisplayFunc(desenhaCenaDouble);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    inicializa();

    glutTimerFunc(0, atualizaCena, 33);
    glutMainLoop();
    return 0;
}
