#include "vetor.h"

enum situacaoIcone {
    ATOA,
    SELECIONADO
};

struct icone {
    ponto posicao;
    dimensoes tamanho;
    cor cor;
    enum situacaoIcone situacao;
};

void desenhaIcone(struct icone icone);
