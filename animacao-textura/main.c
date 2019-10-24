#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include "texto.h"
#define TRUE 1
#define FALSE 0

struct Textura {
    GLuint id;
    char arquivo[50];
};
GLubyte texturaAtual = 0;
GLubyte qtdeTexturas = 3;

static struct Textura texturas[] = { {0, "lava.jpg"}, {0, "alien-wall.jpg"}, {0, "waterfall.jpg"} };

enum Objetos {
    QUADRADO,
    PLANO,
    CUBO,
    CILINDRO,
    ROSQUINHA,
    QTDE_OBJETOS
};
GLubyte objetoAtual = QUADRADO;
GLUquadric* quadricaCilindro;

static GLfloat velocidadeS = 0.001f;
static GLfloat velocidadeT = 0;
static GLfloat deslocamentoS = 0;
static GLfloat deslocamentoT = 0;


GLuint carregaTextura(const char* caminho) {
    GLuint idTextura = SOIL_load_OGL_texture(
        caminho,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro do SOIL ao carregar %s: '%s'\n", caminho, SOIL_last_result());
    }

    return idTextura;
}

void escreveInstrucoes() {
    printf("Instrucoes:\n");
    printf("\t[ESC]: sai da aplicacao\n");
    printf("\t[space]: troca de objeto (quadrado, plano, cubo, cilindro, rosquinha)\n");
    printf("\t[i]: troca a imagem da textura\n");
    printf("\t[s], [S]: diminui ou aumenta a velocidade da coordenada s\n");
    printf("\t[t], [T]: diminui ou aumenta a velocidade da coordenada t\n\n");
}

void inicializa() {
    glClearColor (1, 1, 1, 0);

    // habilita teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // habilita transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // habilita descarte de faces internas
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // configura cilindro
    quadricaCilindro =  gluNewQuadric();
    gluQuadricTexture(quadricaCilindro, GLU_TRUE);

    // carrega as texturas
    for (int c = 0; c < qtdeTexturas; c++) {
        texturas[c].id = carregaTextura(texturas[c].arquivo);
    }

    escreveInstrucoes();
}

void desenha() {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(deslocamentoS, deslocamentoT, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 1, 1);

    // habilita e configura o uso de texturas
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texturas[texturaAtual].id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (objetoAtual) {
    case QUADRADO:
        glPushMatrix();
            glScalef(2, 2, 0);
            glBegin(GL_TRIANGLE_FAN);
                // Associamos um canto da textura para cada vértice
                glTexCoord2f(0, 0); glVertex3f(-1, -1,  0);
                glTexCoord2f(1, 0); glVertex3f( 1, -1,  0);
                glTexCoord2f(1, 1); glVertex3f( 1,  1,  0);
                glTexCoord2f(0, 1); glVertex3f(-1,  1,  0);
            glEnd();
        glPopMatrix();
        break;

    case PLANO:
        glPushMatrix();
            glTranslatef(0, -1.5, 0);
            glScalef(20, 20, 1);
            glRotatef(-85, 1, 0, 0);
            glBegin(GL_TRIANGLE_FAN);
                // Associamos um canto da textura para cada vértice
                glTexCoord2f( 0,  0); glVertex3f(-1, -1,  0);
                glTexCoord2f( 8,  0); glVertex3f( 1, -1,  0);
                glTexCoord2f( 8, 8); glVertex3f( 1,  1,  0);
                glTexCoord2f( 0, 8); glVertex3f(-1,  1,  0);
            glEnd();
        glPopMatrix();
        break;

    case CUBO:
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glPushMatrix();
            glRotatef(20, 1, 0, 0);
            glRotatef(35, 0, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        break;

    case CILINDRO:
    default:
        glPushMatrix();
            glTranslatef(0, -1.5, -1);
            glRotatef(-90, 1, 0, 0);
            glRotatef( 10, 0, 0, 1);
            gluCylinder(quadricaCilindro, 1, 1, 3, 30, 3);
        glPopMatrix();
        break;

    case ROSQUINHA:
        glPushMatrix();
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glTranslatef(0, 0, -1);
            glRotatef(115, 1, 0, 0);
            glRotatef( 20, 0, 0, 1);
            glutSolidTorus(0.5, 1.5, 30, 30);
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
        glPopMatrix();
        break;
    }
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void redimensiona(int w, int h) {
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60, ((float)w)/h, 0.1, 1000);

   glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char key, int x, int y) {
    const GLfloat incremento = 0.001;
    switch (key) {
    case 27:
        exit(0);
        break;
    case ' ':
        objetoAtual = (objetoAtual + 1) % QTDE_OBJETOS;
        break;
    case 'i':
    case 'I':
        texturaAtual = (texturaAtual + 1) % qtdeTexturas;
        break;
    case 's':
        velocidadeS -= incremento;
        break;
    case 'S':
        velocidadeS += incremento;
        break;
    case 't':
        velocidadeT -= incremento;
        break;
    case 'T':
        velocidadeT += incremento;
        break;
    }
}

void atualiza() {
    deslocamentoS += velocidadeS;
    deslocamentoT += velocidadeT;

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Animacao de textura");
    inicializa();

    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);
    glutIdleFunc(atualiza);
    glutMainLoop();

    return 0;
}
