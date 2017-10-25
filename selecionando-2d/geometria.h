#include "vetor.h"

struct retangulo {
    ponto posicao;
    dimensoes dimensoes;
};

// Converte um ponto ou vetor do sistema de coordenadas da janela para o
// sistema de coordenadas do mundo, dadas coordenadas (x,y) na janela
ponto converteCoordenadaJanelaParaMundo(int x, int y);

// Verifica se um retângulo intersecta outro.
// Retorna 1 se verdadeiro ou 0 se falso.
int colideComRetangulo(struct retangulo objeto1, struct retangulo objeto2);
int colideComPonto(struct retangulo objeto1, ponto objeto2);
