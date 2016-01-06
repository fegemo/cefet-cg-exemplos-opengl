#include "sistemaparticulas.h"
#include <gl/glut.h>

SistemaParticulas::SistemaParticulas(Vector3 posicao, int quantidade)
{
    this->posicao = posicao;
    this->particulas = vector<Particula>();
    while (--quantidade >= 0) {
        this->particulas.push_back(Particula(Vector3(0, 0, 0)));
    }
}

SistemaParticulas::~SistemaParticulas()
{
    //dtor
}


void SistemaParticulas::atualiza() {
    for (vector<Particula>::iterator it = particulas.begin() ; it != particulas.end(); ++it) {
        it->atualiza();
    }
}

void SistemaParticulas::desenha() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    for (vector<Particula>::iterator it = particulas.begin() ; it != particulas.end(); ++it) {
        it->desenha();
    }
    glDisable(GL_BLEND);
}
