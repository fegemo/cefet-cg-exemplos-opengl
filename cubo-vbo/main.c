#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

// configurações dos modos deste programa
#define QUANTIDADE_CUBOS 100000
#define RAIZ_QUADRADA_CUBOS 100
#define METADE_RAIZ_QUADRADA_CUBOS RAIZ_QUADRADA_CUBOS/2.0f

enum tipoArmazenamento {
  IMMEDIATE_MODE,
  VERTEX_ARRAYS,
  VERTEX_BUFFER_OBJECTS ,
  QUANTIDADE_DE_TIPOS_DE_ARMAZENAMENTO    // conterá o inteiro: 4
};
unsigned short int armazenamento = IMMEDIATE_MODE;
const char* getNomeArmazenamento(unsigned short int tipo) {
    switch (tipo) {
        case IMMEDIATE_MODE:
            return "modo imediatista";
        case VERTEX_ARRAYS:
            return "vertex array";
        case VERTEX_BUFFER_OBJECTS:
        default:
            return "VBO";
    }
}

enum modo {
    UM_CUBO,
    MUITOS_CUBOS,
    QUANTIDADE_DE_MODOS
};
unsigned short int modo = UM_CUBO;



// variáveis para cálculo e apresentação do FPS
char tituloDaJanela[200];
int momentoAnterior;
int quadrosNesteSegundo = 0;
float fps = 0;


// variáveis do cubo
// 100.000 cubos, com 108 vértices cada = ~1 milhão de vértices
GLdouble rotacaoCubo = 0;
GLuint vboCubo[1];

// cubo ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
// Um cubo tem 6 faces com 2 triângulos cada. Portanto, um cubo possui
// 36 vértices (6 faces * 2 tris * 3 vértices = 36 vértices).
// Cada vértice possui 3 floats (x,y,z), então, precisamos de 108
// floats (36 * 3 = 108).
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

    // inicializa o VBO que vai armazenar os dados do cubo
    // é importante que um VBO contenha todos os dados necessários ao desenho
    // do objeto que ele reprersenta (eg, coords, cores, normais)
    //
    // neste VBO, que será criado para o cubo, vamos colocar assim:
    // coords[v0]
    // coords[v1]
    //    ...
    // coords[vn]
    // normal[v0]
    // normal[v1]
    //    ...
    // normal[vn]
    // cor[v0]
    // cor[v1]
    //    ...
    // cor[vn]
    //
    // passo 1: pedimos para o opengl criar 1 buffer apenas
    glGenBuffers(1, vboCubo);
    // passo 2: ativamos o buffer que acabamos de criar (id = vboCubo[0])
    glBindBuffer(GL_ARRAY_BUFFER, vboCubo[0]);
    // passo 3: reservamos o espaço necessário para coords + normais + cores
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(verticesCubo) + sizeof(normaisCubo) + sizeof(coresCubo),
        0,
        GL_STATIC_DRAW);
    // passos 4-6: copiamos da RAM para VRAM (video ram) o array de coordenadas,
    //             depois o de normais e por fim o de cores, sempre "saltando"
    //             espaço para não sobrescrever o que já foi escrito
    // glBufferSubData(TIPO_DO_BUFFER, offset, tamanho, arrayComOsDados);
    glBufferSubData(GL_ARRAY_BUFFER,
        0,                                              // offset
        sizeof(verticesCubo),                           // tamanho do array
        verticesCubo);                                  // array de coords
    glBufferSubData(GL_ARRAY_BUFFER,
        sizeof(verticesCubo),                           // offset
        sizeof(normaisCubo),                            // tamanho do array
        normaisCubo);                                   // array de normais
    glBufferSubData(GL_ARRAY_BUFFER,
        sizeof(verticesCubo) + sizeof(normaisCubo),     // offset
        sizeof(coresCubo),                              // tamanho do array
        coresCubo);                                     // array de cores

    // por fim, desativamos o VBO (e só o reativamos na hora que formos
    //  desenhá-lo)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)(w)/h, 1, 10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void desenhaCubo() {
    switch (armazenamento) {
        case VERTEX_ARRAYS:
            // habilita os "vertex arrays" que vamos usar
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY);

            // especificamos onde na memória RAM o OpenGL
            // vai encontrar (a) a coordenada, (b) a normal
            // e (c) a cor de cada vértice
            glNormalPointer(GL_FLOAT, 0, normaisCubo);
            glColorPointer(3, GL_FLOAT, 0, coresCubo);
            glVertexPointer(3, GL_FLOAT, 0, verticesCubo);

            // manda desenhar GL_TRIANGLES usando 36 vértices,
            // com 3 vértices por tri (ie, 12 triângulos, 6 faces)
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            break;

        case VERTEX_BUFFER_OBJECTS:
            glBindBuffer(GL_ARRAY_BUFFER, vboCubo[0]);

            // habilita os "vertex arrays" que vamos usar
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            // como os VBOs colocam tudo junto (coords, normais e cores),
            // precisamos definir os offsets de onde começam cada um
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glNormalPointer(GL_FLOAT, 0, (void*)sizeof(verticesCubo));
            glColorPointer(3, GL_FLOAT, 0, (void*)(sizeof(verticesCubo) + sizeof(normaisCubo)));

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);

            // pára de usar este VBO (colocando o VBO com id=0 para tanto)
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;

        case IMMEDIATE_MODE:
        default:

            glBegin(GL_TRIANGLES);
              int c;
              for (c = 0; c < 108; c += 3) {
                  glNormal3fv(&normaisCubo[c]);
                  glColor3fv(&coresCubo[c]);
                  glVertex3fv(&verticesCubo[c]);
              }
            glEnd();
            break;
    }
}


void desenhaCena() {

    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;
    quadrosNesteSegundo++;

    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

        switch (modo) {
            case UM_CUBO:
            default:
                gluLookAt(0, 2.5, -5, 0, 0, 0, 0, 1, 0);
                glRotated(rotacaoCubo, 0, 1, 0);
                desenhaCubo();

                break;
            case MUITOS_CUBOS:
                gluLookAt(0, 5, -10, 0, 0, 0, 0, 1, 0);
                int i,j;
                for (i = 0; i < RAIZ_QUADRADA_CUBOS; i++) {
                    glPushMatrix();
                        glTranslatef(0, 0, (i-3)*3);
                        for (j = 0; j < RAIZ_QUADRADA_CUBOS; j++) {
                            glPushMatrix();
                                glTranslatef(j*(3) - METADE_RAIZ_QUADRADA_CUBOS, 0, 0);
                                glRotated(rotacaoCubo, 0, 1, 0);
                                desenhaCubo();
                            glPopMatrix();
                        }
                  glPopMatrix();
                }
                break;
        }
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

        case ' ':
            armazenamento = (armazenamento + 1) % QUANTIDADE_DE_TIPOS_DE_ARMAZENAMENTO;
            // soma 10° na rotação, só pra dar um efeito e mostrar que mudou
            rotacaoCubo += 10;
            glutPostRedisplay();
            break;
        case 'm':
        case 'M':
            modo = (modo + 1) % QUANTIDADE_DE_MODOS;
            // soma 10° na rotação, só pra dar um efeito e mostrar que mudou
            rotacaoCubo += 10;
            glutPostRedisplay();
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
    // calcula quantos quadros por segundo está chamando a desenha
    // usamos o MAX(delta, 1) pra evitar divisão por 0
    fps = quadrosNesteSegundo;
    quadrosNesteSegundo = 0;

    sprintf(tituloDaJanela,
        "Cubo usando %s (%.2f fps)",
        getNomeArmazenamento(armazenamento),
        fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(periodo, atualizaFPS, periodo);
}

void instrui() {
    printf("Instruções:\n");
    printf("\t'espaço'\tAlterna o modo de armazenamento\n");
    printf("\t'm'     \tAlterna entre 1 cubo e %d cubos\n", QUANTIDADE_CUBOS);
}

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(4, 6);
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

    instrui();
    inicializa();

    glutMainLoop();
    return 0;
}
