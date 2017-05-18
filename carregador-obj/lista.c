#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

void iniciaLista(listaEncadeada *l) {
    l->primeiro = (struct no*) malloc(sizeof(struct no));
    l->ultimo = l->primeiro;
//    l->ultimo->proximo = NULL;
    l->primeiro->proximo = NULL;
    l->tamanho = 0;
}

void adicionaNoFinalLista(listaEncadeada *l, void *conteudo) {
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
    if (n > 0) {
        printf("Erro: a lista estava menor que n = %d itens.\n", n);
    }

    return atual == NULL ? NULL : atual->conteudo;
}
