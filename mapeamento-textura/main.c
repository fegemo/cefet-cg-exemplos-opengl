#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "carregaTextura.h"

#define DIVISOES_ESFERA 20
#define RAIO_ESFERA 15
#define AUMENTO_WIREFRAME 1.001

GLuint toalhaDeMesa;
GLfloat rotacaoTexturaZ = 0;
GLfloat escalaTextura = 12;
GLuint mostrarArame = 0;

// Desenha uma esfera na origem, com certo raio e subdivisões
// latitudinais e longitudinais
//
// Não podemos usar glutSolidSphere, porque ela não chama
// glTexCoord para os vértices. Logo, se você quer texturizar
// uma esfera, não pode usar glutSolidSphere
void solidSphere(int radius, int stacks, int columns)
{
    // cria uma quádrica
    GLUquadric* quadObj = gluNewQuadric();
    // estilo preenchido... poderia ser GLU_LINE, GLU_SILHOUETTE
    // ou GLU_POINT
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    // chama 01 glNormal para cada vértice.. poderia ser
    // GLU_FLAT (01 por face) ou GLU_NONE
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    // chama 01 glTexCoord por vértice
    gluQuadricTexture(quadObj, GL_TRUE);
    // cria os vértices de uma esfera
    gluSphere(quadObj, radius, stacks, columns);
    // limpa as variáveis que a GLU usou para criar
    // a esfera
    gluDeleteQuadric(quadObj);
}

void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // configura a matriz de textura
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(escalaTextura, escalaTextura, 1);
    // glRotatef(rotacaoTexturaZ, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        // posiciona a câmera
        gluLookAt(0, 40, 1,
                  0, -5, -50,
                  0, 1, 0);

        // posiciona a esfera
        glTranslatef(0, 0, -40);
        glRotatef(90, 1, 0, 0);

        // desenha a preenchida
        glColor3f(1, 1, 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, toalhaDeMesa);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPolygonMode(GL_FRONT, GL_FILL);
        solidSphere(RAIO_ESFERA, DIVISOES_ESFERA, DIVISOES_ESFERA);
        glDisable(GL_TEXTURE_2D);

        // desenha em arames
        if (mostrarArame) {
            glColor4f(0, 0, 0, 0.5);
            glScalef(AUMENTO_WIREFRAME, AUMENTO_WIREFRAME, AUMENTO_WIREFRAME);
            glPolygonMode(GL_FRONT, GL_LINE);
            solidSphere(RAIO_ESFERA, DIVISOES_ESFERA, DIVISOES_ESFERA);
        }
    glPopMatrix();

    glFlush();
}

void inicializa(void)
{
    glClearColor(1, 1, 1, 0);

    // ativa o depth buffer
    glEnable(GL_DEPTH_TEST);

    // habilita mesclagem de cores, para termos suporte a texturas (semi-)transparentes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // carrega uma textura
    toalhaDeMesa = carregaTextura("toalha-de-mesa-restaurante-italiano.png");
}

void redimensiona(int w, int h)
{
   glViewport(0, 0, w, h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45, ((float)w)/h, 1, 1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void teclado(unsigned char key, int x, int y)
{
   switch(key)
   {
      // Tecla ESC
      case 27:
          exit(0);
          break;
      case ' ':
          mostrarArame = mostrarArame == 1 ? 0 : 1;
          break;
      case '+':
      case '=':
          escalaTextura++;
          break;
      case '-':
      case '_':
          if (escalaTextura > 1) {
            escalaTextura--;
          } else if (escalaTextura > 0.1) {
            escalaTextura -= 0.1;
          }
          break;
   }

   glutPostRedisplay();
}

void atualiza(int periodo) {
    rotacaoTexturaZ += 0.1;

    glutPostRedisplay();
    glutTimerFunc(periodo, atualiza, periodo);
}

void imprimeInstrucoes() {
    printf("Instruções:\n");
    printf("\t'space': ativa/desativa modo de arame\n");
    printf("\t'+':     aumenta a escala da matriz de textura\n");
    printf("\t'-':     reduz a escala da matriz de textura\n\n");
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Mapeamento de Texturas");

    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutTimerFunc(33, atualiza, 33);
    inicializa();

    imprimeInstrucoes();
    glutMainLoop();

    return 0;
}
