#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
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
    int textura;
} ladrilho;

material plasticoAzul, marromFosco;
ladrilho ladrilhos[4];

ponto posicaoDaLuz;
int subdivisoesLadrilho = 4;
int modoDeArame = 0;
int modoSombreamento = 0;

void carregaTextura(int *textura, char* nomeDoArquivo) {
    *textura = SOIL_load_OGL_texture(
      nomeDoArquivo,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

    if (*textura == 0) {
        printf("Erro ao carregar textura '%s': %s\n",
            nomeDoArquivo,
            SOIL_last_result());
    }
}

/// Configura os materiais (plastico e marrom) e a fonte de luz posicional.
///
void configuraIluminacao() {
    // glEnable(GL_LIGHTING);
    plasticoAzul.ambiente = (cor){ 0.25, 0.25, 0.25, 1 };
    plasticoAzul.emissiva = (cor){ 0, 0, 0, 1 };
    plasticoAzul.difusa = (cor){ 0.1, 0.1, 0.4, 1 };
    plasticoAzul.especular = (cor){ 1, 1, 1, 1 };
    plasticoAzul.brilhosidade[0] = 100;

    marromFosco.ambiente = (cor){ 0.1, 0.1, 0.1, 1 };
    marromFosco.emissiva = (cor){ 0, 0, 0, 1 };
    marromFosco.difusa = (cor){ .49, .22, .02, 1 };
    marromFosco.especular = (cor){ 0, 0, 0, 1 };
    marromFosco.brilhosidade[0] = 0;


    posicaoDaLuz = (ponto){ 0, 0, -1, 1 };
    cor corDaLuz = { 1.0, 1.0, 1.0, 1.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_POSITION, posicaoDaLuz.v);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corDaLuz.v);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corDaLuz.v);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

/// Configura os 4 ladrilhos:
///  - Em cima: plastico sem textura, com textura
///  - Abaixo: marrom sem textura, com textura
void inicializaLadrilhos() {
    int texturaPlastico, texturaMadeira;
    carregaTextura(&texturaPlastico, "plastico.jpg");
    carregaTextura(&texturaMadeira, "madeira.jpg");

    int i;
    for (i = 0; i < 4; i++) {
        ladrilhos[i].dim = (dimensoes){ 2.0f, 2.0f };
        ladrilhos[i].posicao.x = ((i % 2) - 0.5)  * ladrilhos[0].dim.largura;
        ladrilhos[i].posicao.y =  ((i / 2) - 0.5)  * ladrilhos[0].dim.altura;
        ladrilhos[i].posicao.z = 0;
        ladrilhos[i].mat = (i / 2 == 0 ? marromFosco : plasticoAzul);
        ladrilhos[i].textura = (i % 2 == 1    // se estamos na coluna da direita
            ? (i / 2 == 0                     // se linha de baixo
                ? texturaMadeira
                : texturaPlastico)
            : 0);
    }
}

/// Desenha um retângulo que está subdivido em retângulos menores em uma
/// quantidade igual a "subdivisoes" tanto no eixo x, quanto no eixo y.
/// Portanto, o número total de subretângulos é de (subdivisoes^2) e de
/// triângulos é de (subdivisoes^2 * 2).
///
/// O retângulo desenhado é um quadrado com lado igual a 1, centrado na origem.
///
/// Se for chamada com subdivisoes=1, o retângulo é desenhado tendo apenas
/// 1 subretângulo que representa o retângulo inteiro (i.e., 2 triângulos,
/// 4 vértices).
void desenhaRetanguloSubdividido(int subdivisoes) {
    int i, j;
    float alturaDoSubTriangulo = 1.0f / (subdivisoes);
    float larguraDoSubTriangulo = 1.0f / (subdivisoes);
    glPolygonMode(GL_FRONT, modoDeArame ? GL_LINE : GL_FILL);
    glPushMatrix();
        glTranslatef(-0.5f, -0.5f, 0);
        // desenha uma quantidade de tiras de triângulos (TRIANGLE_STRIP) igual
        // a "subdivisoes". Por exemplo, para subdivisoes=3:
        //  ___ ___ ___
        // |  /|  /|  /|  - esta é a terceira tira
        // |/__|/__|/__|
        // |  /|  /|  /|  - esta é a segunda tira
        // |/__|/__|/__|
        // |  /|  /|  /|  - esta é a primeira tira
        // |/__|/__|/__|
        for (i = 0; i < subdivisoes; i++) {
            glBegin(GL_TRIANGLE_STRIP);
                // define o vetor normal do ladrilho
                glNormal3f(0,  0, 1);
                float yVerticesLinhaDeBaixo = alturaDoSubTriangulo * (i);
                float yVerticesLinhaDeCima = yVerticesLinhaDeBaixo
                    + alturaDoSubTriangulo;

                // veja uma tira de triângulos com subdivisoes=3:
                //
                // v0     v2     v4     v6
                // |    / |    / |    / |
                // |  /   |  /   |  /   |
                // v1     v3     v5     v7
                for (j = 0; j < subdivisoes + 1; j++) {
                    float xVertice = larguraDoSubTriangulo * j;
                    glTexCoord2f(xVertice, yVerticesLinhaDeCima);
                    glVertex2f(xVertice, yVerticesLinhaDeCima);
                    glTexCoord2f(xVertice, yVerticesLinhaDeBaixo);
                    glVertex2f(xVertice, yVerticesLinhaDeBaixo);
                }
            glEnd();
        }
    glPopMatrix();
}

void desenhaLadrilho(ladrilho l) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, l.mat.ambiente.v);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, l.mat.difusa.v);
    glMaterialfv(GL_FRONT, GL_SPECULAR, l.mat.especular.v);
    glMaterialfv(GL_FRONT, GL_SHININESS, l.mat.brilhosidade);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, l.textura);
    glPushMatrix();
        glTranslatef(l.posicao.x, l.posicao.y, l.posicao.z);
        glScalef(l.dim.largura, l.dim.altura, 1);
        desenhaRetanguloSubdividido(subdivisoesLadrilho);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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

    if (modoSombreamento == 0) {
        glShadeModel(GL_SMOOTH);
    } else {
      glShadeModel(GL_FLAT);
    }
    // desenha objetos
    glPushMatrix();
        // adentra o sistema de coordenadas 4u no sentido da tela
        glTranslatef(0, 0, -4);
        desenhaLadrilho(ladrilhos[0]);
        desenhaLadrilho(ladrilhos[1]);
        desenhaLadrilho(ladrilhos[2]);
        desenhaLadrilho(ladrilhos[3]);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glutSwapBuffers();
}

void redimensiona(int w, int h) {
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, ((float)w)/h, 1, 40);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void movimentoMouse(int x, int y) {
    posicaoDaLuz.x = ((float)x)/glutGet(GLUT_WINDOW_WIDTH)*10 - 5;
    posicaoDaLuz.y = -((float)y)/glutGet(GLUT_WINDOW_HEIGHT)*10 + 5;
    posicaoDaLuz.z = 1;   // sempre à frente da cena (1 unidade)
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
        case 'p':
            modoDeArame = !modoDeArame;
            break;
        case '+':
        case '=':
            subdivisoesLadrilho = fmin(100, subdivisoesLadrilho + 1);
            break;
        case '-':
        case '_':
            subdivisoesLadrilho = fmax(1, subdivisoesLadrilho - 1);
            break;
        case 'l':
        case 'L':
            glDisable(GL_LIGHT0);
            break;

        case 'm':
        case 'M':
            modoSombreamento = (modoSombreamento + 1) % 2;
            break;
    }
    glutPostRedisplay();
}

void imprimeInstrucoes() {
      printf("Ajuda:\n");
      printf("  Aperte 'p' para alternar entre modo de arame e preenchido\n");
      printf("  Aperte '+'/'-' para aumentar/diminuir o número de subdivisões.\n");
      printf("  Aperte 'm' para alternar o modelo de sombreamento entre GL_FLAT e GL_SMOOTH.\n");
      printf("  Movimente o mouse para mudar a posição da fonte de luz posicional.\n");
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Luz e Material - ladrilhos");
    configuraIluminacao();
    inicializaLadrilhos();
    imprimeInstrucoes();
    glutDisplayFunc(desenha);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutMainLoop();
    return 0;
}
