#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // translate, rotate, scale, identity
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"

using namespace std;

// Nesta nossa aplicação, cada vértice terá uma coordenada (xyzw) e uma
// cor (rgba)
struct Vertice {
    float coords[4];
    float cores[4];
};

enum buffer {VERTICES_QUADRADO};
enum objeto {QUADRADO};

// Aqui definimos os vértices (coords+cor)
static Vertice verticesDoQuadrado[] = {
    { { 20.0, 20.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } },
    { { 80.0, 20.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } },
    { { 20.0, 80.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } },
    { { 80.0, 80.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } }
};

shader *colorShader;

// IDs de buffers ("gavetas" na memória RAM) para armazenar os shaders,
// os vértices do quadrado e o quadrado propriamente dito, entre outros
static unsigned int
    buffer[1],
    vao[1];


// Inicializa configurações do OpenGL
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    colorShader = new shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    // Como não há mais modo imediatista (i.e., glBegin/glEnd), precisamos usar
    // buffers com arrays de vértices (VAOs e VBOs)
    // VBO (Vertex Buffer Object): todos os vértices (coords, cores etc.)
    // que formam um objeto
    // VAO (Vertex Array Object): encapsula todas as chamadas de um VBO
    // Passos:
    //   1. Cria 1 VAO (é apenas 1 objeto: o quadrado)
    glGenVertexArrays(1, vao);
    //   2. Cria 1 VBO (usaremos apenas 1 buffer para coordenadas e cores)
    glGenBuffers(1, buffer);
    //   3. Ativamos o VAO que acaba de ser criado
    //      - Agora podemos definir propriedades do VBO, para que elas fiquem
    //        atreladas a este VAO
    glBindVertexArray(vao[QUADRADO]);
    //   4. Ativamos o VBO que guardará coords+cores do quadrado
    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES_QUADRADO]);
    //   5. Associa o array de Vertices (coords+cores) ao VBO criado
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(verticesDoQuadrado),
                 verticesDoQuadrado, GL_STATIC_DRAW);

    // Agora, precisamos **conectar** a informação dos vértices (VBO) com o
    // programa shader. Mais especificamente, precisamos falar como o
    // vertex shader vai conseguir buscar a coordenada e a cor de cada vértice
    //
    // Argumentos:
    //  0: índice da posição da variável (attribute) do shader a que estamos
    //     conectado (veja a linha "layout(location=0) in vec4 squareCoords"
    //     do vertex shader)
    //  4: número de componentes do array que serão usadas na posição 0 (4: xyzw)
    //  GL_FLOAT: tipo de dados das componentes
    //  GL_FALSE: se o valor da componente deve ser normalizado (ficar entre 0 e 1)
    //  sizeof(vert..[0]): número de bytes a pular até chegar aos 4 componentes da
    //      próxima coordenada
    //  0: número de bytes a saltar a partir do início do array
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
                          sizeof(verticesDoQuadrado[0]), 0);
    // Ativa a variável do shader (attribute) que conterá as coordenadas
    glEnableVertexAttribArray(0);
    // Mesmo que anterior, porém agora carrega na posição da
    // variável (attribute) com índice 1 as 4 componentes (rgba) de cor
    // do vértice
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(verticesDoQuadrado[0]),
                          (GLvoid*)sizeof(verticesDoQuadrado[0].coords));
    glEnableVertexAttribArray(1);
    ///////////////////////////////////////

    colorShader->use();

    glm::mat4 projection = glm::ortho(
            0.0f,
            100.0f,
            0.0f,
            100.0f,
            -1.0f,
            1.0f
        );
    colorShader->setUniformMatrixVariable("projMat", projection);
    colorShader->setUniformMatrixVariable("modelViewMat", glm::mat4(1.0));
}

// Desenha a cena
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Simplesmente manda desenhar o VAO corrente usando a conectividade
    // de TRIANGLE_STRIP
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void keyInput(unsigned char key, int x, int y)
{
    switch(key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World - Pipeline Programável");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
