#include <stdio.h>
#include <GL/gl.h>

#define DIMENSION 4

void printMatrix(float *m) {
    int i, j;
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            printf("% .2f  ", m[i * DIMENSION + j]);
        }
        printf("\n");
    }
}

void swapArrayValue(float *m, int idx1, int idx2) {
    float originalIdx1 = m[idx1];
    m[idx1] = m[idx2];
    m[idx2] = originalIdx1;
}

void transposeMatrix(float m[16]) {
    int i, j;

    for (i = 0; i < DIMENSION; i++) {
        for (j = i + 1; j < DIMENSION; j++) {
            int from = i * DIMENSION + j;
            int to = j * DIMENSION + i;
            swapArrayValue(m, from, to);
        }
    }
}

void printOpenGLMatrix(unsigned int matrixStack) {
    // cria um vetor de 16 posições
    float m[DIMENSION * DIMENSION];

    // chama a glGet (para vetor de floats) para pegar a matriz que está
    // no topo da pilha especificada pelo parâmetro matrixStack
    glGetFloatv(matrixStack, m);

    // transpõe a matriz, porque o OpenGL é colum-major, ou seja, os 16 floats
    // que representam uma matriz vão preenchendo as colunas primeiro, e não
    // as linhas
    transposeMatrix(m);

    // chama uma função que desenha 16 floats como uma matriz "row-major"
    printMatrix(m);
}
