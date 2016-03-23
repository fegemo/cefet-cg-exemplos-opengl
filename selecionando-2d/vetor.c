#include "vetor.h"
#include "math.h"

vetor multiplicaVetorPorEscalar(vetor a, float alpha) {
    vetor resultado = {
        a.x * alpha,
        a.y * alpha,
        a.z * alpha
    };
    return resultado;
}

vetor somaEntreVetores(vetor a, vetor b) {
    vetor resultado = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
    return resultado;
}

vetor diferencaEntreVetores(vetor a, vetor b) {
    return somaEntreVetores(a, multiplicaVetorPorEscalar(b, -1));
}

vetor diferencaEntrePontos(ponto p, ponto q) {
    return somaEntreVetores((vetor)p, (vetor)multiplicaVetorPorEscalar((vetor)q, -1));
}

float produtoInterno(vetor a, vetor b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float norma(vetor a) {
    return sqrt(produtoInterno(a, a));
}

float distanciaEntrePontos(ponto a, ponto b) {
    return norma(diferencaEntrePontos(a, b));
}
