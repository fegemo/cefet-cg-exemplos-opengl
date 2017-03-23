#include <stdio.h>
#include "vetor.h"

int main(int argc, char* argv[]) {
    struct vetor velocidade;
    velocidade.x = 1.0f;
    velocidade.y = 0.7f;
    printf("velocidade inicial: (%.2f, %.2f)\n", velocidade.x, velocidade.y);

    velocidade = multiEscalar(velocidade, 2);
    printf("velocidade * 2: (%.2f, %.2f)\n", velocidade.x, velocidade.y);
    
    return 0;
}
