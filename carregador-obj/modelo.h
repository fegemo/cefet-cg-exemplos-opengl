#ifndef _MODELO_OBJ_
#define _MODELO_OBJ_

#include "malha.h"

struct modelo {
    struct malha malha;
    int listaVisualizacao;
};

struct modelo *carregaModelo(char *);
void desenhaModelo(struct modelo*);

#endif
