#include <GL/freeglut.h>
#include <stdio.h>

const float LARGURA_BRACO = 2.0;
int anguloOmbro = 0, anguloCutuvelo = 0;

void inicializa() {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}


void redimensiona(int w, int h) {
    glViewport (0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // a origem do mundo está no centro da janela (o gluPerspective
    // é sempre assim)
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // chega o mundo todo mais pro fundo, pra poder desenhar no z = 0
    glTranslatef (0.0, 0.0, -6.0);
}


// o braço e ante-braço são cada um uma caixa de tamanho 2 no eixo x, 0.4 no y
// e 1 de profundidade:
//  _________
// /________/|
// |________|/
//
// a glutSolid|WireCube desenha o cubo na origem (e.g., -WIDTH/2, +WIDTH/2...)
void desenhaParteDoBraco() {
    glPushMatrix();
        glScalef(LARGURA_BRACO, 0.4, 1.0);
        glutWireCube(1.0);
    glPopMatrix();
}


void desenha() {
    // entramos na desenha() com o sistema de coordenadas com origem no centro
    // da janela

    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix();
        // -------
        // o braço (ombro ao cutuvelo), que é desenhado tendo largura = 2
        //
        // (1) anda 1 unidade (larg/2) para a esquerda
        //     (lado esquerdo do braço, que é o ombro),
        // (2) rotaciona o eixo x graus (referente ao ombro)
        // (3) devolve o sistema de coordenadas 1 unidade (larg/2) no sentido x
        //     (não é mais "a direita do mundo")
        // (4) desenha o braço onde está nosso "cursor" (no 0,0 do sistema de
        //     coordenadas corrente)
        glTranslatef(-LARGURA_BRACO/2, 0, 0);
        glRotatef(anguloOmbro, 0, 0, 1);
        glTranslatef(1, 0, 0);
        desenhaParteDoBraco();

        // ---------
        // antebraço (cutuvelo ao punho)
        //
        // continuamos com o sistema de coordenadas no centro do braço,
        // que foi rotacionado pelo ângulo do ombro...
        //
        // agora vamos:
        // (1) mover o cursor para o lado direito do braço (o cutuvelo)
        // (2) rotacionar o sistema y graus (ref. ao cutuvelo)
        // (3) andar +1 unidade no eixo x, para chegar até o centro do antebraço
        // (4) desneha o antebraço
        glTranslatef(LARGURA_BRACO/2, 0.0, 0.0);
        glRotatef(anguloCutuvelo, 0.0, 0.0, 1.0);
        glTranslatef(LARGURA_BRACO/2, 0.0, 0.0);
        glPushMatrix();
            desenhaParteDoBraco();
        glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
   switch (key) {
      // Tecla ESC
      case 27:
          exit(0);
          break;
      case 'o':
         anguloOmbro = (anguloOmbro + 5) % 360;
         glutPostRedisplay();
         break;
      case 'O':
         anguloOmbro = (anguloOmbro - 5) % 360;
         glutPostRedisplay();
         break;
      case 'c':
         anguloCutuvelo = (anguloCutuvelo + 5) % 360;
         glutPostRedisplay();
         break;
      case 'C':
         anguloCutuvelo = (anguloCutuvelo - 5) % 360;
         glutPostRedisplay();
         break;
      default:
         break;
   }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Braco do Robo");
    inicializa();

    glutDisplayFunc(desenha);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);

    printf("Aperte:\n\n");
    printf("\t- 'o' e 'O': para aumentar/diminuir rotação do 'ombro'\n");
    printf("\t- 'c' e 'C': para aumentar/diminuir rotação do 'cutuvelo'\n\n");

    glutMainLoop();
    return 0;
}
