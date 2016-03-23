#ifndef VETOR_H
#define VETOR_H

struct coordenadas {
    float x, y, z;
};

typedef struct coordenadas vetor;
typedef struct coordenadas ponto;
typedef struct coordenadas dimensoes;
typedef struct coordenadas cor;

vetor multiplicaVetorPorEscalar(vetor a, float alpha);
vetor somaEntreVetores(vetor a, vetor b);
vetor diferencaEntreVetores(vetor a, vetor b);
vetor diferencaEntrePontos(ponto p, ponto q);
float produtoInterno(vetor a, vetor b);
float norma(vetor a);
float distanciaEntrePontos(ponto a, ponto b);

#endif // VETOR_H
