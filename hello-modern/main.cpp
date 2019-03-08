#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

// Nesta nossa aplicação, cada vértice terá uma coordenada (xyzw) e uma
// cor (rgba)
struct Vertice {
    float coords[4];
    float cores[4];
};

// O OpenGL não conhece mais as matrizes de transformação e projeção
// Precisamos defini-las nós mesmos, ou usar uma biblioteca que o faça
// (e.g., glm)
struct Matriz4x4 {
    float valores[16];
};

static const Matriz4x4 MATRIZ_IDENTIDADE4x4 = {
    {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    }
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

// Não existem mais as pilhas de matrizes.
// Aqui definimos uma matriz (em vez de uma pilha) para cada uma de
// "modelView" e "projection".
static Matriz4x4
    modelViewMat = MATRIZ_IDENTIDADE4x4,
    projMat = MATRIZ_IDENTIDADE4x4;

// IDs de buffers ("gavetas" na memória RAM) para armazenar os shaders,
// os vértices do quadrado e o quadrado propriamente dito, entre outros
static unsigned int
    programId,                  // ID do conjunto formado por 1 vertex + 1 fragment shader
    vertexShaderId,             // ID do programa em GLSL do vertex shader
    fragmentShaderId,           // ID do programa em GLSL do fragment shader
    modelViewMatLoc,            // ID da "localização" da constante "modelViewMat" no código fonte do vertex shader
    projMatLoc,                 // ID da "localização" da constante "projMat" no código fonte do vertex shader
    buffer[1],
    vao[1];

// Lê um arquivo texto e retorna uma string (char*) com seu conteúdo
char* leArquivoTexto(char* caminho)
{
    FILE* arquivo = fopen(caminho, "rb");
    char* conteudo = NULL;
    long qtdeCaracteres = 0;

    fseek(arquivo, 0L, SEEK_END);
    qtdeCaracteres = ftell(arquivo);
    fseek(arquivo, 0L, SEEK_SET);
    conteudo = (char*) malloc((qtdeCaracteres+1) * sizeof(char));
    fread(conteudo, 1, qtdeCaracteres, arquivo);
    conteudo[qtdeCaracteres] = '\0';
    fclose(arquivo);
    return conteudo;
}

// Inicializa configurações do OpenGL
void inicializa(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    /////////////////////////////////////////////////////////////////
    // Um **programa shader** é formado por, pelo menos:
    //   - 1 vertex shader
    //   - 1 fragment shader
    // Passos para carregar cada um deles:
    //   1. Lê o código fonte (GLSL) do **vertex shader** no arquivo
    char* vertexShader = leArquivoTexto((char*)"shaders/vertex-shader.glsl");
    //   2. Chama glCreateShader(GL_"TIPO"_SHADER) para criar um ID (GLuint)
    //      de um buffer para armazenar este shader
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    //   3. Chama glShaderSource(...) para atrelar o código fonte lido do
    //      arquivo ao ID do shader gerado
    glShaderSource(vertexShaderId, 1,
                   (const char**) &vertexShader, NULL);
    //   4. Compila o shader
    glCompileShader(vertexShaderId);



    // Lê o código fonte (GLSL) do **fragment shader** no arquivo
    char* fragmentShader = leArquivoTexto((char*)"shaders/fragment-shader.glsl");
    // Chama glCreateShader para gerar um ID de buffer para o shader
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    // Chama glShaderSource(...) para atrelar o código fonte do shader ao
    // ID gerado
    glShaderSource(fragmentShaderId, 1,
                   (const char**) &fragmentShader, NULL);
    // Compila o shader
    glCompileShader(fragmentShaderId);


    // Agora, precisamos combinar vertex+fragment shader em um **programa**
    // Primeiro, solicitamos um ID (GLuint) para o novo programa
    programId = glCreateProgram();
    // Associa o vertex shader ao programa
    glAttachShader(programId, vertexShaderId);
    // Associa o fragment shader ao programa
    glAttachShader(programId, fragmentShaderId);
    // Faz a linkedição do programa: "junta o output do vertex com o input
    // do fragment"
    glLinkProgram(programId);
    // Ativa o programa shader que criamos
    glUseProgram(programId);
    /////////////////////////////////////////////////////////////////

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

    // Definimos uma matriz de projeção ortogonal análoga à matriz que seria
    // obtida com glOrtho(0, 100, 0, 100, -1, 1);
    Matriz4x4 projMat = {
        {
            0.02, 0.0,  0.0, -1.0,
            0.0,  0.02, 0.0, -1.0,
            0.0,  0.0, -1.0,  0.0,
            0.0,  0.0,  0.0,  1.0
        }
    };
    // Solicitamos em que posição no código fonte do programa shader está a
    // constante (uniform) chamada "projMat"
    projMatLoc = glGetUniformLocation(programId, "projMat");
    // Define a constante (uniform) projMat do shader tendo o valor dos
    // 16 valores da matriz de projeção ("projMat")
    glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, projMat.valores);
    ///////////////////////////////////////

    // Faz o mesmo para a matriz de modelo e visualização
    Matriz4x4 modelViewMat = MATRIZ_IDENTIDADE4x4;
    modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE,
                       modelViewMat.valores);
    ///////////////////////////////////////
}

// Desenha a cena
void desenhaCena(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Simplesmente manda desenhar o VAO corrente usando a conectividade
    // de TRIANGLE_STRIP
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
}

void redimensiona(int w, int h)
{
    glViewport(0, 0, w, h);
}

void teclado(unsigned char key, int x, int y)
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

    glutInitContextVersion(4, 6);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello World - Pipeline Programavel");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(teclado);

    glewExperimental = GL_TRUE;
    glewInit();

    inicializa();

    glutMainLoop();
}
