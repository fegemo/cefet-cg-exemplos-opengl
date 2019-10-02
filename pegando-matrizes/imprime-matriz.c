#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <stdlib.h>

#define TAMANHO_MATRIZ 4

void imprimeMatriz(float *m)
{
    int i, j;
    for (i = 0; i < TAMANHO_MATRIZ; i++) {
        for (j = 0; j < TAMANHO_MATRIZ; j++) {
            printf("% .2f  ", m[i * TAMANHO_MATRIZ + j]);
        }
        printf("\n");
    }
}

void trocaValoresEmVetor(float *m, int idx1, int idx2)
{
    float originalIdx1 = m[idx1];
    m[idx1] = m[idx2];
    m[idx2] = originalIdx1;
}

void transposeMatrix(float m[16])
{
    int i, j;

    for (i = 0; i < TAMANHO_MATRIZ; i++) {
        for (j = i + 1; j < TAMANHO_MATRIZ; j++) {
            int from = i * TAMANHO_MATRIZ + j;
            int to = j * TAMANHO_MATRIZ + i;
            trocaValoresEmVetor(m, from, to);
        }
    }
}

void imprimeMatrizOpenGL(unsigned int pilhaDeMatrizes)
{
    // cria um vetor de 16 posições
    float m[TAMANHO_MATRIZ * TAMANHO_MATRIZ];

    // chama a glGet (para vetor de floats) para pegar a matriz que está
    // no topo da pilha especificada pelo parâmetro matrixStack
    glGetFloatv(pilhaDeMatrizes, m);

    // transpõe a matriz, porque o OpenGL é colum-major, ou seja, os 16 floats
    // que representam uma matriz vão preenchendo as colunas primeiro, e não
    // as linhas
    transposeMatrix(m);

    // chama uma função que desenha 16 floats como uma matriz "row-major"
    imprimeMatriz(m);
}


float* pegaMatrizOpenGL(unsigned int pilhaDeMatrizes)
{
    // cria um vetor de 16 posições com alocação dinâmica (precisa retornar)
    float* m = (float*) malloc(
            TAMANHO_MATRIZ * TAMANHO_MATRIZ * sizeof(float)
        );

    // chama a glGet (para vetor de floats) para pegar a matriz que está
    // no topo da pilha especificada pelo parâmetro matrixStack
    glGetFloatv(pilhaDeMatrizes, m);

    // transpõe a matriz, porque o OpenGL é colum-major, ou seja, os 16 floats
    // que representam uma matriz vão preenchendo as colunas primeiro, e não
    // as linhas
    transposeMatrix(m);

    // chama uma função que desenha 16 floats como uma matriz "row-major"
    return m;
}
