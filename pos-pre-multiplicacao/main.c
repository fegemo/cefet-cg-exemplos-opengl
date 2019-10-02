#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "textura.h"
#include "texto.h"
#include "imprime-matriz.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define TRUE 1
#define FALSE 0

int larguraJanela;
int alturaJanela;
const float margemViewport = 0.5f;
const float tamanhoVertice = 5;
const int tamanhoMundo = 10;
GLuint texturaUnicornio;

enum PassosExecucao {
    IDENTIDADE,
    ROTACAO,
    TRANSLACAO,
    ESCALA,
    QTDE_PASSOS
};

enum PassosExecucao passoAtual = IDENTIDADE;

struct Vertice {
    float x, y;
    float xResultante, yResultante;
    GLuint selecionado;
    char textoCoordenadas[50];
};

struct Vertice pos0, pos1, pos2, pos3;
struct Vertice pre0, pre1, pre2, pre3;
struct Vertice** vertices;

// Inicia algumas variáveis de estado
void inicializa(void)
{
    glClearColor(1, 1, 1, 1);

    // habilita mesclagem de cores, para termos suporte a texturas
    // com transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // configura desenho de pontos como círculos com certo raio
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1+tamanhoVertice);

    texturaUnicornio = carregaTextura("unicornio.png");

    // inicializa os vértices
    pos0.x = 0; pos0.y = 0; pos0.xResultante = 1.73; pos0.yResultante = 1.00; pos0.selecionado = FALSE;
    pos1.x = 2; pos1.y = 0; pos1.xResultante = 2.59; pos1.yResultante = 1.50; pos1.selecionado = FALSE;
    pos2.x = 2; pos2.y = 2; pos2.xResultante = 2.09; pos2.yResultante = 2.36; pos2.selecionado = FALSE;
    pos3.x = 0; pos3.y = 2; pos3.xResultante = 1.23; pos3.yResultante = 1.86; pos3.selecionado = FALSE;
    pre0.x = 0; pre0.y = 0; pre0.xResultante = 1.00; pre0.yResultante = 0.00; pre0.selecionado = FALSE;
    pre1.x = 2; pre1.y = 0; pre1.xResultante = 1.87; pre1.yResultante = 0.50; pre1.selecionado = FALSE;
    pre2.x = 2; pre2.y = 2; pre2.xResultante = 1.37; pre2.yResultante = 1.37; pre2.selecionado = FALSE;
    pre3.x = 0; pre3.y = 2; pre3.xResultante = 0.50; pre3.yResultante = 0.87; pre3.selecionado = FALSE;

    vertices = (struct Vertice**)malloc(8 * sizeof(struct Vertice*));
    vertices[0] = &pos0;
    vertices[1] = &pos1;
    vertices[2] = &pos2;
    vertices[3] = &pos3;
    vertices[4] = &pre0;
    vertices[5] = &pre1;
    vertices[6] = &pre2;
    vertices[7] = &pre3;
}

void configuraViewport(int indice, int total)
{
    int larguraViewport = larguraJanela / total;
    int alturaViewport = larguraViewport;
    float viewportX = indice * larguraViewport;
    if (larguraJanela / ((float)alturaJanela) > 3) {
        larguraViewport = alturaJanela;
        alturaViewport = alturaJanela;
    }
    float viewportY = MAX(0, (alturaJanela - alturaViewport) / 2);
    glViewport(viewportX, viewportY, larguraViewport, alturaViewport);

    // configura a projeção ortogonal
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        -margemViewport, tamanhoMundo + margemViewport, // left, right
        -margemViewport, tamanhoMundo + margemViewport, // bottom, top
        -1, 1                                           // near, far
    );

    // muda para a pilha de matrizes modelView
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void imprimePassoEMatriz()
{
    char* nomePassoAtual = (char*) malloc(sizeof(char) * 50);
    switch (passoAtual)
    {
        case IDENTIDADE:
            strncpy(nomePassoAtual, "Identidade", 50);
            break;
        case ROTACAO:
            strncpy(nomePassoAtual, "Identidade x Rotação", 50);
            break;
        case TRANSLACAO:
            strncpy(nomePassoAtual, "Identidade x Rotação x Translação", 50);
            break;
        case ESCALA:
            strncpy(nomePassoAtual, "Identidade x Rotação x Translação x Escala", 50);
            break;
        default:
            break;

    }
    printf("Passo: %s\n", nomePassoAtual);
    imprimeMatrizOpenGL(GL_MODELVIEW_MATRIX);
    printf("\n");
}

float* determinaPosicaoVertices(int multiplicadorIndice)
{
    int indiceV0 = multiplicadorIndice * 4;
    int indiceV3 = (multiplicadorIndice+1) * 4 - 1;

    // pega a matriz atual do topo da MODEL_VIEW
    float* m = pegaMatrizOpenGL(GL_MODELVIEW_MATRIX);

    for (int c = indiceV0; c <= indiceV3; c++)
    {
        int x = vertices[c]->x;
        int y = vertices[c]->y;
        int z = 1;

        // multiplica as coordenadas do vértice pela matriz
        vertices[c]->xResultante = m[0]*x+m[1]*y+m[3]*z;
        vertices[c]->yResultante = m[4]*x+m[5]*y+m[7]*z;

        sprintf(vertices[c]->textoCoordenadas, "(%.2f, %.2f)", vertices[c]->xResultante, vertices[c]->yResultante);
    }

    return m;
}

void desenhaEixos()
{
    glColor3f(0, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        // linhas
        glBegin(GL_LINES);
            glVertex2f(0, 0);
            glVertex2f(tamanhoMundo - 2*margemViewport, 0);
            glVertex2f(0, 0);
            glVertex2f(0, tamanhoMundo - 2*margemViewport);
        glEnd();

        // ticks horizontais, depois verticais
        glBegin(GL_LINES);
        for (int c = 1; c < tamanhoMundo-1; c++)
        {
            // horizontais
            glVertex2f(c, -0.1);
            glVertex2f(c,  0.1);

            // verticais
            glVertex2f(-0.1, c);
            glVertex2f( 0.1, c);
        }
        glEnd();

        // triangulim
        glBegin(GL_TRIANGLES);
            glVertex2f(tamanhoMundo - 2*margemViewport, 0);
            glVertex2f(tamanhoMundo - 2*margemViewport - 0.6,  0.15);
            glVertex2f(tamanhoMundo - 2*margemViewport - 0.6, -0.15);
        glEnd();
        glRotatef(90, 0, 0, 1);
        glBegin(GL_TRIANGLES);
            glVertex2f(tamanhoMundo - 2*margemViewport, 0);
            glVertex2f(tamanhoMundo - 2*margemViewport - 0.6,  0.15);
            glVertex2f(tamanhoMundo - 2*margemViewport - 0.6, -0.15);
        glEnd();
    glPopMatrix();
}

void desenhaUnicornioNaOrigem()
{
    // desenha 2x: uma preenchido e c/ textura, outra wireframe
    GLuint modos[2] = {GL_FILL, GL_LINE};
    for(int i = 0; i < 2; i++)
    {
        glPolygonMode(GL_FRONT, modos[i]);
        if (modos[i] == GL_FILL)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texturaUnicornio);
            glColor3f(1, 1, 1);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glColor4f(0, 0, 0, 0.25);
        }

        // desenha os vértices com suas coordenadas de textura
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0);
            glVertex2i(0, 0);

            glTexCoord2f(1, 0);
            glVertex2i(2, 0);

            glTexCoord2f(1, 1);
            glVertex2i(2, 2);

            glTexCoord2f(0, 1);
            glVertex2i(0, 2);
        glEnd();

    }

    // desenha círculos nos vértices
    glBegin(GL_POINTS);
        glColor4f(0.5, 0.5, 1, pos0.selecionado == TRUE || pre0.selecionado == TRUE ? 1 : 0.35);
        glVertex2f(pos0.x, pos0.y);

        glColor4f(0.5, 0.5, 1, pos1.selecionado == TRUE || pre1.selecionado == TRUE ? 1 : 0.35);
        glVertex2f(pos1.x, pos1.y);

        glColor4f(0.5, 0.5, 1, pos2.selecionado == TRUE || pre2.selecionado == TRUE ? 1 : 0.35);
        glVertex2f(pos2.x, pos2.y);

        glColor4f(0.5, 0.5, 1, pos3.selecionado == TRUE || pre3.selecionado == TRUE ? 1 : 0.35);
        glVertex2f(pos3.x, pos3.y);
    glEnd();
}

void desenhaValoresVerticesSelecionados()
{
    for (int v = 0; v < 2; v++)
    {
        configuraViewport(v, 2);

        // desenha os valores (x,y) dos vértices selecionados nos eixos
        for (int c = 0; c < 4; c++)
        {
            int selecionado = vertices[c]->selecionado == TRUE || vertices[c+4]->selecionado == TRUE;
            if (selecionado == TRUE)
            {
                // texto: (x,y)
                glColor3f(0.2, 0.2, 0.2);
                escreveTexto(GLUT_BITMAP_8_BY_13, vertices[c+v*4]->textoCoordenadas, vertices[c+v*4]->xResultante, vertices[c+v*4]->yResultante, 0);

                // "réguas"
                glColor3f(0.8, 0.8, 0.8);
                glLineStipple(1, 0xAAAA);           // linha tracejada
                glEnable(GL_LINE_STIPPLE);          // idem

                glBegin(GL_LINES);
                    glVertex2f(vertices[c+v*4]->xResultante, 0);
                    glVertex2f(vertices[c+v*4]->xResultante, vertices[c+v*4]->yResultante);
                    glVertex2f(0, vertices[c+v*4]->yResultante);
                    glVertex2f(vertices[c+v*4]->xResultante, vertices[c+v*4]->yResultante);
                glEnd();

                glDisable(GL_LINE_STIPPLE);
            }
        }
    }
}

void desenhaMatriz(float *m, int ehPosMultiplicacao)
{
    // escreve qual é o passo atual
    glColor3f(0.8, 0.8, 0.8);
    char textoMatrizResultante[100];
    switch (passoAtual)
    {
        case IDENTIDADE:
            sprintf(textoMatrizResultante, "Matriz: I");
            break;
        case ROTACAO:
            sprintf(textoMatrizResultante, ehPosMultiplicacao == TRUE ? "Matriz: I x R" : "Matriz: R x I");
            break;
        case TRANSLACAO:
            sprintf(textoMatrizResultante, ehPosMultiplicacao == TRUE ? "Matriz: I x R x T" : "Matriz: T x R x I");
            break;
        case ESCALA:
            sprintf(textoMatrizResultante, ehPosMultiplicacao == TRUE ? "Matriz: I x R x T x S" : "Matriz: S x T x R x I");
            break;
        default:
            break;
    }
    escreveTexto(GLUT_BITMAP_8_BY_13, textoMatrizResultante, 2, 8.5, 0);

    // escreve a matriz
    char textoMatriz[100];
    sprintf(textoMatriz, "| %- 3.2f  %- 3.2f  %- 3.2f |", m[0], m[1], m[3]);
    escreveTexto(GLUT_BITMAP_8_BY_13, textoMatriz, 2, 8.0, 0);
    sprintf(textoMatriz, "| %- 3.2f  %- 3.2f  %- 3.2f |", m[4], m[5], m[7]);
    escreveTexto(GLUT_BITMAP_8_BY_13, textoMatriz, 2, 7.5, 0);
    sprintf(textoMatriz, "| %- 3.2f  %- 3.2f  %- 3.2f |", m[12], m[13], m[15]);
    escreveTexto(GLUT_BITMAP_8_BY_13, textoMatriz, 2, 7.0, 0);
}

void desenhaCena()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);

    // desenha unicórnio com PÓS-multiplicação
    configuraViewport(0, 2);
    desenhaEixos();
    glLoadIdentity();                 // 1. carrega identidade
    if (passoAtual >= ROTACAO)
    {
        glRotatef(30, 0, 0, 1);         // 2. rotação 30º
        if (passoAtual >= TRANSLACAO)
        {
            glTranslatef(2, 0, 0);        // 3. translação (2,0)
            if (passoAtual >= ESCALA)
            {
                glScalef(0.5f, 0.5f, 0.5f); // 4. escala 0.5
            }
        }
    }
    desenhaUnicornioNaOrigem();
    float *matriz = determinaPosicaoVertices(0);
    glLoadIdentity();
    desenhaMatriz(matriz, TRUE);
    free(matriz);


    // desenha unicórnio com PRÉ-multiplicação
    configuraViewport(1, 2);
    desenhaEixos();
    glLoadIdentity();

    if (passoAtual >= ROTACAO)
    {
        glRotatef(30, 0, 0, 1);
        if (passoAtual >= TRANSLACAO)
        {
            // glTranslatef(2, 0, 0);
            float translacaoXrotacao[] = {0.87, 0.5, 0, 0, -0.5, 0.87, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1};
            glLoadMatrixf(translacaoXrotacao);
            if (passoAtual >= ESCALA)
            {
                // glScalef(0.5f, 0.5f, 0.5f);
                float escalaXtranslacaoXrotacao[] = {0.43, 0.25, 0, 0, -0.25, 0.43, 0, 0, 0, 0, 0.5, 0, 1, 0, 0, 1};
                glLoadMatrixf(escalaXtranslacaoXrotacao);
            }
        }
    }
    desenhaUnicornioNaOrigem();
    matriz = determinaPosicaoVertices(1);
    glLoadIdentity();
    desenhaMatriz(matriz, FALSE);
    free(matriz);

    // desenha os valores (x,y) e réguas dos vértices selecionados
    desenhaValoresVerticesSelecionados();

    // Diz ao OpenGL para colocar o que desenhamos na tela
    glutSwapBuffers();
}

// Callback de evento de teclado
void teclado(unsigned char key, int x, int y)
{
    switch(key)
    {
        // Tecla ESC
        case 27:
            exit(0);
            break;
        case '+':
            passoAtual = MIN(passoAtual + 1, QTDE_PASSOS - 1);
            glutPostRedisplay();
            break;
        case '-':
            passoAtual = passoAtual > 0 ? passoAtual - 1 : 0;
            glutPostRedisplay();
            break;
    }
}

void movimentoMouse(int x, int y)
{
    int larguraJanela = glutGet(GLUT_WINDOW_WIDTH)/2;
    int alturaJanela = glutGet(GLUT_WINDOW_HEIGHT);
    float xMundo = ((float)x/larguraJanela) * (tamanhoMundo + 2*margemViewport) - margemViewport;
    float yMundo = tamanhoMundo - ((float)y/alturaJanela) * (tamanhoMundo + 2*margemViewport) + margemViewport;
    int precisaRedesenhar = FALSE;

    // percorre a lista de vértices verificando se o mouse está sobre algum
    for (int c = 0; c < 4; c++)
    {
        float distanciaMouseAteVertice = sqrt(pow(xMundo - vertices[c]->xResultante, 2) + pow(yMundo - vertices[c]->yResultante, 2));
        int selecionado = distanciaMouseAteVertice <= 0.5 ? TRUE : FALSE;
        if (selecionado != vertices[c]->selecionado)
        {
            precisaRedesenhar = TRUE;
            vertices[c]->selecionado = selecionado;
        }
    }

    // para a segunda viewport, reduz meia largura
    xMundo -= (tamanhoMundo + 2*margemViewport);

    for (int c = 4; c < 8; c++)
    {
        float distanciaMouseAteVertice = sqrt(pow(xMundo - vertices[c]->xResultante, 2) + pow(yMundo - vertices[c]->yResultante, 2));
        int selecionado = distanciaMouseAteVertice <= 0.5 ? TRUE : FALSE;
        if (selecionado != vertices[c]->selecionado)
        {
            precisaRedesenhar = TRUE;
            vertices[c]->selecionado = selecionado;
        }
    }

    if (precisaRedesenhar == TRUE)
    {
        glutPostRedisplay();
    }
}

void redimensionada(int w, int h)
{
    larguraJanela = w;
    alturaJanela = h;
}

// Rotina principal
int main(int argc, char **argv)
{
    // Configuração inicial da janela do GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 250);
    glutInitWindowPosition(100, 100);

    // Abre a janela
    glutCreateWindow("Pós vs pré-multiplicação");

    // Registra callbacks para alguns eventos
    glutDisplayFunc(desenhaCena);
    glutKeyboardFunc(teclado);
    glutPassiveMotionFunc(movimentoMouse);
    glutReshapeFunc(redimensionada);
    inicializa();

    // Entra em loop e nunca sai
    glutMainLoop();
    return 0;
}
