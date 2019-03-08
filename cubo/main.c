// ATENÇÃO: se vsync estiver habilitado na placa de vídeo,
// o FPS reportado será próximo de 60.
// Para desabilitar vsync para este programa, defina uma variável de ambiente
// logo antes de executá-lo:
//    vblank_mode=0 (para placas da Intel)
//    __GL_SYNC_TO_VBLANK=0 (da NVIDIA)
//
// Mais informações na thread: https://stackoverflow.com/questions/17196117/disable-vertical-sync-for-glxgears

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// variáveis para cálculo e apresentação do FPS
char tituloDaJanela[200];
int momentoAnterior;
int quadrosNesteSegundo = 0;
float fps = 0;


// variáveis do cubo
GLdouble rotacaoCubo = 0;

GLfloat verticesCubo[] = {
    1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (frente)
   -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

    1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (direita)
    1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

    1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (cima)
   -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (esquerda)
   -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

   -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (baixo)
    1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

    1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (trás)
   -1, 1,-1,   1, 1,-1,   1,-1,-1       // v6-v5-v4
};

// array com as normais de cada vértice
GLfloat normaisCubo[]   = {
    0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (frente)
    0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

    1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (direita)
    1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

    0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (cima)
    0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (esquerda)
   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

    0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (baixo)
    0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

    0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (trás)
    0, 0,-1,   0, 0,-1,   0, 0,-1       // v6-v5-v4
};

// array com a cor de cada vértice
GLfloat coresCubo[]    = {
    1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (frente)
    1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

    1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (direita)
    0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

    1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (cima)
    0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

    1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (esquerda)
    0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

    0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (baixo)
    1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

    0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (trás)
    0, 1, 0,   0, 1, 1,   0, 0, 1       // v6-v5-v4
};

void inicializa() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)(w)/h, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaCubo() {
    // o cubo tem 36 vértices definidos.
    // cada vértice tem x,y,z, logo, são 108 valores (36 x 3)
    // NUM_VALORES = 108
    const int NUM_VALORES = sizeof(verticesCubo) / sizeof(GLfloat);

    glBegin(GL_TRIANGLES);
      int c;
      for (c = 0; c < NUM_VALORES; c += 3) {
          glNormal3f(normaisCubo[c], normaisCubo[c+1], normaisCubo[c+2]);
          glColor3f(coresCubo[c], coresCubo[c+1], coresCubo[c+2]);
          glVertex3f(verticesCubo[c], verticesCubo[c+1], verticesCubo[c+2]);
      }
    glEnd();
}

void desenhaCena() {

    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;
    quadrosNesteSegundo++;

    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        gluLookAt(0, 2.5, -5, 0, 0, 0, 0, 1, 0);
        glRotated(rotacaoCubo, 0, 1, 0);
        desenhaCubo();

    glPopMatrix();


    momentoAnterior = momentoAtual;

    // troca os buffers e manda desenhar
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
    }
}

void atualiza() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;

    // efetivamente atualiza a rotação do cubo
    rotacaoCubo += 0.01f * delta;
    if (rotacaoCubo > 360.0f) {
      rotacaoCubo -= 360.0f;
    }

    glutPostRedisplay();
}

void atualizaFPS(int periodo) {
    fps = quadrosNesteSegundo;
    quadrosNesteSegundo = 0;
    
    sprintf(tituloDaJanela,
        "Cubo (%.2f fps)",
        fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(periodo, atualizaFPS, periodo);
}


int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(2, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("...");

    glutReshapeFunc(redimensiona);
    glutDisplayFunc(desenhaCena);
    glutIdleFunc(atualiza);
    glutTimerFunc(0, atualizaFPS, 1000);
    glutKeyboardFunc(teclado);

    glewInit();
    inicializa();


    glutMainLoop();
    return 0;
}
