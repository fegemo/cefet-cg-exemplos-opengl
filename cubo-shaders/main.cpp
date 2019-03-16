#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // translate, rotate, scale, identity
#include "glm/gtc/matrix_transform.hpp" // perspective
#include "error.h"
#include "shader.h"

#define TRUE 1
#define FALSE 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


// 100.000 cubos, com 108 vértices cada = ~1 milhão de vértices
#define QUANTIDADE_CUBOS 100000
#define RAIZ_QUADRADA_CUBOS 100
#define METADE_RAIZ_QUADRADA_CUBOS RAIZ_QUADRADA_CUBOS/2.0f


// variáveis para cálculo e apresentação do FPS
char tituloDaJanela[200];
int momentoAnterior;
int quadrosNesteSegundo = 0;
float fps = 0;

Shader* shader;
// variáveis do cubo
GLdouble rotacaoCubo = 0;

enum modo {
    UM_CUBO,
    MUITOS_CUBOS,
    QUANTIDADE_DE_MODOS
};

unsigned short int modo = UM_CUBO;

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

void inicializaVbo() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
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


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          0, 0);
    // Ativa a variável do shader (attribute) que conterá as coordenadas
    glEnableVertexAttribArray(0);

    // Mesmo que anterior, porém agora carrega na posição da
    // variável (attribute) com índice 1
    // a 3 coordenadas (xyz) da normal do vértice
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          0,
                          (GLvoid*)sizeof(verticesCubo));
    glEnableVertexAttribArray(1);

    // idem, mas para cores (3 componentes, rgb)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          0,
                          (GLvoid*)(sizeof(verticesCubo) + sizeof(normaisCubo)));
    glEnableVertexAttribArray(2);
}

void inicializaShader() {
    float razaoAspecto = ((float)glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);
    try {
        shader = new Shader("shaders/vertex-shader.glsl", "shaders/fragment-shader.glsl");
        shader->use();
        shader->setUniformMatrixVariable("projection", glm::perspective(glm::radians(60.0f), razaoAspecto, 1.0f, 10000.0f));
        shader->setUniformMatrixVariable("modelView", glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0)));
    } catch(std::string error) {
        std::cout << "Erro ao carregar shader: " << error << "\n\nSaindo do programa...\n";
        exit(1);
    }
}

void inicializa() {
    glCheckError();
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    inicializaShader();
    inicializaVbo();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    float razaoAspecto = ((float)glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);
    shader->setUniformMatrixVariable("projection", glm::perspective(glm::radians(60.0f), razaoAspecto, 1.0f, 10000.0f));
}

void desenhaCubo() {
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void desenhaCena() {

    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    quadrosNesteSegundo++;

    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camera;
    glm::mat4 model;

    switch (modo) {
        case UM_CUBO:
        default:
            camera = glm::lookAt(
                glm::vec3(0, 2.5, -5),
                glm::vec3(0, 0.0,  0),
                glm::vec3(0, 1.0,  0)
            );
            model = glm::rotate(camera, (float)glm::radians(rotacaoCubo), glm::vec3(0.0, 1.0, 0.0));
            shader->setUniformMatrixVariable("modelView", model);
            desenhaCubo();

            break;
        case MUITOS_CUBOS:
            camera = glm::lookAt(
                glm::vec3(0, 5, -10),
                glm::vec3(0, 0,   0),
                glm::vec3(0, 1,   0)
            );

            for (int i = 0; i < RAIZ_QUADRADA_CUBOS; i++) {
                    glm::mat4 rowModel = glm::translate(camera, glm::vec3(0, 0, (i - 3) * 3));
                    for (int j = 0; j < RAIZ_QUADRADA_CUBOS; j++) {
                        model = glm::translate(rowModel, glm::vec3(j * 3 - METADE_RAIZ_QUADRADA_CUBOS, 0, 0));
                        model = glm::rotate(model, (float)glm::radians(rotacaoCubo), glm::vec3(0.0, 1.0, 0.0));
                        shader->setUniformMatrixVariable("modelView", model);
                        desenhaCubo();
                    }
            }
            break;
    }


    momentoAnterior = momentoAtual;


    // troca os buffers e manda desenhar
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
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
    fps = quadrosNesteSegundo;
    quadrosNesteSegundo = 0;

    sprintf(tituloDaJanela,
        "Cubo com shaders (%.2f fps)",
        fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(periodo, atualizaFPS, periodo);
}


int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

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
