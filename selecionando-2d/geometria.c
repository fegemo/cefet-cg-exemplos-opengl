#include "geometria.h"
#include "vetor.h"
#include "configuracoes.h"
#include <GL/freeglut.h>

// Converte um ponto ou vetor do sistema de coordenadas da janela para o
// sistema de coordenadas do mundo
// Faz uma regra de três para determinar a coordenada no sistema do mundo, usando:
// 1. Limites da janela
// 2. Limites do mundo
// 3. As coordenadas do ponto que queremos transformar do sistema da janela para o do mundo.
// A ideia é transformar o retângulo da janela no retângulo (ou caixa)
// do mundo definido via glOrtho.
ponto converteCoordenadaJanelaParaMundo(int x, int y) {
    ponto posicaoNoMundo;
    int larguraJanela = glutGet(GLUT_WINDOW_WIDTH);
    int alturaJanela = glutGet(GLUT_WINDOW_HEIGHT);
    float razaoAspecto = (float)larguraJanela/(float)alturaJanela;

    // Faz uma regra de tres para converter a coordenada (x,y) do sistema da janela para o do mundo
    posicaoNoMundo.x = (float)x/(float)larguraJanela * TAMANHO_HORIZONTAL_MUNDO*razaoAspecto - TAMANHO_HORIZONTAL_MUNDO_SOBRE_2*razaoAspecto;
    posicaoNoMundo.y = -((float)y/(float)alturaJanela * TAMANHO_VERTICAL_MUNDO - TAMANHO_VERTICAL_MUNDO_SOBRE_2);
    posicaoNoMundo.z = 0;

    return posicaoNoMundo;
}




// Converte um ponto ou vetor do sistema de coordenadas da janela para o
// sistema de coordenadas do mundo
// Usa o comando gluUnProject que, dada a projeção (glOrtho etc.) sendo usada
// e uma coordenada no sistema da janela, devolve a coordenada no mundo.
// Em outras palavras, gluUnProject faz uma operação inversa da projeção.
ponto converteCoordenadaJanelaParaMundo_fora(int x, int y) {
    double wx, wy, wz;
    double modelMatrix[16];
    double projMatrix[16];
    int viewport[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT,viewport);

    ponto posicaoNoMundo;
    gluUnProject(x, y, 0,
                 modelMatrix, projMatrix, viewport,
                 &wx, &wy, &wz
                 );

    posicaoNoMundo.x = wx;
    posicaoNoMundo.y = -wy;
    posicaoNoMundo.z = wz;

    return posicaoNoMundo;
}


int colideComRetangulo(struct retangulo objeto1, struct retangulo objeto2) {
    float esquerdaObjeto1 = objeto1.posicao.x - objeto1.dimensoes.x / 2.0f;
    float direitaObjeto1 = objeto1.posicao.x + objeto1.dimensoes.x / 2.0f;
    float baixoObjeto1 = objeto1.posicao.y - objeto1.dimensoes.y / 2.0f;
    float cimaObjeto1 = objeto1.posicao.y + objeto1.dimensoes.y / 2.0f;

    float esquerdaObjeto2 = objeto2.posicao.x - objeto2.dimensoes.x / 2.0f;
    float direitaObjeto2 = objeto2.posicao.x + objeto2.dimensoes.x / 2.0f;
    float baixoObjeto2 = objeto2.posicao.y - objeto2.dimensoes.y / 2.0f;
    float cimaObjeto2 = objeto2.posicao.y + objeto2.dimensoes.y / 2.0f;

    if (direitaObjeto1 <= esquerdaObjeto2 && esquerdaObjeto1 <= direitaObjeto2) {
        if (cimaObjeto1 <= baixoObjeto2 && baixoObjeto1 <= cimaObjeto2) {
            return 1;
        }
    }
    return 0;
}

int colideComPonto(struct retangulo objeto1, ponto objeto2) {
    float esquerdaObjeto1 = objeto1.posicao.x - objeto1.dimensoes.x / 2.0f;
    float direitaObjeto1 = objeto1.posicao.x + objeto1.dimensoes.x / 2.0f;
    float baixoObjeto1 = objeto1.posicao.y - objeto1.dimensoes.y / 2.0f;
    float cimaObjeto1 = objeto1.posicao.y + objeto1.dimensoes.y / 2.0f;

    if (esquerdaObjeto1 <= objeto2.x && direitaObjeto1 >= objeto2.x) {
        if (baixoObjeto1 <= objeto2.y && cimaObjeto1 >= objeto2.y) {
            return 1;
        }
    }
    return 0;
}
