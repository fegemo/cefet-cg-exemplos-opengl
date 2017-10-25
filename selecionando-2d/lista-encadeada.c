#include "lista-encadeada.h"
#include <string.h>

struct no* criaNovoNo(void *novoObjeto, size_t tamanhoDoObjetoEmBytes) {
    // Aloca memória para o nó
    struct no *novoNo = (struct no*)malloc(sizeof(struct no));
    novoNo->objeto = malloc(tamanhoDoObjetoEmBytes);

    // Copia o conteúdo do novoObjeto para o espaço de memória que foi alocado
    memcpy(novoNo->objeto, novoObjeto, tamanhoDoObjetoEmBytes);

    return novoNo;
}

void insereNoInicio(struct lista *lista, void *novoObjeto, size_t tamanhoDoObjetoEmBytes) {
    struct no *novoNo = criaNovoNo(novoObjeto, tamanhoDoObjetoEmBytes);

    // Muda o início da lista para o novo nó
    novoNo->anterior = NULL;
    novoNo->proximo = lista->inicio;

    if (lista->inicio == NULL) {
        lista->inicio = lista->fim = novoNo;
    } else {
        lista->inicio->anterior = novoNo;
        lista->inicio = novoNo;
    }
}

void insereNoFinal(struct lista *lista, void *novoObjeto, size_t tamanhoDoObjetoEmBytes) {
    struct no *novoNo = criaNovoNo(novoObjeto, tamanhoDoObjetoEmBytes);

    novoNo->proximo = NULL;
    novoNo->anterior = lista->fim;

    if (lista->fim == NULL) {
        lista->inicio = lista->fim = novoNo;
    } else {
        lista->fim->proximo = novoNo;
        lista->fim = novoNo;
    }
}

void retiraNoComObjeto() {

}

void inicializaLista(struct lista *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
}
