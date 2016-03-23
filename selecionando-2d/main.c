#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "configuracoes.h"
#include "icone.h"
#include "lista-encadeada.h"
#include "geometria.h"

struct lista *lista;
struct no *noSelecionado;
ponto posicaoMouseNoMundo;

// Rotina de desenho
void desenhaMinhaCena() {
    glClear(GL_COLOR_BUFFER_BIT);

    struct no* noCorrente = lista->inicio;
    while(noCorrente != NULL) {
        desenhaIcone(*((struct icone*)noCorrente->objeto));
        noCorrente = noCorrente->proximo;
    }

    glFlush();
}

void iniciaPrograma() {
    lista = (struct lista*)malloc(sizeof(struct lista));
    inicializaLista(lista);
}

// Inicia algumas variáveis de estado do OpenGL
void iniciaOpenGL() {
    // define qual é a cor do fundo
    glClearColor(1.0, 1.0, 1.0, 0.0); // branco
}

void redimensionada(int width, int height) {
    float razaoAspecto = (float)width/(float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(razaoAspecto * -TAMANHO_HORIZONTAL_MUNDO_SOBRE_2, razaoAspecto* TAMANHO_HORIZONTAL_MUNDO_SOBRE_2,
            -TAMANHO_VERTICAL_MUNDO_SOBRE_2, TAMANHO_VERTICAL_MUNDO_SOBRE_2,
            -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void teclaPressionada(unsigned char key, int x, int y) {
    // vê qual tecla foi pressionada
    switch(key)
    {
    case 27:      // Tecla "ESC"
        exit(0);  // Sai da aplicação
        break;
    default:
        break;
    }
}


void mouseClicado(int button, int state, int x, int y) {
    // se botão esquerdo foi pressionado
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

        // converte o ponto (x,y) do sistema de coordenadas da janela para
        // o sistema de coordenadas do mundo
        ponto posicaoNoMundo = converteCoordenadaJanelaParaMundo(x, y);

        // percorre a lista de icones, verificando se algum deles foi atingido pelo clique
        struct no* noCorrente = lista->inicio;
        int acertouIcone = 0;
        printf("indo percorrer icones\n");
        noSelecionado = NULL;
        while(noCorrente != NULL && !acertouIcone) {
            struct retangulo retanguloIcone;
            retanguloIcone.posicao = ((struct icone*)noCorrente->objeto)->posicao;
            retanguloIcone.dimensoes = ((struct icone*)noCorrente->objeto)->tamanho;

            acertouIcone = colideComPonto(retanguloIcone, posicaoNoMundo);
            if (acertouIcone) {
                noSelecionado = noCorrente;
                ((struct icone*)noSelecionado->objeto)->situacao = SELECIONADO;
            }

            noCorrente = noCorrente->proximo;
        }
    }

    // se o botão esquerdo foi solto
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && noSelecionado != NULL) {
        ((struct icone*)noSelecionado->objeto)->situacao = ATOA;
        noSelecionado = NULL;
    }

    // se o botão direito foi solto
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {

        // cria um novo ícone
        struct icone *novo = (struct icone*)malloc(sizeof(struct icone));

        // converte o ponto (x,y) do sistema de coordenadas da janela para
        // o sistema de coordenadas do mundo
        ponto posicaoNoMundo = converteCoordenadaJanelaParaMundo(x, y);

        // configura o novo ícone
        novo->posicao.x = posicaoNoMundo.x;
        novo->posicao.y = posicaoNoMundo.y;
        novo->posicao.z = novo->tamanho.z = 0;
        novo->tamanho.x = novo->tamanho.y = TAMANHO_DO_ICONE;
        novo->cor = COR_PADRAO_DO_ICONE;

        insereNoFinal(lista, novo, sizeof(struct icone));
    }
}

void mouseMovimentado(int x, int y) {
    posicaoMouseNoMundo = converteCoordenadaJanelaParaMundo(x, y);
}

void atoa() {
    if (noSelecionado != NULL) {
        ((struct icone*)noSelecionado->objeto)->posicao = posicaoMouseNoMundo;
    }
    glutPostRedisplay();
}

// Função principal
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(400, 320);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Selecionando em 2D");

    // Registra callbacks para eventos
    glutDisplayFunc(desenhaMinhaCena);
    glutReshapeFunc(redimensionada);
    glutKeyboardFunc(teclaPressionada);
    glutMouseFunc(mouseClicado);
    glutPassiveMotionFunc(mouseMovimentado);
    glutMotionFunc(mouseMovimentado);
    glutIdleFunc(atoa);

    iniciaOpenGL();
    iniciaPrograma();

    glutMainLoop();
    return 0;
}
