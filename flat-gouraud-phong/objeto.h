#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/glew.h>
#include <GL/freeglut.h>


// Nesta nossa aplicação, cada vértice terá uma coordenada (xyz), um
// vetor normal (xyz) e uma cor (rgba)
struct Vertice {
    float coordenada[3];
    float normal[3];
    float cor[4];
};

typedef GLuint Indice;

class Objeto {

public:
    Objeto(GLuint, GLuint);
    Objeto(GLuint, GLuint, GLuint);
    void desenha();
    Objeto* novoVertice(Vertice);
    Objeto* novoIndice(Indice);
    void inicializa();

private:
    GLuint primitiva = GL_TRIANGLE_FAN;
    // IDs de buffers ("gavetas" na memória RAM) para armazenar os shaders,
    // os vértices do quadrado e o quadrado propriamente dito, entre outros
    //static unsigned int
    GLuint vaoId;
    GLuint vboId;
    Vertice* vertices;
    GLuint qtdeVertices = 0;
    GLuint maximoVertices;
    GLuint iboId;
    Indice* indices;
    GLuint qtdeIndices = 0;
    GLuint maximoIndices;
    bool mostraArame = false;

    bool usaIndices();
};


Objeto* criaQuadrado();
Objeto* criaCubo();
Objeto* criaEsfera(bool);

#endif
