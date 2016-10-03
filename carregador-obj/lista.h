#ifndef _LISTA_
#define _LISTA_

struct no {
    void *conteudo;
    struct no *proximo;
};

struct lista {
    struct no *primeiro, *ultimo;
    int tamanho;
};

typedef struct lista listaEncadeada;

void iniciaLista(listaEncadeada *l);
void adicionaNoFinalLista(listaEncadeada *l, void *conteudo);
void *recuperaEnesimoNo(listaEncadeada *l, int n);

#endif
