#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "texto.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

struct Casinha {
    float x, y;
    float lado;
    float angulo;
};

struct Casinha casinha1, casinha2, casinha3;

void inicializaCasinhas() {
    casinha1.x = 40;
    casinha1.y = 10;
    casinha1.lado = 20;
    casinha1.angulo = 0;

    casinha2.x = 40;
    casinha2.y = 10;
    casinha2.lado = 20;
    casinha2.angulo = 0;

    casinha3.x = 50;
    casinha3.y = 20;
    casinha3.lado = 20;
    casinha3.angulo = 0;
}

// Inicia algumas variáveis de estado
void inicializa() {
    glClearColor(1, 1, 1, 1);
    glPointSize(9);
    
    // inicia as estrelinhas
    inicializaCasinhas();
}

void configuraViewport(int indice, int total) {
    int larguraJanela = glutGet(GLUT_WINDOW_WIDTH);
    int alturaJanela = glutGet(GLUT_WINDOW_HEIGHT);
    int larguraViewport = larguraJanela / total;
    int alturaViewport = larguraViewport;
    float viewportX = indice * larguraViewport;
    if (larguraJanela / ((float)alturaJanela) > 3) {
        larguraViewport = alturaJanela;
        alturaViewport = alturaJanela;
    }
    float viewportY = MAX(0, (alturaJanela - alturaViewport) / 2);
    glViewport(viewportX, viewportY, larguraViewport, alturaViewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 100, 0, 100, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaEixos() {
    glColor3f(0, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef(5, 5, 0);

        // linhas
        glBegin(GL_LINES);
            glVertex2f(0, 0);
            glVertex2f(90, 0);
            glVertex2f(0, 0);
            glVertex2f(0, 90);
        glEnd();

        // triangulim
        glBegin(GL_TRIANGLES);
            glVertex2f(90, 0);
            glVertex2f(90 - 6,  1.5);
            glVertex2f(90 - 6, -1.5);
        glEnd();
        glRotatef(90, 0, 0, 1);
        glBegin(GL_TRIANGLES);
            glVertex2f(90, 0);
            glVertex2f(90 - 6,  1.5);
            glVertex2f(90 - 6, -1.5);
        glEnd();

    glPopMatrix();
}

void desenhaCena(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // casinha 1
    configuraViewport(0, 3);
    desenhaEixos();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1, .5, .5);
    glPushMatrix();
        glTranslatef(5, 5, 0);
        // NÃO translada o sistema de coordenadas e
        // rotaciona o sistema de coordenadas no próprio lugar
        glRotatef(casinha1.angulo, 0, 0, 1);

        // paredes
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(casinha1.x, casinha1.y);
            glVertex2f(casinha1.x + casinha1.lado, casinha1.y);
            glVertex2f(casinha1.x + casinha1.lado, casinha1.y + casinha1.lado);
            glVertex2f(casinha1.x, casinha1.y + casinha1.lado);
        glEnd();
        // telhado
        glBegin(GL_TRIANGLES);
            glVertex2f(casinha1.x + casinha1.lado, casinha1.y + casinha1.lado);
            glVertex2f(casinha1.x + casinha1.lado / 2, casinha1.y + casinha1.lado * 1.5);
            glVertex2f(casinha1.x, casinha1.y + casinha1.lado);
        glEnd();

        // centro
        glBegin(GL_POINTS);
            glVertex2f(0, 0);
        glEnd();
        glColor3f(0.35, 0.35, 0.35);
        escreveTexto(GLUT_BITMAP_HELVETICA_10, "(0, 0)", -5, -4, 0);
    glPopMatrix();

    // casinha 2
    configuraViewport(1, 3);
    desenhaEixos();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.3, 0.8, 0.3);
    glPushMatrix();
        glTranslatef(5, 5, 0);
        // translada o sistema de coordenadas e
        glTranslatef(casinha2.x, casinha2.y, 0);
        // rotaciona o sistema de coordenadas no próprio lugar
        glRotatef(casinha2.angulo, 0, 0, 1);

        // paredes
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0, 0);
            glVertex2f(casinha2.lado, 0);
            glVertex2f(casinha2.lado, casinha2.lado);
            glVertex2f(0, casinha2.lado);
        glEnd();
        // telhado
        glBegin(GL_TRIANGLES);
            glVertex2f(casinha2.lado, casinha2.lado);
            glVertex2f(casinha2.lado / 2, casinha2.lado * 1.5);
            glVertex2f(0, casinha2.lado);
        glEnd();

        // centro
        glBegin(GL_POINTS);
            glVertex2f(0, 0);
        glEnd();
        glColor3f(0.35, 0.35, 0.35);
        escreveTexto(GLUT_BITMAP_HELVETICA_10, "(0, 0)", -6, -4, 0);
    glPopMatrix();

    // casinha 3
    configuraViewport(2, 3);
    desenhaEixos();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(.5, .5, 1);
    glPushMatrix();
        glTranslatef(5, 5, 0);
        // translada o sistema de coordenadas e
        glTranslatef(casinha3.x, casinha3.y, 0);
        // rotaciona o sistema de coordenadas no próprio lugar
        glRotatef(casinha3.angulo, 0, 0, 1);

        // paredes
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(-casinha3.lado/2, -casinha3.lado/2);
            glVertex2f( casinha3.lado/2, -casinha3.lado/2);
            glVertex2f( casinha3.lado/2,  casinha3.lado/2);
            glVertex2f(-casinha3.lado/2,  casinha3.lado/2);
        glEnd();
        // telhado
        glBegin(GL_TRIANGLES);
            glVertex2f( casinha3.lado/2,  casinha3.lado/2);
            glVertex2f( 0, casinha2.lado );
            glVertex2f(-casinha3.lado/2, casinha3.lado/2);
        glEnd();

        // centro
        glBegin(GL_POINTS);
            glVertex2f(0, 0);
        glEnd();
        glColor3f(0.35, 0.35, 0.35);
        escreveTexto(GLUT_BITMAP_HELVETICA_10, "(0, 0)", -6, -4, 0);
    glPopMatrix();


    // desenha texto com angulo atual
    glColor3f(0, 0, 0);
    char textoAngulo[100];
    sprintf(textoAngulo, "Angulo: %.0f°", casinha1.angulo);
    escreveTexto(GLUT_BITMAP_HELVETICA_12, textoAngulo, 60, 85, 0);
    // desenha instruções
    escreveTexto(GLUT_BITMAP_HELVETICA_12, "(+) aumenta", 60, 75, 0);
    escreveTexto(GLUT_BITMAP_HELVETICA_12, "(+) diminui", 60, 65, 0);


    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}


// Callback de evento de teclado
void teclado(unsigned char key, int x, int y) {
    switch(key) {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case '+':
            casinha1.angulo += 1;
            casinha2.angulo += 1;
            casinha3.angulo += 1;
            break;
        case '-':
            casinha1.angulo -= 1;
            casinha2.angulo -= 1;
            casinha3.angulo -= 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void redimensiona(int w, int h) {
    
}

// Rotina principal
int main(int argc, char **argv) {
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 200);

    // Abre a janela
    glutCreateWindow("Rotacao ao redor de um ponto");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutKeyboardFunc(teclado);
    glutReshapeFunc(redimensiona);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
