#ifndef _MALHA_
#define _MALHA_

#include "coords.h"
#include "lista.h"

struct face {
    // ponteiros para (ponto)
    listaEncadeada coordenadas;
    // ponteiros para (ponto)
    listaEncadeada coordenadasTextura;
    // ponteiros para (vetor)
    listaEncadeada normais;
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

#endif
