#ifndef _MODELO_OBJ_
#define _MODELO_OBJ_

#include "malha.h"
#include "material.h"

struct grupo {
    char* nome;
    struct material* material;
    listaEncadeada faces;
};

struct modelo {
    struct malha malha;
    listaEncadeada materiais;
    listaEncadeada grupos;

    int listaVisualizacao;
};

struct modelo *carregaModelo(char *);
void desenhaModelo(struct modelo*);

#endif
