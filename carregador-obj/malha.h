#ifndef _MALHA_
#define _MALHA_

#include "coords.h"
#include "lista.h"

struct vertice {
    ponto *coordenada;
    ponto *coordenadaTextura;
    vetor *normal;
};

struct face {
    // no: (struct vertice)
    listaEncadeada vertices;
};

struct malha {
    // no: (struct face)
    listaEncadeada faces;
    // no: (struct ponto)
    listaEncadeada coordenadas;
    // no: (struct ponto)
    listaEncadeada coordenadasTextura;
    // no: (struct vetor)
    listaEncadeada normais;
};

void iniciaMalha(struct malha*);
void adicionaCoordenadaMalha(struct malha*, ponto*);
void adicionaCoordenadaTexturaMalha(struct malha*, ponto*);
void adicionaNormalMalha(struct malha*, vetor*);
struct face *novaFaceMalha(struct malha *);
void adicionaVerticeFace(struct malha*, struct face*, int, int, int);
void imprimeMalha(struct malha*);

#endif
