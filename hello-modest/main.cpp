#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp" // translate, rotate, scale, identity
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "object.h"
#include "error.h"


using namespace std;


Object* square;
Shader* colorShader;


// Inicializa configurações do OpenGL
void inicializa(void)
{
    glClearColor(1, 1, 1, 1);
    glCheckError();

    // configura o programa sombreador a ser usado
    colorShader = new Shader("shaders/vertex-shader.glsl", "shaders/fragment-shader.glsl");
    colorShader->use();

    // configura o objeto que queremos desenhar
    square = new Object(4);
    square->addVertex(Vertex{{20, 20, 0, 1}, {0, 1, 0, 1}});
    square->addVertex(Vertex{{80, 20, 0, 1}, {0, 1, 0, 1}});
    square->addVertex(Vertex{{80, 80, 0, 1}, {0, 1, 0, 1}});
    square->addVertex(Vertex{{20, 80, 0, 1}, {0, 1, 0, 1}});
    square->initialize();

    // define o valor das variáveis uniformes do shader
    colorShader->setUniformMatrixVariable("projMat", glm::ortho(0.0f, 100.0f, 0.0f, 100.0f));
    colorShader->setUniformMatrixVariable("modelViewMat", glm::mat4(1.0));
}

// Desenha a cena
void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // ativa o VAO referente ao objeto que queremos desenhar
    square->use();

    // simplesmente manda desenhar o VAO corrente usando a conectividade
    // de GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glFlush();
}

void redimensiona(int w, int h)
{
    glViewport(0, 0, w, h);
}

void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
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

    glutInitContextVersion(4, 6);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World - Pipeline Programável");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);

    glCheckError();
    glewExperimental = GL_TRUE;
    glewInit();

    inicializa();

    glutMainLoop();

    return 0;
}
