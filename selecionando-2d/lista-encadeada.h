#include <stdlib.h>

struct no {
    // "Ponteiro para void" pode armazenar qualquer tipo de dados
    // É um hack que nos possibilita fazer uma lista encadeada genérica
    void *objeto;

    // Cada nó tem um ponteiro para o anterior e para o próximo: lista duplamente encadeada
    struct no *proximo;
    struct no *anterior;
};

struct lista {
    struct no *inicio;
    struct no *fim;
};

void insereNoInicio(struct lista *lista, void *novoObjeto, size_t tamanhoDoObjetoEmBytes);
void insereNoFinal(struct lista *lista, void *novoObjeto, size_t tamanhoDoObjetoEmBytes);
void inicializaLista(struct lista *lista);

