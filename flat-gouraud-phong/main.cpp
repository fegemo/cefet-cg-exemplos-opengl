#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // translate, rotate, scale, identity
#include "glm/gtc/matrix_transform.hpp" // perspective
#include "error.h"
#include "shader.h"
#include "objeto.h"
#include "luz.h"


// variáveis para cálculo e apresentação do FPS
char tituloDaJanela[200];
int momentoAnterior;
int quadrosNesteSegundo = 0;
float fps = 0;

// variáveis dos objetos
Shader *shaderFlat, *shaderGouraud, *shaderPhong;
float orientacao = 0;
enum NormaisModelo { NORMAL_POR_FACE, NORMAL_POR_VERTICE, QTDE_TIPOS_MODELO };
Objeto *quadrado[QTDE_TIPOS_MODELO], *cubo[QTDE_TIPOS_MODELO], *esfera[QTDE_TIPOS_MODELO];
Objeto**modeloAtual;
Luz luz = {glm::vec3(0, 25, 3), glm::vec3(1, 1, 1)};
enum ModeloIluminacao { PHONG, BLINN_PHONG, QTDE_MODELOS_ILUMINACAO };
ModeloIluminacao modeloIluminacao = PHONG;

Shader* inicializaShader(const char* vertexFile, const char* fragmentFile, float razaoAspecto) {
    try {
        Shader* shader = new Shader(vertexFile, fragmentFile);
        shader->ativa();
        shader->defineUniformMatriz("projecao", glm::perspective(glm::radians(60.0f), razaoAspecto, 1.0f, 10000.0f));
        shader->defineUniformBool("blinnPhong", modeloIluminacao == BLINN_PHONG);
        shader->defineUniformArray("posicaoLuz", luz.posicao);
        shader->defineUniformArray("corLuz", luz.cor);

        return shader;
    } catch(std::string error) {
        std::cout << "Erro ao carregar shader ("
            << vertexFile << ", " << fragmentFile << "): "
            << error << "\n\nSaindo do programa...\n";
        exit(1);
    }
}

void inicializaShaders() {
    float razaoAspecto = ((float)glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);

    // carrega o flat shader
    shaderFlat = inicializaShader(
        "shaders/flat-vertex.glsl",
        "shaders/flat-fragment.glsl",
        razaoAspecto);

    // carrega o gouraud shader
    shaderGouraud = inicializaShader(
         "shaders/gouraud-vertex.glsl",
         "shaders/gouraud-fragment.glsl",
        razaoAspecto);

    // carrega o phong shader
    shaderPhong = inicializaShader(
         "shaders/phong-vertex.glsl",
         "shaders/phong-fragment.glsl",
        razaoAspecto);
}

void inicializaObjetos() {
    // configura o quadrado que vamos desenhar
    glCheckError();
    quadrado[NORMAL_POR_FACE] = criaQuadrado();
    quadrado[NORMAL_POR_VERTICE] = quadrado[NORMAL_POR_FACE];

    // configura o cubo
    glCheckError();
    cubo[NORMAL_POR_FACE] = criaCubo();
    cubo[NORMAL_POR_VERTICE] = cubo[NORMAL_POR_FACE];

    // configura a esfera
    glCheckError();
    esfera[NORMAL_POR_FACE] = criaEsfera(false);
    esfera[NORMAL_POR_VERTICE] = criaEsfera(true);

    modeloAtual = cubo;
}

void inicializa() {
    glCheckError();
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    inicializaShaders();
    inicializaObjetos();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    float razaoAspecto = ((float)glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);
    glm::mat4 projecao = glm::perspective(glm::radians(60.0f), razaoAspecto, 1.0f, 10000.0f);

    shaderFlat->ativa();
    shaderFlat->defineUniformMatriz("projecao", projecao);

    shaderGouraud->ativa();
    shaderGouraud->defineUniformMatriz("projecao", projecao);

    shaderPhong->ativa();
    shaderPhong->defineUniformMatriz("projecao", projecao);
}

void desenhaCena() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    quadrosNesteSegundo++;

    // apaga a tela para desenhar de novo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 camera;
    glm::mat4 model;

    camera = glm::lookAt(
        glm::vec3(0,  5.0, 25),
        glm::vec3(0, -2.0,  0),
        glm::vec3(0,  1.0,  0)
    );

    // desenha com flat
    model = glm::translate(camera, glm::vec3(-12.5, -2.0, 0.0));
    model = glm::scale(model, glm::vec3(5));
    model = glm::rotate(model, (float)glm::radians(15.0), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, (float)glm::radians(orientacao), glm::vec3(0.0, 1.0, 0.0));
    shaderFlat->ativa();
    shaderFlat->defineUniformMatriz("modeloVisualizacao", model);
    modeloAtual[NORMAL_POR_FACE]->desenha();

    // desenha com gouraud
    model = glm::translate(camera, glm::vec3(0.0, -2.0, 0.0));
    model = glm::scale(model, glm::vec3(5));
    model = glm::rotate(model, (float)glm::radians(15.0), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, (float)glm::radians(orientacao), glm::vec3(0.0, 1.0, 0.0));
    shaderGouraud->ativa();
    shaderGouraud->defineUniformMatriz("modeloVisualizacao", model);
    modeloAtual[NORMAL_POR_VERTICE]->desenha();

    // desenha com phong
    model = glm::translate(camera, glm::vec3( 12.5, -2.0, 0.0));
    model = glm::scale(model, glm::vec3(5));
    model = glm::rotate(model, (float)glm::radians(15.0), glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, (float)glm::radians(orientacao), glm::vec3(0.0, 1.0, 0.0));
    shaderPhong->ativa();
    shaderPhong->defineUniformMatriz("modeloVisualizacao", model);
    modeloAtual[NORMAL_POR_VERTICE]->desenha();


    momentoAnterior = momentoAtual;

    // troca os buffers e manda desenhar
    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            exit(0);
            break;

        case ' ': // espaço
            if (modeloAtual == quadrado) {
                modeloAtual = cubo;
            } else if (modeloAtual == cubo) {
                modeloAtual = esfera;
            } else {
                modeloAtual = quadrado;
            }
            break;

        case 'i':
        case 'I':
            modeloIluminacao = (ModeloIluminacao) ((modeloIluminacao + 1) % QTDE_MODELOS_ILUMINACAO);

            shaderFlat->ativa();
            shaderFlat->defineUniformBool("blinnPhong", modeloIluminacao == BLINN_PHONG);
            shaderGouraud->ativa();
            shaderGouraud->defineUniformBool("blinnPhong", modeloIluminacao == BLINN_PHONG);
            shaderPhong->ativa();
            shaderPhong->defineUniformBool("blinnPhong", modeloIluminacao == BLINN_PHONG);
            break;
    }
}

void atualiza() {
    int momentoAtual = glutGet(GLUT_ELAPSED_TIME);
    int delta = momentoAtual - momentoAnterior;

    // efetivamente atualiza a rotação do cubo
    orientacao += 0.05f * delta;
    if (orientacao > 360.0f) {
      orientacao -= 360.0f;
    }

    glutPostRedisplay();
}

void atualizaFPS(int periodo) {
    fps = quadrosNesteSegundo;
    quadrosNesteSegundo = 0;

    sprintf(tituloDaJanela,
        "Flat, Gouraud e Phong (%.2f fps)",
        fps);
    glutSetWindowTitle(tituloDaJanela);
    glutTimerFunc(periodo, atualizaFPS, periodo);
}


int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitContextVersion(4, 6);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
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
