#include "objeto.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

Objeto::Objeto(GLuint qtdeVertices, GLuint primitiva)
    : Objeto::Objeto(qtdeVertices, 0, primitiva) {
}

Objeto::Objeto(GLuint qtdeVertices, GLuint qtdeIndices, GLuint primitiva) {
    this->vertices = new Vertice[qtdeVertices];
    this->indices = new Indice[qtdeIndices];
    this->primitiva = primitiva;

    this->maximoVertices = qtdeVertices;
    this->maximoIndices = qtdeIndices;
}

Objeto* Objeto::novoVertice(Vertice vertice) {
    if (this->qtdeVertices > this->maximoVertices) {
        stringstream error;
        error << "Pediu para colocar mais um vértice mas o máximo era  " << this->maximoVertices;
        throw error.str();
    }
    this->vertices[this->qtdeVertices] = vertice;
    this->qtdeVertices++;

    // retorna "this" pra possibilitar chaining de chamadas:
    // objeto.novoVertice(...).novoVertice(...)...
    return this;
}

Objeto* Objeto::novoIndice(Indice index) {
    if (this->qtdeIndices > this->maximoIndices) {
        stringstream error;
        error << "Pediu para colocar mais um índice mas o máximo era  " << this->maximoIndices;
        throw error.str();
    }
    this->indices[this->qtdeIndices] = index;
    this->qtdeIndices++;

    // retorna "this" pra possibilitar chaining de chamadas:
    // objeto.novoVertice(...).novoVertice(...)...
    return this;
}

void Objeto::inicializa() {
    // Como não há mais modo imediatista (i.e., glBegin/glEnd), precisamos usar
    // buffers com arrays de vértices (VAOs e VBOs)
    // VBO (Vertice Buffer Objeto): todos os vértices (coords, cores etc.)
    // que formam um objeto
    // VAO (Vertice Array Objeto): encapsula todas as chamadas de um VBO
    // Passos:
    //   1. Cria 1 VAO (é apenas 1 objeto)
    glGenVertexArrays(1, &this->vaoId);
    //   2. Cria 1 VBO (usaremos apenas 1 buffer para coordenadas e cores)
    glGenBuffers(1, &this->vboId);
    //   3. Ativamos o VAO que acaba de ser criado
    //      - Agora podemos definir propriedades do VBO, para que elas fiquem
    //        atreladas a este VAO
    glBindVertexArray(this->vaoId);
    //   4. Ativamos o VBO que guardará coords+cores do objeto
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    //   5. Associa o array de Vertice (coords+normal+cor) ao VBO criado
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(*this->vertices) * this->qtdeVertices,
                 this->vertices, GL_STATIC_DRAW);

    // Agora, precisamos **conectar** a informação dos vértices (VBO) com o
    // programa shader. Mais especificamente, precisamos falar como o
    // vertice shader vai conseguir buscar a coordenada e a cor de cada vértice
    //
    // Argumentos:
    //  0: índice da posição da variável (attribute) do shader a que estamos
    //     conectado (veja a linha "layout(location=0) in vec4 squareCoords"
    //     do vertice shader)
    //  4: número de componentes do array que serão usadas na posição 0 (4: xyzw)
    //  GL_FLOAT: tipo de dados das componentes
    //  GL_FALSE: se o valor da componente deve ser normalizado (ficar entre 0 e 1)
    //  sizeof(vert..[0]): número de bytes a pular até chegar aos 4 componentes da
    //      próxima coordenada
    //  0: número de bytes a saltar a partir do início do array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(this->vertices[0]), 0);
    // Ativa a variável do shader (attribute) que conterá as coordenadas
    glEnableVertexAttribArray(0);
    // Mesmo que anterior, porém agora carrega na posição da
    // variável (attribute) com índice 1 as 3 coordenadas (xyz) da normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(this->vertices[0]),
                          (GLvoid*)(sizeof(this->vertices[0].coordenada)));
    glEnableVertexAttribArray(1);
    // Mesmo que anterior, porém agora carrega na posição da
    // variável (attribute) com índice 2 as 4 componentes (rgba) de cor
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
                          sizeof(this->vertices[0]),
                          (GLvoid*)(
                              sizeof(this->vertices[0].coordenada) +
                              sizeof(this->vertices[0].normal)
                          ));
    glEnableVertexAttribArray(2);


    // Em caso de este ser um objeto que está sendo desenhado com índices
    // (glDrawElements) em vez de vértice a vértice (glDrawArrays), é necessário
    // ter um vbo para os índices
    if (this->usaIndices()) {
        glGenBuffers(1, &this->iboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(this->indices[0]) * this->qtdeIndices,
            this->indices,
            GL_STATIC_DRAW);
    }
}

void Objeto::desenha() {
    glBindVertexArray(this->vaoId);

    if (this->usaIndices()) {
        glDrawElements(this->primitiva, this->qtdeIndices, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(this->primitiva, 0, this->qtdeVertices);
    }
}

bool Objeto::usaIndices() {
    return this->qtdeIndices > 0;
}


Objeto* criaQuadrado() {
    Objeto *obj = new Objeto(4, GL_TRIANGLE_FAN);
    obj->novoVertice(Vertice{{-0.5, -0.5, 0}, {0, 0, 1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5, -0.5, 0}, {0, 0, 1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5, 0}, {0, 0, 1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5, 0}, {0, 0, 1}, {0, 1, 0, 1}});
    obj->inicializa();

    return obj;
}

Objeto* criaCubo() {
    Objeto *obj = new Objeto(24, 36, GL_TRIANGLES);

    // vértices da face da frente
    obj->novoVertice(Vertice{{-0.5, -0.5,  0.5}, {0, 0,  1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5, -0.5,  0.5}, {0, 0,  1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5,  0.5}, {0, 0,  1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5,  0.5}, {0, 0,  1}, {0, 1, 0, 1}});
    // vértices da face de trás
    obj->novoVertice(Vertice{{ 0.5, -0.5, -0.5}, {0, 0, -1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5, -0.5, -0.5}, {0, 0, -1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5, -0.5}, {0, 0, -1}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5, -0.5}, {0, 0, -1}, {0, 1, 0, 1}});
    // vértices da face da direita
    obj->novoVertice(Vertice{{ 0.5, -0.5,  0.5}, { 1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5, -0.5, -0.5}, { 1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5, -0.5}, { 1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5,  0.5}, { 1, 0, 0}, {0, 1, 0, 1}});
    // vértices da face da esquerda
    obj->novoVertice(Vertice{{-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5, -0.5,  0.5}, {-1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5,  0.5}, {-1, 0, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5, -0.5}, {-1, 0, 0}, {0, 1, 0, 1}});
    // vértices da face de cima
    obj->novoVertice(Vertice{{-0.5,  0.5,  0.5}, {0,  1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5,  0.5}, {0,  1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5,  0.5, -0.5}, {0,  1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5,  0.5, -0.5}, {0,  1, 0}, {0, 1, 0, 1}});
    // vértices da face de baixo
    obj->novoVertice(Vertice{{-0.5, -0.5, -0.5}, {0, -1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5, -0.5, -0.5}, {0, -1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{ 0.5, -0.5,  0.5}, {0, -1, 0}, {0, 1, 0, 1}});
    obj->novoVertice(Vertice{{-0.5, -0.5,  0.5}, {0, -1, 0}, {0, 1, 0, 1}});

    // índices dos triângulos
    obj->novoIndice(0);
    obj->novoIndice(1);
    obj->novoIndice(2);
    obj->novoIndice(0);
    obj->novoIndice(2);
    obj->novoIndice(3);

    obj->novoIndice(4);
    obj->novoIndice(5);
    obj->novoIndice(6);
    obj->novoIndice(4);
    obj->novoIndice(6);
    obj->novoIndice(7);

    obj->novoIndice(8);
    obj->novoIndice(9);
    obj->novoIndice(10);
    obj->novoIndice(8);
    obj->novoIndice(10);
    obj->novoIndice(11);

    obj->novoIndice(12);
    obj->novoIndice(13);
    obj->novoIndice(14);
    obj->novoIndice(12);
    obj->novoIndice(14);
    obj->novoIndice(15);

    obj->novoIndice(16);
    obj->novoIndice(17);
    obj->novoIndice(18);
    obj->novoIndice(16);
    obj->novoIndice(18);
    obj->novoIndice(19);

    obj->novoIndice(20);
    obj->novoIndice(21);
    obj->novoIndice(22);
    obj->novoIndice(20);
    obj->novoIndice(22);
    obj->novoIndice(23);

    obj->inicializa();

    return obj;

}

#include <math.h>
#include "glm/glm.hpp"

Objeto* criaEsfera(bool normaisSuaves) {
    const GLushort stacks = 12;   // latitude
    const GLushort sectors = 24;  // longitude

    // normaisSuaves true: uma normal por vértice (pode usar índices)
    Objeto *obj = nullptr;
    if (normaisSuaves) {
        const GLushort qtdeVertices = (stacks+1) * (sectors+1);
        const GLushort qtdeIndices = (stacks-1) * (sectors) * 6;
        obj = new Objeto(qtdeVertices, qtdeIndices, GL_TRIANGLES);

        float stackStep = M_PI / stacks;
        float sectorStep = M_PI*2.0f / sectors;

        for (GLushort i = 0; i <= stacks; i++) {
            // começa com phi = 90º e vai "descendo"
            float stackAngle = M_PI_2 - i * stackStep;
            float y = sinf(stackAngle);
            float xz = cosf(stackAngle);

            // cria (sectors + 1) vértices para cada stack
            for (GLushort j = 0; j <= sectors; j++) {
                float sectorAngle = j * sectorStep;

                // acha as coordenadas do vértice
                float x = xz * cosf(sectorAngle);
                float z = xz * sinf(sectorAngle);

                // acha as coordenadas da normal
                float nx = x;
                float ny = y;
                float nz = z;

                obj->novoVertice(Vertice{{x, y, z}, {nx, ny, nz}, {0, 1, 0, 1}});
            }
        }


        // com os vértices criados, agora devemos criar o array de índices
        for (GLushort i = 0; i < stacks; i++) {

            // acha o índice do início da stack atual e o da stack seguinte
            GLushort stack1 = i * (sectors + 1);
            GLushort stack2 = stack1 + (sectors + 1);

            for (GLushort j = 0; j < sectors; j++, stack1++, stack2++) {

                // se estiver na stack do alto, não tem este triângulo
                if (i != 0) {
                    // v0-v1
                    // | /
                    // v2
                    obj->novoIndice(stack1);
                    obj->novoIndice(stack1 + 1);
                    obj->novoIndice(stack2);
                }

                // se estiver na stack de baixo, não tem este triângulo
                if (i != stacks - 1) {
                    //    v0
                    //  /  |
                    // v2 v1
                    obj->novoIndice(stack1 + 1);
                    obj->novoIndice(stack2 + 1);
                    obj->novoIndice(stack2);
                }
            }
        }
    }

    // normaisSuaves false: uma normal por face (não pode usar índices)
    else {
        const GLushort qtdeVertices = (stacks - 1) * (sectors) * 6;
        obj = new Objeto(qtdeVertices, GL_TRIANGLES);

        float stackStep = M_PI / stacks;
        float sectorStep = M_PI*2.0f / sectors;

        for (GLushort i = 0; i < stacks; i++) {
            // começa com phi = 90º e vai "descendo"
            float stackAngle = M_PI_2 - i * stackStep;
            float stackAngleNext = stackAngle - stackStep;

            float y0 = sinf(stackAngle);
            float xz0 = cosf(stackAngle);
            float y1 = sinf(stackAngleNext);
            float xz1 = cosf(stackAngleNext);

            // cria (sectors + 1) vértices para cada stack
            for (GLushort j = 0; j < sectors; j++) {
                float sectorAngle = j * sectorStep;
                float sectorAngleNext = sectorAngle + sectorStep;

                // coordenadas da normal da face
                glm::vec3 normal;

                // se estiver na stack do alto, não tem este triângulo
                if (i != 0) {
                    // v0-v1
                    // | /
                    // v2
                    // acha as coordenadas do vértice v0
                    float x0 = xz0 * cosf(sectorAngle);
                    float z0 = xz0 * sinf(sectorAngle);

                    // acha as coordenadas do vértice v1
                    float x1 = xz0 * cosf(sectorAngleNext);
                    float z1 = xz0 * sinf(sectorAngleNext);

                    // acha as coordenadas do vértice v2
                    float x2 = xz1 * cosf(sectorAngle);
                    float z2 = xz1 * sinf(sectorAngle);

                    // acha as coordenadas da normal da face
                    glm::vec3 v0(x0, y0, z0);
                    glm::vec3 v1(x1, y0, z1);
                    glm::vec3 v2(x2, y1, z2);
                    normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                    // adiciona os 3 vértices deste triângulo
                    obj->novoVertice(Vertice{{x0, y0, z0}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                    obj->novoVertice(Vertice{{x1, y0, z1}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                    obj->novoVertice(Vertice{{x2, y1, z2}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                }

                // se estiver na stack de baixo, não tem este triângulo
                if (i != stacks - 1) {
                    //    v0
                    //  /  |
                    // v2-v1
                    // acha as coordenadas do vértice v0
                    float x0 = xz0 * cosf(sectorAngleNext);
                    float z0 = xz0 * sinf(sectorAngleNext);

                    // acha as coordenadas do vértice v1
                    float x1 = xz1 * cosf(sectorAngleNext);
                    float z1 = xz1 * sinf(sectorAngleNext);

                    // acha as coordenadas do vértice v2
                    float x2 = xz1 * cosf(sectorAngle);
                    float z2 = xz1 * sinf(sectorAngle);

                    if (i == 0) {
                        glm::vec3 v0(x0, y0, z0);
                        glm::vec3 v1(x1, y1, z1);
                        glm::vec3 v2(x2, y1, z2);
                        normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                    }

                    // adiciona os 3 vértices deste triângulo
                    obj->novoVertice(Vertice{{x0, y0, z0}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                    obj->novoVertice(Vertice{{x1, y1, z1}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                    obj->novoVertice(Vertice{{x2, y1, z2}, {normal.x, normal.y, normal.z}, {0, 1, 0, 1}});
                }
            }
        }
    }

    obj->inicializa();
    
    return obj;
}
