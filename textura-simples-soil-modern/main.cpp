#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "object.h"
#include "error.h"

using namespace std;


Object* square;
Shader* colorTextureShader;
GLuint idTexturaMario;

// Carrega uma textura de um arquivo
GLuint carregaTextura(const char* arquivo)
{
    // cria um id para a textura
    GLuint idTextura;
    glGenTextures(1, &idTextura);
    glBindTexture(GL_TEXTURE_2D, idTextura);

    int largura, altura, canais;
    unsigned char* texels = SOIL_load_image(arquivo, &largura, &altura, &canais, SOIL_LOAD_RGBA);

    // inverte o eixo t (a SOIL_load_OGL_texture fazia isso automaticamente...)
    for(int j = 0; j < altura / 2; j++ )
    {
        int index1 = j * largura * 4;
        int index2 = (altura - 1 - j) * largura * canais;

        for(int i = largura * canais; i > 0; i--)
        {
            unsigned char temp = texels[index1];
            texels[index1] = texels[index2];
            texels[index2] = temp;
            ++index1;
            ++index2;
        }
    }

    // envia os texels da textura para a VRAM (memória da placa de vídeo_
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largura, altura, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, texels);

    // libera a RAM que estava guardando a textura (já que ela já foi para a VRAM)
    SOIL_free_image_data(texels);

    if (texels == nullptr) {
        printf("Erro do SOIL '%s' tentando carregar o arquivo '%s'.\n", SOIL_last_result(), arquivo);
    }

    return idTextura;
}


// Inicializa configurações do OpenGL
void inicializa() {
    glCheckError();
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // configura o programa sombreador a ser usado
    colorTextureShader = new Shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    colorTextureShader->use();

    // carrega a textura do arquivo mario.png
    idTexturaMario = carregaTextura("mario.png");

    // habilita mesclagem de cores, para termos suporte a texturas (semi-)transparentes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // configura o objeto que queremos desenhar
    square = new Object(4);
    square->addVertex(Vertex{{20, 20, 0, 1}, {1, 1, 1, 1}, {0, 0}});
    square->addVertex(Vertex{{80, 20, 0, 1}, {1, 1, 1, 1}, {1, 0}});
    square->addVertex(Vertex{{80, 80, 0, 1}, {1, 1, 1, 1}, {1, 1}});
    square->addVertex(Vertex{{20, 80, 0, 1}, {1, 1, 1, 1}, {0, 1}});
    square->initialize();

    // define o valor das variáveis uniformes do shader
    colorTextureShader->setUniformMatrixVariable("projMat", glm::ortho(0.0f, 100.0f, 0.0f, 100.0f));
    colorTextureShader->setUniformMatrixVariable("modelViewMat", glm::mat4(1.0));
    colorTextureShader->setUniformUintVariable("texture", idTexturaMario);
}

// Desenha a cena
void desenhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    square->use();

    // ativa o uso de textura e configura a textura do mario
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexturaMario);

    // simplesmente manda desenhar o VAO corrente usando a conectividade
    // de GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisable(GL_TEXTURE_2D);

    glFlush();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);
}

void teclado(unsigned char key, int x, int y) {
    switch(key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);

    glutInitContextVersion(4, 6);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Carregando textura com SOIL - Pipeline Programavel");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);

    glCheckError();
    glewInit();

    inicializa();

    glutMainLoop();

    return 0;
}
