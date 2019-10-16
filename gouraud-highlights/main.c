#include <GL/glew.h>
#include <GL/freeglut.h>
// #include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "texto.h"

int subdivisoesEsquerda = 16;
const int subdivisoesDireita = 160;

float anguloRotacao = 0;
float posicaoDaLuz[4] = {1, 0, 0, 1};
float cameraZ = 0;
float expoenteEspecularidade = 50;
int modoArame = GL_TRUE;

// escreve informações na tela
void escreveEstadoNaTela() {
    glColor3f(0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        glOrtho(0, glutGet(GLUT_WINDOW_WIDTH)-1, glutGet(GLUT_WINDOW_HEIGHT)-1, 0, -1, 1);

        char texto[256];
        sprintf(texto, "subdivisoes: %d [+/-]", subdivisoesEsquerda);
        escreveTexto(texto, 5, 10, 0);

        sprintf(texto, "Expoente: %.0f [S/s]", expoenteEspecularidade);
        escreveTexto(texto, 5, 22, 0);
        //
        sprintf(texto, "Modo arame: [w]");
        escreveTexto(texto, 5, 34, 0);

        sprintf(texto, "Camera: [scroll]");
        escreveTexto(texto, 5, 46 , 0);

        sprintf(texto, "subdivisoes: %d", subdivisoesDireita);
        escreveTexto(texto, glutGet(GLUT_WINDOW_WIDTH)/2 +5, 10, 0);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    escreveEstadoNaTela();

    glLoadIdentity();
    gluLookAt(
        0, 0, cameraZ,
        0, 0, cameraZ-1,
        0, 1, 0
    );

    // desenha uma esferinha na posição da fonte de luz
    glPushMatrix();
        glTranslatef(posicaoDaLuz[0], posicaoDaLuz[1], posicaoDaLuz[2]);
        glColor3f(1, 1, 0);
        glutWireSphere(0.05, 5, 5);
    glPopMatrix();


    // ESFERA à esquerda
    glPushMatrix();
        glTranslatef(-1.25, 0, -3);
        glRotatef(anguloRotacao, 0, 1, 0);
        glRotatef(90, 1, 0, 0);

        glEnable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        glutSolidSphere(1, subdivisoesEsquerda, subdivisoesEsquerda);
        glDisable(GL_LIGHTING);

        if (modoArame == GL_TRUE) {
            glScalef(1.001, 1.001, 1.001);
            glColor4f(0, 0, 0, 0.15);
            glutWireSphere(1, subdivisoesEsquerda, subdivisoesEsquerda);
        }
    glPopMatrix();

    // ESFERA à direita
    glPushMatrix();
        glTranslatef( 1.25, 0, -3);
        glRotatef(anguloRotacao, 0, 1, 0);
        glRotatef(90, 1, 0, 0);

        glEnable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        glutSolidSphere(1, 160, 160);
        glDisable(GL_LIGHTING);

        if (modoArame == GL_TRUE) {
            glScalef(1.001, 1.001, 1.001);
            glColor4f(0, 0, 0, 0.15);
            glutWireSphere(1, 160, 160);
        }
    glPopMatrix();

    glutSwapBuffers();
}

// para alterar o estado de algumas variáveis
void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case 27 :     // Tecla 'ESC
            exit(0);
            break;

        case '+':
            subdivisoesEsquerda++;
            break;

        case '-':
            if (subdivisoesEsquerda>3) {
                subdivisoesEsquerda--;
            }
            break;

        case 'w':
        case 'W':
            modoArame = modoArame == GL_TRUE ? GL_FALSE : GL_TRUE;
            break;

        case 's':
            expoenteEspecularidade -= 5;
            if (expoenteEspecularidade < 1) {
                expoenteEspecularidade = 1;
            }
            glMaterialf(GL_FRONT, GL_SHININESS, expoenteEspecularidade);
            break;

        case 'S':
            expoenteEspecularidade += 5;
            if (expoenteEspecularidade > 128) {
                expoenteEspecularidade = 128;
            }
            glMaterialf(GL_FRONT, GL_SHININESS, expoenteEspecularidade);
            break;
    }

    glutPostRedisplay();
}

// para rotacionar as esferas ao longo do tempo
void atoa() {
    anguloRotacao += 0.75;
    glutPostRedisplay();
}

// para mudar a posição da fonte de luz com o movimento do mouse
void movimentoMouse(int x, int y) {
    posicaoDaLuz[0] = ((float)x)/glutGet(GLUT_WINDOW_WIDTH)*5 - 2.5;
    posicaoDaLuz[1] = -((float)y)/glutGet(GLUT_WINDOW_HEIGHT)*5 + 2.5;
    posicaoDaLuz[2] = 0;   // sempre à frente da cena (1 unidade)
    posicaoDaLuz[3] = 1;   // luz posicional (em vez de direcional)

    // posiciona a luz
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoDaLuz);
}


// iniciliza estado do opengl com luz e material
void inicializa() {
    glCheckError();
    // configura alguns parâmetros do modelo de iluminação: FONTE DE LUZ
    const GLfloat componenteAmbienteLuz[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat componenteDifusaLuz[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat componenteEspecularLuz[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // configura alguns parâmetros do modelo de iluminação: MATERIAL
    const GLfloat componenteAmbienteMaterial[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
    const GLfloat componenteEmissivaMaterial[]   = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat componenteDifusaMaterial[]    = { 1.0f, 0.3f, 0.3f, 1.0f };
    const GLfloat componenteEspecularMaterial[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat luzAmbienteGlobal[] = { 0.3, 0.3, 0.3, 1.0 };


    glClearColor(1, 1, 1, 1);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.

    glCheckError();
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteGlobal);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  componenteAmbienteLuz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  componenteDifusaLuz);
    glLightfv(GL_LIGHT0, GL_POSITION,  posicaoDaLuz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, componenteEspecularLuz);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   componenteAmbienteMaterial);
    glMaterialfv(GL_FRONT, GL_EMISSION,  componenteEmissivaMaterial);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   componenteDifusaMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  componenteEspecularMaterial);
    glMaterialf(GL_FRONT, GL_SHININESS, expoenteEspecularidade);
    glCheckError();
}

// para navegar com a câmera usando mouse wheel
void mouse(int button, int state, int x, int y) {
    enum {
        GLUT_SCROLL_UP_BUTTON = 3,
        GLUT_SCROLL_DOWN_BUTTON = 4
    };

    if (button == GLUT_SCROLL_UP_BUTTON && state == GLUT_UP) {
        cameraZ -= 0.5;
    } else if (button == GLUT_SCROLL_DOWN_BUTTON && state == GLUT_UP) {
        cameraZ += 0.5;
    }
}

void redimensiona(int width, int height) {
    float razaoAspecto = ((float) width) / height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, razaoAspecto, 1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 380);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Highlights usando Gouraud");

    glutPassiveMotionFunc(movimentoMouse);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);
    glutMouseFunc(mouse);
    glutIdleFunc(atoa);

    inicializa();

    glutMainLoop();

    return 0;
}
