#ifndef __SISTEMAPARTICULAS_H__
#define __SISTEMAPARTICULAS_H__

#include <vector>
#include "vector.h"
#include "particula.h"

using namespace std;

class SistemaParticulas
{
    public:
        SistemaParticulas(Vector3, int);
        virtual ~SistemaParticulas();
        void atualiza();
        void desenha();
    protected:
    private:
        vector<Particula> particulas;
        Vector3 posicao;
};

#endif // __SISTEMAPARTICULAS_H__
