#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "coords.h"
#include "malha.h"
#include "modelo.h"


void preparaListaVisualizacao(struct modelo *m) {
    printf("preparando lista de visualizacao: %d\n", m->listaVisualizacao);
    glNewList(m->listaVisualizacao, GL_COMPILE);

    // para cada face da malha...
    struct no *iteradorFace = m->malha.faces.primeiro->proximo;
    int faces = 0;
    while (iteradorFace != NULL) {
        printf("face %d: ", ++faces);
        glBegin(GL_POLYGON);
            struct face *faceAtual = (struct face *)iteradorFace->conteudo;

            // para cada vértice da face atual...
            struct no *iteradorCoordenada = faceAtual->coordenadas.primeiro->proximo;
            struct no *iteradorCoordenadaTextura = faceAtual->coordenadasTextura.primeiro->proximo;
            struct no *iteradorNormal = faceAtual->normais.primeiro->proximo;
            int coord = 0;
            while (iteradorCoordenada != NULL) {
                ponto *coordenadaAtual = (ponto *)iteradorCoordenada->conteudo;
                ponto *coordenadaTexturaAtual = (ponto *)iteradorCoordenada->conteudo;
                vetor *normalAtual = (vetor *)iteradorCoordenada->conteudo;

                glTexCoord3fv(coordenadaTexturaAtual->v);
                glNormal3fv(normalAtual->v);
                glVertex3fv(coordenadaAtual->v);

                iteradorCoordenada = iteradorCoordenada->proximo;
                iteradorCoordenadaTextura = iteradorCoordenadaTextura->proximo;
                iteradorNormal = iteradorNormal->proximo;
                coord++;
            }
            printf("%d vertices", coord);
        glEnd();
        printf("\n");
        iteradorFace = iteradorFace->proximo;
    }

    glEndList();
}


char *paxtok (char *str, char *seps) {
    static char *tpos, *tkn, *pos = NULL;
    static char savech;

    // Specific actions for first and subsequent calls.

    if (str != NULL) {
        // First call, set pointer.

        pos = str;
        savech = 'x';
    } else {
        // Subsequent calls, check we've done first.

        if (pos == NULL)
            return NULL;

        // Then put character back and advance.

        while (*pos != '\0')
            pos++;
        *pos++ = savech;
    }

    // Detect previous end of string.

    if (savech == '\0')
        return NULL;

    // Now we have pos pointing to first character.
    // Find first separator or nul.

    tpos = pos;
    while (*tpos != '\0') {
        tkn = strchr (seps, *tpos);
        if (tkn != NULL)
            break;
        tpos++;
    }

    savech = *tpos;
    *tpos = '\0';

    return pos;
}

void interpretaLinha(const char *linha, struct modelo *m) {

    char *tokenizedLinha = (char*)malloc(1 + strlen(linha));
    strcpy(tokenizedLinha, linha);
    char *tipoLinha;
    tipoLinha = strtok(tokenizedLinha, " ");

    if (strcmp(tipoLinha, "v") == 0) {
//        printf("carregando coordenada\n");
        char *escalar;
        int i = 0;
        ponto *coordenada = (ponto *)malloc(sizeof(ponto));
        *coordenada = (ponto){ { 0, 0, 0 } };
        while ((escalar = strtok(NULL, " ")) != NULL && i < 4) {
            sscanf(escalar, "%f", &(coordenada->v[i]));
            i++;
        }
        adicionaCoordenadaMalha(&(m->malha), coordenada);
        m->qtdeCoordenadas++;
    } else if (strcmp(tokenizedLinha, "vt") == 0) {
//        printf("carregando textura\n");
        // formatos válidos:
        // ----------------
        // vt 0.1213 0.1254
        char *escalar;
        int i = 0;
        ponto *coordenadaTextura = (ponto *)malloc(sizeof(ponto));
        *coordenadaTextura = (ponto){ { 0, 0, 0 } };
        while ((escalar = strtok(NULL, " ")) != NULL && i < 2) {
            sscanf(escalar, "%f", &(coordenadaTextura->v[i]));
            i++;
        }
        adicionaCoordenadaTexturaMalha(&(m->malha), coordenadaTextura);
        m->qtdeCoordenadasTextura++;
    } else if (strcmp(tokenizedLinha, "vn") == 0) {
//        printf("carregando normal\n");
        // formatos válidos:
        // ----------------
        // vn 0.1213 0.1254 -0.4543
        // vn 0.1213 0.1254 -0.4543 0
        char *escalar;
        int i = 0;
        vetor *normal = (vetor *)malloc(sizeof(vetor));
        *normal = (vetor){ { 0, 0, 0 } };
        while ((escalar = strtok(NULL, " ")) != NULL && i < 4) {
            sscanf(escalar, "%f", &(normal->v[i]));
            i++;
        }
        adicionaNormalMalha(&(m->malha), normal);
        m->qtdeNormais++;
    } else if (strcmp(tokenizedLinha, "f") == 0) {
        printf("carregando face\n");
        // formatos válidos:
        // ----------------
        // f 32/23/10 23/1/2 3/1/5 4/5/7 ...
        // f 1 3 5 ...
        // f 1//1 2//1 3//1 ...
        // f 1/1 2/1 3/1 ...
        struct face *novaFace = novaFaceMalha(&(m->malha));
        m->qtdeFaces++;

        int indiceCoordenada = -1,
            indiceCoordenadaTextura = -1,
            indiceNormal = -1;
        int coord = 0;
        char *triplice;
        while ((tokenizedLinha = strtok(NULL, " ")) != NULL) {
            coord++;
            triplice = (char *) malloc(1 + strlen(tokenizedLinha));
            strcpy(triplice, tokenizedLinha);
            paxtok(triplice, "/");
            sscanf(triplice, "%d", &indiceCoordenada);
            if ((triplice = paxtok(NULL, "/")) != NULL) {
                if (strcmp(triplice, "") != 0) {
                    sscanf(triplice, "%d", &indiceCoordenadaTextura);
                }
            }
            if ((triplice = paxtok(NULL, "/")) != NULL) {
                sscanf(triplice, "%d", &indiceNormal);
            }
            adicionaVerticeFace(&(m->malha), novaFace,
                indiceCoordenada, indiceCoordenadaTextura, indiceNormal);
        };

        printf("face: %d vertices\n", coord);
    }
//    printf("\tVertices: %d\n\tCoordenadas de Textura: %d\n\tNormais: %d\n\tFaces: %d\n\n",
//        m->qtdeCoordenadas, m->qtdeCoordenadasTextura, m->qtdeNormais, m->qtdeFaces
//    );
}

struct modelo *carregaModelo(char *nome) {
    struct modelo *m = (struct modelo*) malloc(sizeof(struct modelo));
    iniciaMalha(&(m->malha));
    m->qtdeCoordenadas = 0;
    m->qtdeCoordenadasTextura = 0;
    m->qtdeNormais = 0;
    m->qtdeFaces = 0;

    FILE *arquivo;
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


    printf("Modelo '%s' carregado com sucesso. Info:\n", nome);
    printf("\tVertices: %d\n\tCoordenadas de Textura: %d\n\tNormais: %d\n\tFaces: %d\n\n",
        m->qtdeCoordenadas, m->qtdeCoordenadasTextura, m->qtdeNormais, m->qtdeFaces
    );

    m->listaVisualizacao = glGenLists(1);
    printf("Criando lista de visualizacao: %d\n", m->listaVisualizacao);
    preparaListaVisualizacao(m);
    printf(" Lista ok\n");

    return m;
}

void desenhaModelo(struct modelo *m) {
    printf("desenhando lista de visualizacao: %d\n", m->listaVisualizacao);
    glCallList(m->listaVisualizacao);
}
