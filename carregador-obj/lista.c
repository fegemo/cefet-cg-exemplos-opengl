#include <stdlib.h>
#include "lista.h"

void iniciaLista(listaEncadeada *l) {
    l->primeiro = (struct no*) malloc(sizeof(struct no));
    l->ultimo = l->primeiro;
    l->primeiro->proximo = l->ultimo;
    l->tamanho = 0;
}

void adicionaNoFinalLista(listaEncadeada *l, void *conteudo) {
    printf("\tl->ultimo: %p\n", l->ultimo);
    printf("\tl->ultimo->proximo: %p\n", l->ultimo->proximo);
    l->ultimo->proximo = (struct no*) malloc(sizeof(struct no));
    l->ultimo = l->ultimo->proximo;
    l->ultimo->conteudo = conteudo;
    l->ultimo->proximo = NULL;
    l->tamanho++;
}

void *recuperaEnesimoNo(listaEncadeada *l, int n) {
    struct no *atual = l->primeiro;
    while (n > 0 && atual != NULL) {
        atual = atual->proximo;
        n--;
    }

    return atual == NULL ? NULL : atual->conteudo;
}
