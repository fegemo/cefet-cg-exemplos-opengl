#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#include "modelo.h"

struct modelo *modelo;
float orientacao = 0;
float xInicialArraste;
int girandoObjeto = 0;
int windowWidth;
float cameraZoom = -10;
int wireframeToo = 1;

void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        gluLookAt(0, 0, cameraZoom,
                  0, 0, 0,
                  0, 1, 0);

        glRotatef(orientacao, 0, 1, 0);

//        glColor3f(0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);

        glPushMatrix();
//            glColor3f(1.0, 1.0, 0.0);
            glScalef(2, 2, 2);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_LIGHTING);
            desenhaModelo(modelo);
            glDisable(GL_LIGHTING);
            if (wireframeToo) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glColor3f(0, 0, 0);
                desenhaModelo(modelo);
            }
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void setup() {
    glClearColor(1.0, 1.0, 1.0, 0.0); // branco
    glCullFace(GL_BACK);
    // habilita o depth buffer para que a coordenada Z seja usada
    glEnable(GL_DEPTH_TEST);

    // habilita anti-aliasing para desenhar as linhas de forma suave
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable( GL_LINE_SMOOTH );

    // habilita blending para que transparências (e.g., linhas suaves) funcionem
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // modelo de iluminação
    glEnable(GL_LIGHT0);
    vetor direcaoLuz[] = { -1.0, 2.0, 2.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, direcaoLuz);
    glEnable(GL_LIGHT1);
    vetor direcaoLuz2[] = { 2.0, 4.0, -2.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, direcaoLuz2);

    glShadeModel(GL_FLAT);

    // carrega o modelo
    modelo = carregaModelo("cubo.obj");
//    modelo = carregaModelo("arvore.obj");
}

void redimensionada(int width, int height) {
   windowWidth = width;
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, ((float)width)/height, 0.1, 50);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    case 'w':
        wireframeToo = !wireframeToo;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // começou a arrastar, salva a posição x
            xInicialArraste = x;
            girandoObjeto = 1;
        } else {
            // fim do arraste
            girandoObjeto = 0;
        }
    } else if ((button == 3) || (button == 4) || (button == 5) || (button == 6)) {
        // 3, 4: rodinha do mouse para frente, tras
        // 5, 6: trackpad de notebook para cima, baixo
       if (state == GLUT_UP) return;
       cameraZoom += (button % 2 == 1 ? 1 : -1) * 1;
       glutPostRedisplay();
    }
}

void mouseArrastando(int x, int y) {
    if (girandoObjeto) {
        orientacao += ((float)(x - xInicialArraste) / windowWidth) * 150;
        xInicialArraste = x;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Carregador de Objs");

    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutMotionFunc(mouseArrastando);
    glutMouseFunc(mouse);

    setup();

    glutMainLoop();
    return 0;
}
