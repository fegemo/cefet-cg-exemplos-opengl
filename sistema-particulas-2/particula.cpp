#include "particula.h"
#include <gl/glut.h>
#include <iostream>
#include <cstdlib>

Particula::Particula(Vector3 posicao)
{
    this->posicao = posicao;
    this->velocidade = Vector3(0, 1, 0);
    float x = rand() / (float)RAND_MAX;
    float z = rand() / (float)RAND_MAX;
    this->aceleracao = Vector3(x/5 - .05, -0.04, z/5 - .05);
}

void Particula::atualiza() {
    velocidade = velocidade.add(aceleracao);
    posicao = posicao.add(velocidade);
    tempoDeVida++;
}

void Particula::desenha() {
    glPointSize(4);
    glColor4f(1, 1, 1, .5);
    glBegin(GL_POINTS);
        glVertex3fv(posicao.components);
    glEnd();
    //cout << "etestes" << endl;
}
