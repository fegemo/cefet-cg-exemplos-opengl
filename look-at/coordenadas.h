#ifndef __COORDENADAS_H__
#define __COORDENADAS_H__

// Cria um novo nome ("escalar") para o tipo primitivo double
typedef float escalar;

// Estrutura que armazena 4 escalares
struct coordenadas {
  escalar x, y, z, w;
};

// Novo nome para a estrutura coordenadas: vetor
typedef struct coordenadas vetor;
// Novo nome para a estrutura coordenadas: ponto
typedef struct coordenadas ponto;

vetor multiplicaPorEscalar(vetor, escalar);
vetor somaVetorComVetor(vetor, vetor);
vetor diferencaVetorComVetor(vetor, vetor);
vetor diferencaEntrePontos(ponto, ponto);
ponto somaPontoComVetor(ponto, vetor);
escalar normaDoVetor(vetor);
vetor normalizado(vetor);
escalar distanciaEntrePontos(ponto, ponto);
escalar produtoEscalar(vetor, vetor);
vetor produtoVetorial(vetor, vetor);
escalar anguloEntreVetores(vetor, vetor);
ponto combinacaoAfim2Pontos(ponto, ponto, escalar);

void imprime(struct coordenadas, char*);

#endif
