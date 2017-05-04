#include <stdlib.h>
#include "malha.h"

void iniciaMalha(struct malha *m) {
    printf("\tindo iniciar as listas da malha\n");
    iniciaLista(&m->faces);
    iniciaLista(&m->coordenadas);
    iniciaLista(&m->coordenadasTextura);
    iniciaLista(&m->normais);
}

void adicionaCoordenadaMalha(struct malha *m, ponto *coordenada) {
    printf("\tadicionando um vertice a malha\n");
    adicionaNoFinalLista(&(m->coordenadas), &coordenada);
    printf("\tadicionou um vertice a malha\n");
}

void adicionaCoordenadaTexturaMalha(struct malha *m, ponto *coordenadaTextura) {
    adicionaNoFinalLista(&m->coordenadasTextura, coordenadaTextura);
}

void adicionaNormalMalha(struct malha *m, vetor *normal) {
    adicionaNoFinalLista(&m->normais, normal);
}

struct face *novaFaceMalha(struct malha *m) {
    struct face *novaFace = (struct face*) malloc(sizeof(struct face));
    iniciaLista(&novaFace->coordenadas);
    iniciaLista(&novaFace->coordenadasTextura);
    iniciaLista(&novaFace->normais);
    adicionaNoFinalLista(&m->faces, novaFace);
    return novaFace;
}

void adicionaVerticeFace(struct malha *m, struct face *f, int indiceCoordenada,
    int indiceCoordenadaTextura, int indiceNormal) {

    ponto *coordenada = recuperaEnesimoNo(&m->coordenadas, indiceCoordenada);
    adicionaNoFinalLista(&f->coordenadas, coordenada);

    if (indiceCoordenadaTextura > 0) {
        ponto *coordenadaTextura = recuperaEnesimoNo(&m->coordenadasTextura,
            indiceCoordenadaTextura);
        if (coordenadaTextura != NULL) {
            adicionaNoFinalLista(&f->coordenadasTextura, coordenadaTextura);
        }
    }

    if (indiceNormal > 0) {
        vetor *normal = recuperaEnesimoNo(&m->normais, indiceNormal);
        if (normal != NULL) {
            adicionaNoFinalLista(&f->normais, normal);
        }
    }
}
