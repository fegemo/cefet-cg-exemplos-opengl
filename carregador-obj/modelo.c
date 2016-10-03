#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "coords.h"
#include "malha.h"
#include "modelo.h"

void interpretaLinha(char *linha, struct modelo *m) {

    char *tipoLinha;
    tipoLinha = strtok(linha, " ");

    if (strcmp(tipoLinha, "v") == 0) {
        char *escalar;
        int i = 0;
        ponto *coordenada = (ponto *)malloc(sizeof(ponto));
        *coordenada = (ponto){ { 0, 0, 0, 1 } };
        while ((escalar = strtok(linha, " ")) != NULL && i < 4) {
            sscanf(escalar, "%f", &(coordenada->v[i]));
            i++;
        }
        adicionaCoordenadaMalha(&(m->malha), coordenada);
    } else if (strcmp(linha, "vt") == 0) {
        // formatos válidos:
        // ----------------
        // vt 0.1213 0.1254
        char *escalar;
        int i = 0;
        ponto *coordenadaTextura = (ponto *)malloc(sizeof(ponto));
        *coordenadaTextura = (ponto){ { 0, 0, 0, 1 } };
        while ((escalar = strtok(linha, " ")) != NULL && i < 2) {
            sscanf(escalar, "%f", &(coordenadaTextura->v[i]));
            i++;
        }
        adicionaCoordenadaTexturaMalha(&(m->malha), coordenadaTextura);
    } else if (strcmp(linha, "vn") == 0) {
        // formatos válidos:
        // ----------------
        // vn 0.1213 0.1254 -0.4543
        // vn 0.1213 0.1254 -0.4543 0
        char *escalar;
        int i = 0;
        vetor *normal = (vetor *)malloc(sizeof(vetor));
        *normal = (vetor){ { 0, 0, 0, 0 } };
        while ((escalar = strtok(linha, " ")) != NULL && i < 4) {
            sscanf(escalar, "%f", &(normal->v[i]));
            i++;
        }
        adicionaNormalMalha(&(m->malha), normal);
    } else if (strcmp(linha, "f") == 0) {
        // formatos válidos:
        // ----------------
        // f 32/23/10 23/1/2 3/1/5 4/5/7 ...
        // f 1 3 5 ...
        // f 1//1 2//1 3//1 ...
        // f 1/1 2/1 3/1 ...
        struct face *novaFace = novaFaceMalha(&(m->malha));
        char *triplice;

        int indiceCoordenada = -1,
            indiceCoordenadaTextura = -1,
            indiceNormal = -1;
        char *valor;
        while ((triplice = strtok(linha, " ")) != NULL) {
            valor = strtok(triplice, "/");
            sscanf(valor, "%d", &indiceCoordenada);
            if ((valor = strtok(triplice, "/")) != NULL) {
                if (strcmp(valor, "") != -1) {
                    sscanf(valor, "%d", &indiceCoordenadaTextura);
                }
            }
            if ((valor = strtok(triplice, "/")) != NULL) {
                sscanf(valor, "%d", &indiceNormal);
            }
        }

        adicionaVerticeFace(&(m->malha), novaFace,
            indiceCoordenada, indiceCoordenadaTextura, indiceNormal);
    }
}

struct modelo *carregaModelo(char *nome) {
    struct modelo *m = (struct modelo*) malloc(sizeof(struct modelo));

    FILE *arquivo;
    m->listaVisualizacao = glGenLists(1);
    arquivo = fopen(nome,"r");
    if (!arquivo) {
        printf("Nao foi possivel abrir o arquivo %s\n", nome);
        return NULL;
    }

    char *linha = NULL;
    size_t tamanhoLinha = 0;
    int caracteresLidos;

    while ((caracteresLidos = getline(&linha, &tamanhoLinha, arquivo)) != -1) {
        interpretaLinha(linha, m);
    }
    fclose(arquivo);

    return m;
}

void preparaListaVisualizacao(struct modelo *m) {
    glNewList(m->listaVisualizacao, GL_COMPILE);

    // para cada face da malha...
    struct no *iteradorFace = m->malha.faces.primeiro->proximo;
    while (iteradorFace != NULL) {
        glBegin(GL_POLYGON);
            struct face *faceAtual = (struct face *)iteradorFace->conteudo;

            // para cada vértice da face atual...
            struct no *iteradorCoordenada = faceAtual->coordenadas.primeiro->proximo;
            // struct ponto *iteradorCoordenadaTextura = faceAtual->coordenadasTextura.primeiro->proximo;
            struct no *iteradorNormal = faceAtual->normais.primeiro->proximo;
            while (iteradorCoordenada != NULL) {
                ponto *coordenadaAtual = (ponto *)iteradorCoordenada->conteudo;
                // struct ponto *coordenadaTexturaAtual = (struct ponto *)iteradorCoordenada->conteudo;
                vetor *normalAtual = (ponto *)iteradorCoordenada->conteudo;

                // glTexCoord2f(coordenadaTexturaAtual->x, coordenadaTexturaAtual->y);
                glNormal3fv(normalAtual->v);
                glVertex3fv(coordenadaAtual->v);

                iteradorCoordenada = iteradorCoordenada->proximo;
                // iteradorCoordenadaTextura = iteradorCoordenadaTextura->proximo;
                iteradorNormal = iteradorNormal->proximo;
            }
        glEnd();
        iteradorFace = iteradorFace->proximo;
    }

    glEndList();
}

void desenhaModelo(struct modelo *m) {
    glCallList(m->listaVisualizacao);
}
