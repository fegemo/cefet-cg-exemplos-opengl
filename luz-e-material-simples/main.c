#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>


typedef struct coords {
  union {
      float v[4];
      struct {
          float x, y, z, w;
      };
      struct {
          float r, g, b, a;
      };
  };
} ponto, vetor, cor;

typedef struct {
    float largura, altura;
} dimensoes;

typedef struct material {
    cor ambiente;
    cor emissiva;
    cor difusa;
    cor especular;
    float brilhosidade[1];
} material;

typedef struct ladrilho {
    ponto posicao;
    dimensoes dim;
    material mat;
} ladrilho;

material plasticoAzul, marromFosco;
ladrilho ladrilhos[4];

ponto posicaoDaLuz;

void desenhaLadrilho(ladrilho l) {
    glPushMatrix();
        glTranslatef(l.posicao.x, l.posicao.y, l.posicao.z);
        glScalef(l.dim.largura, l.dim.altura, 1);
        glColor3f(1, 1, 1);
        glBegin(GL_TRIANGLE_FAN);
            glNormal3f( 0,  0, 1);
            glVertex3f(-1, -1, 0);
            glVertex3f( 1, -1, 0);
            glVertex3f( 1,  1, 0);
            glVertex3f(-1,  1, 0);
        glEnd();
    glPopMatrix();
}


void configuraCena() {
    // plasticoAzul = {
    //   { 0, 0, 0, 0 },
    //   { 0, 0, 0, 0 },
    //   { 0.2, 0.2, 0.5, 1 },
    //   { 1, 1, 1, 1 },
    //   { 90 }
    // };
    plasticoAzul.ambiente = (cor){ 0.1, 0.1, 0.1, 1 };
    plasticoAzul.emissiva = (cor){ 0, 0, 0, 1 };
    plasticoAzul.difusa = (cor){ 0.1, 0.1, 0.4, 1 };
    plasticoAzul.especular = (cor){ 1, 1, 1, 1 };
    plasticoAzul.brilhosidade[0] = 5;
    //

    posicaoDaLuz = (ponto){ 0, 0, -1, 1 };
    cor corDaLuz = { 1.0, 1.0, 1.0, 1.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plasticoAzul.emissiva.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plasticoAzul.difusa.v);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, plasticoAzul.especular.v);
    // glMaterialfv(GL_FRONT, GL_SHININESS, plasticoAzul.brilhosidade);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plasticoAzul.especular.v);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plasticoAzul.brilhosidade);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoDaLuz.v);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corDaLuz.v);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corDaLuz.v);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

void inicializaLadrilhos() {

    // ladrilhos[0].posicao;
    // ladrilhos[0].dim;
    // ladrilhos[0].mat;
}

void desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glTranslatef(posicaoDaLuz.x, posicaoDaLuz.y, -1);
        glColor3f(1, 1, 0);
        glutWireSphere(0.1, 4, 4);
    glPopMatrix();

    glColor3f(1, 1, 1);
    glEnable(GL_LIGHTING);
    // desenha objetos
    glPushMatrix();
        glTranslatef(0, 0, -4);
        glutSolidSphere(1.0, 48, 48);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glutSwapBuffers();
}

void redimensiona(int w, int h) {
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, w/h, 1, 40);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void movimentoMouse(int x, int y) {
    posicaoDaLuz.x = ((float)x)/glutGet(GLUT_WINDOW_WIDTH)*4 - 2;
    posicaoDaLuz.y = -((float)y)/glutGet(GLUT_WINDOW_HEIGHT)*4 + 2;
    posicaoDaLuz.z = -1;   // sempre à frente da cena (1 unidade)
    posicaoDaLuz.w = 1;   // luz posicional (em vez de direcional)

    // posiciona a luz
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoDaLuz.v);

    glutPostRedisplay();
}

void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case 27:  // ESC
            exit(0);
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Luz e Material");
    configuraCena();
    inicializaLadrilhos();
    glutDisplayFunc(desenha);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutMainLoop();
    return 0;
}
