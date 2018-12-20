#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <GL/glew.h>
#include <GL/freeglut.h>


// Nesta nossa aplicação, cada vértice terá uma coordenada (xyzw) e uma
// cor (rgba). Mas poderia conter 2 coordenadas de textura, um vetor normal (xyz)
// etc.
struct Vertex
{
    float coordinate[4];
    float color[4];
};


class Object
{

public:
    Object(Vertex*);
    Object(unsigned int);
    void use();
    Object* addVertex(Vertex);
    void initialize();

private:
    // IDs de buffers ("gavetas" na memória RAM) para armazenar os shaders,
    // os vértices do quadrado e o quadrado propriamente dito, entre outros
    //static unsigned int
    GLuint vaoId;
    GLuint vboId;
    Vertex* vertices;
    GLuint vertexCount = 0;
};



#endif
