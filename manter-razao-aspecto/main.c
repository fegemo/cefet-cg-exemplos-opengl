#include <GL/glew.h>
#include <GL/freeglut.h>

#define LARGURA_DO_MUNDO 160
#define ALTURA_DO_MUNDO 90

void desenhaRetangulo(int x, int y, int w, int h) {
  glBegin(GL_TRIANGLE_FAN);
      glVertex2i(x, y);
      glVertex2i(x + w, y);
      glVertex2i(x + w, y + h);
      glVertex2i(x, y + h);
  glEnd();
}

// Rotina de desenho
void desenhaMinhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // desenha um retângulo verde do tamanho do mundo
    glColor3f(0, .5, 0);  // verde escuro
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    desenhaRetangulo(1, 1, LARGURA_DO_MUNDO-2, ALTURA_DO_MUNDO-2);

    // desenha um quadrado no centro do mundo
    glColor3f(.6, 0, 0);  // vermelho
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    desenhaRetangulo(LARGURA_DO_MUNDO/2 - 10, ALTURA_DO_MUNDO/2-10, 20, 20);

    glFlush();
}

void setup() {
    glClearColor(1, 1, 1, 0); // branco
}

void redimensionada(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARGURA_DO_MUNDO, 0, ALTURA_DO_MUNDO, -1, 1);

    float razaoAspectoJanela = ((float)width)/height;
    float razaoAspectoMundo = ((float) LARGURA_DO_MUNDO)/ ALTURA_DO_MUNDO;
    // se a janela está menos larga do que o mundo (16:9)...
    if (razaoAspectoJanela < razaoAspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / razaoAspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (razaoAspectoJanela > razaoAspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * razaoAspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    } else {
        glViewport(0, 0, width, height);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y)
{
    if (key == 27) {
      exit(0);
    }
}

// Função principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(480, 270);

    glutCreateWindow("Mantém razão de aspecto");

    // Registra callbacks para eventos
    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);

    // Configura valor inicial de algumas
    // variáveis de estado do OpenGL
    setup();

    glutMainLoop();
    return 0;
}
