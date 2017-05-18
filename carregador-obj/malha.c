#include <stdlib.h>
#include <stdio.h>
#include "malha.h"

void iniciaMalha(struct malha *m) {
    iniciaLista(&m->faces);
    iniciaLista(&m->coordenadas);
    iniciaLista(&m->coordenadasTextura);
    iniciaLista(&m->normais);
}

void adicionaCoordenadaMalha(struct malha *m, ponto *coordenada) {
    adicionaNoFinalLista(&(m->coordenadas), coordenada);
}

void adicionaCoordenadaTexturaMalha(struct malha *m, ponto *coordenadaTextura) {
    adicionaNoFinalLista(&m->coordenadasTextura, coordenadaTextura);
}

void adicionaNormalMalha(struct malha *m, vetor *normal) {
    adicionaNoFinalLista(&m->normais, normal);
}

struct face *novaFaceMalha(struct malha *m) {
    struct face *novaFace = (struct face*) malloc(sizeof(struct face));
    iniciaLista(&novaFace->vertices);
    adicionaNoFinalLista(&m->faces, novaFace);
    return novaFace;
}

void adicionaVerticeFace(struct malha *m, struct face *f, int indiceCoordenada,
    int indiceCoordenadaTextura, int indiceNormal) {

    struct vertice *vertice = (struct vertice*) malloc(sizeof(struct vertice));

    vertice->coordenada = (ponto*)recuperaEnesimoNo(&m->coordenadas, indiceCoordenada);

    vertice->coordenadaTextura = NULL;
    if (indiceCoordenadaTextura > 0) {
        vertice->coordenadaTextura = (ponto*) recuperaEnesimoNo(&m->coordenadasTextura,
            indiceCoordenadaTextura);
    }

    vertice->normal = NULL;
    if (indiceNormal > 0) {
        vertice->normal = (vetor*) recuperaEnesimoNo(&m->normais, indiceNormal);
    }

    adicionaNoFinalLista(&f->vertices, vertice);
}


void imprimeMalha(struct malha *malha) {
    printf("Imprimindo malha:\n");
    printf("\tVertices: %d\n", malha->coordenadas.tamanho);
    printf("\tCoordenadas de textura: %d\n", malha->coordenadasTextura.tamanho);
    printf("\tVetores normais: %d\n", malha->normais.tamanho);
    printf("\tFaces: %d\n", malha->faces.tamanho);

    struct no *iteradorFace = malha->faces.primeiro->proximo;
    int numFaces = 1;
    while (iteradorFace != NULL) {
        struct face *faceAtual = (struct face *)iteradorFace->conteudo;

        printf("\t\tFace #%d:\n", numFaces);
        struct no *iteradorVertice= faceAtual->vertices.primeiro->proximo;
        while (iteradorVertice != NULL) {
            struct vertice *verticeAtual = (struct vertice *)iteradorVertice->conteudo;
            printf("\t\t\tv: ");
            imprimeCoords(verticeAtual->coordenada);
            printf(", vt: ");
            if (verticeAtual->coordenadaTextura != NULL) {
                imprimeCoords(verticeAtual->coordenadaTextura);
            } else {
                printf("ausente");
            }
            printf(", vn: ");
            if (verticeAtual->normal != NULL) {
                imprimeCoords(verticeAtual->normal);
            } else {
                printf("ausente");
            }
            printf("\n");

            iteradorVertice = iteradorVertice->proximo;
        }
        iteradorFace = iteradorFace->proximo;
        numFaces++;
    }
}
