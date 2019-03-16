#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "texto.h"

#define ORTOGONAL 1
#define PERSPECTIVA -1
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

float anguloDeRotacao = 0;
char ladoQueEstaMostrando[20];
int projecao = ORTOGONAL;


int tempoNoFrame = 0;
int momentoAnterior;
float fps = 0;


// Rotina de desenho
void desenhaMinhaCena() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glPushMatrix();
        glRotatef(anguloDeRotacao, 1, 0, 0);

        glPolygonMode(GL_FRONT, GL_FILL);  // Lado de trás: preenchido
        glPolygonMode(GL_BACK, GL_LINE); // Da frente: contorno

        // Desenha um quadrado de lado 60
        glBegin(GL_TRIANGLE_FAN);
            glVertex2i(-30.0, -30.0);
            glVertex2i( 30.0, -30.0);
            glVertex2i( 30.0,  30.0);
            glVertex2i(-30.0,  30.0);
        glEnd();

    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    escreveTexto(ladoQueEstaMostrando, 20, 20);


    // calcula quantos quadros por segundo está chamando a idle
    fps = 1000.0f / MAX(delta, 1);
    momentoAnterior = momentoAtual;

    glutSwapBuffers();
}

// Inicia algumas variáveis de estado do OpenGL
void inicializa() {
    // define qual é a cor do fundo
    glClearColor(1, 1, 1, 1); // branco
}

void configuraProjecao() {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projecao == ORTOGONAL) {
        glOrtho(-50, 50, -50, 50, -100, 100);
    } else if (projecao == PERSPECTIVA) {
        gluPerspective(60, ((float)width/height), 1, 100);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -75);
}

void redimensionada(int width, int height) {
    // left, bottom, right, top
    glViewport(0, 0, width, height);
    configuraProjecao();
}

void teclaPressionada(unsigned char key, int x, int y) {
    // vê qual tecla foi pressionada
    switch(key)
    {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    case '+':
    case '=':
        anguloDeRotacao += 1;
        break;
    case '-':
    case '_':
        anguloDeRotacao -= 1;
        break;
    case 'p':
    case 'P':
        projecao *= -1;
        configuraProjecao();
        break;
    default:
        break;
    }
}

void atualiza() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    tempoNoFrame += momentoAtual - momentoAnterior;
    if (tempoNoFrame >= 30) {
        tempoNoFrame -= 30;
        anguloDeRotacao += 1;
        if ((int)floor((anguloDeRotacao + 90) / 180) % 2 == 0 && strcmp(ladoQueEstaMostrando, "frente") != 0) {
            strcpy(ladoQueEstaMostrando, "frente");
        } else if ((int)floor((anguloDeRotacao + 90) / 180) % 2 == 1 && strcmp(ladoQueEstaMostrando, "traz") != 0) {
            strcpy(ladoQueEstaMostrando, "traz");
        }

    }
    glutPostRedisplay();
}

char tituloDaJanela[100];
void atualizaFPS(int idx) {
    sprintf(tituloDaJanela, "Orientacao de Poligonos (%.2f)", fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(1000, atualizaFPS, 0);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Orientacao de Poligonos");

    // Registra callbacks para eventos
    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutIdleFunc(atualiza);
    glutTimerFunc(0, atualizaFPS, 0);

    // Configura valor inicial de algumas
    // variáveis de estado do OpenGL
    inicializa();

    glutMainLoop();
    return 0;
}
