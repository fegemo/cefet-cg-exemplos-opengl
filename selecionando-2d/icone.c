#include "icone.h"
#include <GL/freeglut.h>
#include "configuracoes.h"

// desenha um retângulo com um certo tamanho na origem
static void desenhaRegiaoRetangular(dimensoes tamanho) {
    float metadeLargura = tamanho.x/2.0f;
    float metadeAltura = tamanho.y/2.0f;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(-metadeLargura, -metadeAltura);
        glVertex2f( metadeLargura, -metadeAltura);
        glVertex2f( metadeLargura,  metadeAltura);
        glVertex2f(-metadeLargura,  metadeAltura);
    glEnd();
}

void desenhaIcone(struct icone icone) {

    glPushMatrix();
        glTranslatef(icone.posicao.x, icone.posicao.y, icone.posicao.z);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(icone.cor.x, icone.cor.y, icone.cor.z);
        desenhaRegiaoRetangular(icone.tamanho);

        if (icone.situacao == SELECIONADO) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(COR_PADRAO_DA_SELECAO.x, COR_PADRAO_DA_SELECAO.y, COR_PADRAO_DA_SELECAO.z);
            desenhaRegiaoRetangular(icone.tamanho);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    glPopMatrix();
}
