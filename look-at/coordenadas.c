#include "coordenadas.h"
#include <math.h>
#include <stdio.h>

/// Referência: http://fegemo.github.io/cefet-cg/classes/geometry/#15
vetor multiplicaPorEscalar(vetor v, escalar alpha)
{
    vetor resultado =
    {
        v.x * alpha,
        v.y * alpha,
        v.z * alpha,
        v.w * alpha
    };
    return resultado;
}

/// Referência: http://fegemo.github.io/cefet-cg/classes/geometry/#15
vetor somaVetorComVetor(vetor v, vetor u)
{
    v.x=v.x+u.x;
    v.y=v.y+u.y;
    v.z=v.z+u.z;
    v.w=v.w+u.w;
    vetor resultado = v;
    return resultado;
}

/// Referência: http://fegemo.github.io/cefet-cg/classes/geometry/#15
vetor diferencaVetorComVetor(vetor v, vetor u)
{
    v.x=v.x-u.x;
    v.y=v.y-u.y;
    v.z=v.z-u.z;
    v.w=v.w-u.w;
    vetor resultado = v;
    return resultado;
}

/// Referência: http://fegemo.github.io/cefet-cg/classes/geometry/#15
vetor diferencaEntrePontos(ponto p, ponto q)
{
    vetor resultado = {p.x=p.x-q.x,p.y=p.y-q.y,p.z=p.z-q.z,p.w=p.w-q.w};
    return resultado;
}

/// Referência: http://fegemo.github.io/cefet-cg/classes/geometry/#15
ponto somaPontoComVetor(ponto p, vetor v)
{
    ponto resultado;
    resultado.x = p.x + v.x;
    resultado.y = p.y + v.y;
    resultado.z = p.z + v.z;
    resultado.w = p.w + v.w;

    return resultado;
}

escalar normaDoVetor(vetor v)
{
    escalar resultado = sqrt(v.x*v.x + v.y*v.y + v.z*v.z +v.w*v.w);
    return resultado;
}

vetor normalizado(vetor v)
{
    escalar norma = normaDoVetor(v);
    v.x = v.x/norma;
    v.y = v.y/norma;
    v.z = v.z/norma;
    v.w = v.w/norma;
    vetor resultado = v;
    return resultado;
}

escalar distanciaEntrePontos(ponto p, ponto q)
{
    escalar resultado = normaDoVetor(diferencaEntrePontos(p, q));
    return resultado;
}

escalar produtoEscalar(vetor v, vetor u)
{
    escalar resultado = (v.x*u.x)+(v.y*u.y)+(v.z*u.z)+(v.w*u.w);
    return resultado;
}

vetor produtoVetorial(vetor v, vetor u)
{
    // Produto vetorial só faz sentido em 3D
    // Ignorar a componente "w" de "v" e "u"
    // Como o resultado é um vetor, o "w" dele deve ser 0
    vetor resultado = v;
    resultado.x=(v.y*u.z)-(v.z*u.y);
    resultado.y=(v.z*u.x)-(v.x*u.z);
    resultado.z=(v.x*u.y)-(v.y*u.x);
    resultado.w=0;
    return resultado;
}

///
/// Referências: http://fegemo.github.io/cefet-cg/classes/geometry/#24
escalar anguloEntreVetores(vetor v, vetor u)
{
    escalar resultado = acos(produtoEscalar(v,u)/(normaDoVetor(v)*normaDoVetor(u)));
    return resultado;
}

///
/// Referências: http://fegemo.github.io/cefet-cg/classes/geometry/#16
ponto combinacaoAfim2Pontos(ponto p, ponto q, escalar alpha)
{
    p.x=p.x*alpha;
    p.y=p.y*alpha;
    p.z=p.z*alpha;
    p.w=p.w*alpha;
    q.x=q.x*alpha;
    q.y=q.y*alpha;
    q.z=q.z*alpha;
    q.w=q.w*alpha;
    p.x=p.x+q.x;
    p.y=p.y+q.y;
    p.z=p.z+q.z;
    p.w=p.w+q.w;
    ponto resultado = p;
    return resultado;
}

/// Imprime um vetor ou ponto no terminal
/// Uso:
///   vetor r = somaVetorComVetor(a, b);
///   imprime("vetor r", r);
void imprime(struct coordenadas c, char* nome)
{
    printf("%s = { %.2f, %.2f, %.2f, %.2f }\n", nome, c.x, c.y, c.z, c.w);
}
