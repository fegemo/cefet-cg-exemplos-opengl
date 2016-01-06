#ifndef __PARTICULA_H__
#define __PARTICULA_H__

#include "vector.h"

class Particula
{
    public:
        Particula(Vector3 posicao);
        void atualiza();
        virtual void desenha();
    protected:
    private:
        Vector3 posicao;
        Vector3 velocidade;
        Vector3 aceleracao;

        float tempoDeVida;
        float expectativaDeVida;
};

#endif // __PARTICULA_H__
