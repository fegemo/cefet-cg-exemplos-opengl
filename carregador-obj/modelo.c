#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "coords.h"
#include "malha.h"
#include "modelo.h"
#include "toksplit.h"

#define MAX_TOKEN_SIZE 500

void preparaListaVisualizacao(struct modelo *m);
void carregaMateriais(struct modelo*, char*);


void iniciaModelo(struct modelo* modelo) {
    iniciaMalha(&modelo->malha);
    iniciaLista(&modelo->materiais);
    iniciaLista(&modelo->grupos);

    // coloca um material "default"
    struct material *material = (struct material *)malloc(sizeof(struct material));
    strcpy(material->nome, "default");
    material->ambiente  = (cor) { { 0.2, 0.2, 0.2, 1 } };
    material->difusa    = (cor) { { 0.8, 0.8, 0.8, 1 } };
    material->especular = (cor) { { 0.0, 0.0, 0.0, 1 } };
    material->emissiva  = (cor) { { 0.0, 0.0, 0.0, 1 } };
    material->expoenteEspecularidade = 65.0;
    material->opacidade = 1.0;
    adicionaNoFinalLista(&modelo->materiais, material);

    // coloca um grupo "default"
    struct grupo *grupo = (struct grupo*) malloc(sizeof(struct grupo));
    grupo->nome = strdup("default");
    grupo->material = material;
    iniciaLista(&grupo->faces);
    adicionaNoFinalLista(&modelo->grupos, grupo);
}

// Lê e interpreta uma linha do arquivo .obj
void interpretaLinha(char *linha, struct modelo *m) {

    // divide a linha em um array com o espaço (" ") como separador
    char tipoLinha[MAX_TOKEN_SIZE + 1];
    char *restoLinha = toksplit(linha, ' ', tipoLinha, MAX_TOKEN_SIZE);

    // uma linha tipo "v" descreve as coords de um vértice
    if (strcmp(tipoLinha, "v") == 0) {
        char escalar[MAX_TOKEN_SIZE + 1];
        int i = 0;
        ponto *coordenada = (ponto *)malloc(sizeof(ponto));
        *coordenada = (ponto){ { 0, 0, 0, 1 } };
        while (restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE), escalar[0] != '\0' && i < 4) {
            sscanf(escalar, "%f", &(coordenada->v[i]));
            i++;
        }
        adicionaCoordenadaMalha(&(m->malha), coordenada);
    }

    // uma linha tipo "vt" tem as coordenadas de textura de um vértice
    else if (strcmp(tipoLinha, "vt") == 0) {
        // formatos válidos:#
        // ----------------
        // vt 0.1213 0.1254
        char escalar[MAX_TOKEN_SIZE + 1];
        int i = 0;
        ponto *coordenadaTextura = (ponto *)malloc(sizeof(ponto));
        *coordenadaTextura = (ponto){ { 0, 0, 0, 1 } };
        while (restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE), escalar[0] != '\0' && i < 2) {
            sscanf(escalar, "%f", &(coordenadaTextura->v[i]));
            i++;
        }
        adicionaCoordenadaTexturaMalha(&(m->malha), coordenadaTextura);
    }

    // uma linha tipo "vn" possui as coords de um vetor normal
    else if (strcmp(tipoLinha, "vn") == 0) {
        // formatos válidos:
        // ----------------
        // vn 0.1213 0.1254 -0.4543
        // vn 0.1213 0.1254 -0.4543 0
        char escalar[MAX_TOKEN_SIZE + 1];
        int i = 0;
        vetor *normal = (vetor *)malloc(sizeof(vetor));
        *normal = (vetor){ { 0, 0, 0, 0 } };
        while (restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE), escalar[0] != '\0' && i < 4) {
            sscanf(escalar, "%f", &(normal->v[i]));
            i++;
        }
        adicionaNormalMalha(&(m->malha), normal);
    }

    // uma linha do tipo "f" descreve uma face
    // a face é composta por 3+ vértices e outras propriedades opcionais,
    // como uma coordenada de textura e um vetor normal
    else if (strcmp(tipoLinha, "f") == 0) {
        // formatos válidos:
        // ----------------
        // f 32/23/10 23/1/2 3/1/5 4/5/7 ...    (vértice/coord. tex./normal)
        // f 1 3 5 ...                          (vértice)
        // f 1//1 2//1 3//1 ...                 (vértice//normal)
        // f 1/1 2/1 3/1 ...                    (vértice/coord. tex.)
        struct face *novaFace = novaFaceMalha(&(m->malha));
        char *triplice = (char*)malloc(sizeof(char)*(MAX_TOKEN_SIZE + 1));

        int indiceCoordenada = -1,
            indiceCoordenadaTextura = -1,
            indiceNormal = -1;

        while (*(restoLinha = toksplit(restoLinha, ' ', triplice, MAX_TOKEN_SIZE)), triplice[0] != '\0') {
            const char *restoTriplice = triplice;
            char escalar[MAX_TOKEN_SIZE + 1];
            restoTriplice = toksplit(restoTriplice, '/', escalar, MAX_TOKEN_SIZE);
            sscanf(escalar, "%d", &indiceCoordenada);
            if ((restoTriplice = toksplit(restoTriplice, '/', escalar, MAX_TOKEN_SIZE))) {
                if (escalar[0] != '\0') {
                    sscanf(escalar, "%d", &indiceCoordenadaTextura);
                }
            }
            if ((restoTriplice = toksplit(restoTriplice, '/', escalar, MAX_TOKEN_SIZE))) {
                if (escalar[0] != '\0') {
                    sscanf(escalar, "%d", &indiceNormal);
                }
            }

            adicionaVerticeFace(&(m->malha), novaFace,
                indiceCoordenada, indiceCoordenadaTextura, indiceNormal);
        }

        // adiciona a face ao grupo atual
        struct grupo *grupoCorrente = (struct grupo*) m->grupos.ultimo->conteudo;
        adicionaNoFinalLista(&grupoCorrente->faces, novaFace);
    }

    // 'mtllib' faz carregar um arquivo .mtl (de descrição de material)
    else if (strcmp(tipoLinha, "mtllib") == 0) {
        // tira o '/n' do final
        restoLinha[strlen(restoLinha) - 1] = '\0';
        // carrega o arquivo
        carregaMateriais(m, strdup(restoLinha));
    }

    else if (strcmp(tipoLinha, "usemtl") == 0) {
        // verifica se temos um material com esse nome
        struct no *iteradorMaterial = m->materiais.primeiro->proximo;
        struct material *material = (struct material*)iteradorMaterial->conteudo;
        restoLinha[strlen(restoLinha) - 1] = '\0';
        while (iteradorMaterial != NULL) {
            struct material *materialAtual = (struct material *)iteradorMaterial->conteudo;
            if (strcmp(materialAtual->nome, restoLinha) == 0) {
                material = materialAtual;
                break;
            }
            iteradorMaterial = iteradorMaterial->proximo;
        }

        // cria novo grupo para que ele tenha o material que vamos começar a usar
        struct grupo *grupo = (struct grupo*) malloc(sizeof(struct grupo));
        grupo->nome = strdup(restoLinha);
        grupo->material = material;
        iniciaLista(&grupo->faces);

        adicionaNoFinalLista(&m->grupos, grupo);
    }
}

struct modelo *carregaModelo(char *nome) {
    struct modelo *modelo = (struct modelo*) malloc(sizeof(struct modelo));

    iniciaModelo(modelo);

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
        interpretaLinha(linha, modelo);
    }
    fclose(arquivo);

    imprimeMalha(&modelo->malha);
    preparaListaVisualizacao(modelo);

    return modelo;
}

void interpretaLinhaMtl(char* linha, struct modelo* modelo) {
    // divide a linha em um array com o espaço (" ") como separador
    char tipoLinha[MAX_TOKEN_SIZE + 1];
    char *restoLinha = toksplit(linha, ' ', tipoLinha, MAX_TOKEN_SIZE);
    struct material *materialCorrente = (struct material *) modelo->materiais.ultimo->conteudo;

    // uma linha tipo "newmtl" descreve um novo material
    if (strcmp(tipoLinha, "newmtl") == 0) {
        // instancia um material novinho
        struct material *material = (struct material*) malloc(sizeof(material));
        // tira o '/n' do nome
        restoLinha[strlen(restoLinha) - 1] = '\0';
        // define o nome
//        material->nome = (char*)malloc(sizeof(char) * (strlen(restoLinha) + 1));
        strcpy(material->nome, restoLinha);
//        material->nome = strdup(restoLinha);
        adicionaNoFinalLista(&modelo->materiais, material);
    }

    // uma linha começando com 'K' é um coeficiente de Phong
    else if (tipoLinha[0] == 'K') {
        char escalar[MAX_TOKEN_SIZE + 1];
        int i;

        switch (tipoLinha[1]) {
        case 'a':   // ambiente
            for (i = 0; i < 3; i++) {
                restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE);
                sscanf(escalar, "%f", &(materialCorrente->ambiente.v[i]));
            }
            materialCorrente->ambiente.v[3] = 1;
            break;

        case 'd':   // difusa
            for (i = 0; i < 3; i++) {
                restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE);
                sscanf(escalar, "%f", &(materialCorrente->difusa.v[i]));
            }
            materialCorrente->difusa.v[3] = 1;
            break;

        case 's':   // especular
            for (i = 0; i < 3; i++) {
                restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE);
                sscanf(escalar, "%f", &(materialCorrente->especular.v[i]));
            }
            materialCorrente->especular.v[3] = 1;
            break;

        case 'e':   // emissiva
            for (i = 0; i < 3; i++) {
                restoLinha = toksplit(restoLinha, ' ', escalar, MAX_TOKEN_SIZE);
                sscanf(escalar, "%f", &(materialCorrente->emissiva.v[i]));
            }
            materialCorrente->emissiva.v[3] = 1;
            break;

        }
    }

    // 'Ns' é um expoente de especularidade... 'Ni', nem ideia
    else if (tipoLinha[0] == 'N') {
        switch (tipoLinha[1]) {
        case 's':
            sscanf(restoLinha, "%f", &(materialCorrente->expoenteEspecularidade));
            materialCorrente->expoenteEspecularidade /= 1000.0;
            materialCorrente->expoenteEspecularidade *= 128.0;
            break;
        case 'i':
            break;
        }
    }

    // 'd' é o coeficiente de opacidade
    else if (strcmp(tipoLinha, "d") == 0) {
        sscanf(restoLinha, "%f", &(materialCorrente->opacidade));
        materialCorrente->ambiente.v[3] = materialCorrente->opacidade;
        materialCorrente->difusa.v[3] = materialCorrente->opacidade;
        materialCorrente->especular.v[3] = materialCorrente->opacidade;
        materialCorrente->emissiva.v[3] = materialCorrente->opacidade;
    }

}

void carregaMateriais(struct modelo* modelo, char *nome) {
    FILE *arquivo;
    arquivo = fopen(nome, "r");

    if (!arquivo) {
        printf("Nao foi possivel abrir o arquivo %s\n", nome);
        return;
    }

    char *linha = NULL;
    size_t tamanhoLinha = 0;
    int caracteresLidos;

    while ((caracteresLidos = getline(&linha, &tamanhoLinha, arquivo)) != -1) {
        interpretaLinhaMtl(linha, modelo);
    }
    fclose(arquivo);
};

void preparaListaVisualizacao(struct modelo *m) {
    m->listaVisualizacao = glGenLists(1);

    glNewList(m->listaVisualizacao, GL_COMPILE);

    // para cada grupo da malha...
    struct no * iteradorGrupo = m->grupos.primeiro->proximo;
    while (iteradorGrupo) {
        struct grupo* grupo = (struct grupo*) iteradorGrupo->conteudo;

        // se o grupo tiver um material, o configura
        struct material *material = grupo->material;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambiente.v);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->difusa.v);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->especular.v);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->emissiva.v);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->expoenteEspecularidade);


        // para cada face do grupo...
        struct no *iteradorFace = grupo->faces.primeiro->proximo;
        while (iteradorFace != NULL) {
            glBegin(GL_POLYGON);
                struct face *faceAtual = (struct face *)iteradorFace->conteudo;

                // para cada vértice da face atual...
                struct no *iteradorVertice = faceAtual->vertices.primeiro->proximo;
                while (iteradorVertice != NULL) {
                    struct vertice *verticeAtual = (struct vertice*)iteradorVertice->conteudo;

                    // se tiver uma coordenada de textura, aplica-a
                    if (verticeAtual->coordenadaTextura != NULL) {
                        glTexCoord2fv(verticeAtual->coordenadaTextura->v);
                    }

                    // se tiver uma normal, aplica-a
                    if (verticeAtual->normal != NULL) {
                        glNormal3fv(verticeAtual->normal->v);
                    }

                    // coordenadas do vértice
                    glVertex3fv(verticeAtual->coordenada->v);

                    // passa para próximo vértice da malha
                    iteradorVertice = iteradorVertice->proximo;
                }
            glEnd();
            // passa para pŕoxima face
            iteradorFace = iteradorFace->proximo;
        }

        iteradorGrupo = iteradorGrupo->proximo;

    }

    glEndList();
}

void desenhaModelo(struct modelo *m) {
    glCallList(m->listaVisualizacao);
}
