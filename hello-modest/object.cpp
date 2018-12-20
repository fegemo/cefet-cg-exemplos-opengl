#include "object.h"

Object::Object(Vertex* vertices)
{
    this->vertices = vertices;
    this->vertexCount = sizeof(vertices) / sizeof(Vertex);
    this->initialize();
}

Object::Object(GLuint numberOfVertices)
{
    this->vertices = new Vertex[numberOfVertices];
}

Object* Object::addVertex(Vertex vertex)
{
    this->vertices[this->vertexCount] = vertex;
    this->vertexCount++;

    // retorna "this" pra possibilitar chaining de chamadas:
    // objeto.addVertex(...).addVertex(...)...
    return this;
}

void Object::initialize()
{
        // Como não há mais modo imediatista (i.e., glBegin/glEnd), precisamos usar
    // buffers com arrays de vértices (VAOs e VBOs)
    // VBO (Vertex Buffer Object): todos os vértices (coords, cores etc.)
    // que formam um objeto
    // VAO (Vertex Array Object): encapsula todas as chamadas de um VBO
    // Passos:
    //   1. Cria 1 VAO (é apenas 1 objeto: o quadrado)
    glGenVertexArrays(1, &this->vaoId);
    //   2. Cria 1 VBO (usaremos apenas 1 buffer para coordenadas e cores)
    glGenBuffers(1, &this->vboId);
    //   3. Ativamos o VAO que acaba de ser criado
    //      - Agora podemos definir propriedades do VBO, para que elas fiquem
    //        atreladas a este VAO
    glBindVertexArray(this->vaoId);
    //   4. Ativamos o VBO que guardará coords+cores do objeto
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    //   5. Associa o array de Vertex (coords+cores) ao VBO criado
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(*this->vertices) * this->vertexCount,
                 this->vertices, GL_STATIC_DRAW);

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
                          sizeof(this->vertices[0]), 0);
    // Ativa a variável do shader (attribute) que conterá as coordenadas
    glEnableVertexAttribArray(0);
    // Mesmo que anterior, porém agora carrega na posição da
    // variável (attribute) com índice 1 as 4 componentes (rgba) de cor
    // do vértice
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(this->vertices[0]),
                          (GLvoid*)sizeof(this->vertices[0].coordinate));
    glEnableVertexAttribArray(1);
}

void Object::use()
{
    glBindVertexArray(this->vaoId);
}
