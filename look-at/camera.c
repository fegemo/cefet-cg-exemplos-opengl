#include "camera.h"
#include <GL/freeglut.h>
#include <math.h>

void inicializaCamera(Camera  *camera) {
    camera->posicao = (ponto){ 0, 0, 0, 1 };
    camera->direcao = (vetor){ 0, 0, -1, 0 };
    camera->cima = (vetor){ 0, 1, 0, 0 };
    camera->cursorMouse = (ponto){ 0, 0, 0, 1 };
    camera->anguloNoPlanoXZ = 0;
    camera->anguloNoPlanoYZ = 0;
}

void atualizaCamera(Camera  *camera)
{

}

void posicionaCamera(Camera  *camera)
{
    ponto alvo = somaPontoComVetor(camera->posicao, camera->direcao);
    gluLookAt(
        camera->posicao.x, camera->posicao.y, camera->posicao.z,
        alvo.x, alvo.y, alvo.z,
        camera->cima.x, camera->cima.y, camera->cima.z);
}

///
/// Chamada quando uma tecla normal ou especial é pressionada
///
void recebeTecladoCamera(Camera  *camera, unsigned int tecla)
{
    const float distancia = 0.5f;

    switch (tecla)
    {
    case 'w':
    case 'W':
    {
        // anda para a frente:
        // 1. acha quanto deve deslocar: frente * valorzinho
        // 2. soma esse vetor deslocamento na posição da câmera
        vetor deslocamento = multiplicaPorEscalar(camera->direcao, distancia);
        camera->posicao = somaVetorComVetor(camera->posicao, deslocamento);
        break;
    }

    case 's':
    case 'S':
    {
        // anda para trás:
        // 1. acha quanto deve deslocar: frente * valorzinho
        // 2. subtrai esse vetor deslocamento na posição da câmera
        vetor deslocamento = multiplicaPorEscalar(camera->direcao, -distancia);
        camera->posicao = somaVetorComVetor(camera->posicao, deslocamento);
        break;
    }

    case 'd':
    case 'D':
    {
        // anda para a direita:
        // 1. acha o vetor direita
        // 2. e o soma à posição, multiplicado por um valorzinho
        vetor direita = produtoVetorial(camera->direcao, camera->cima);
        vetor deslocamento = multiplicaPorEscalar(direita, distancia);
        camera->posicao = somaVetorComVetor(camera->posicao, deslocamento);
        break;
    }

    case 'a':
    case 'A':
    {
        // anda para a esquerda
        // 1. acha o vetor direita
        // 2. e o soma à posição, multiplicado por um valorzinho
        vetor direita = produtoVetorial(camera->direcao, camera->cima);
        vetor deslocamento = multiplicaPorEscalar(direita, -distancia);
        camera->posicao = somaVetorComVetor(camera->posicao, deslocamento);
        break;
    }
    }
}

///
/// Chamada quando o mouse se movimenta na janela, com x e y em [-1, 1],
/// com (0,0) representando o centro da janela, x+ para direita e y+ para
/// cima
///
void recebeMouseCamera(Camera *camera, float x, float y)
{
    ponto novaPosicaoMouse = (ponto){ x, y, 0, 1 };
    vetor deslocamentoMouse = diferencaEntrePontos(novaPosicaoMouse, camera->cursorMouse);

    camera->anguloNoPlanoXZ += -deslocamentoMouse.x * 2.0f;
    camera->anguloNoPlanoYZ += deslocamentoMouse.y * 2.0f;
    camera->anguloNoPlanoYZ = fmin(fmax(camera->anguloNoPlanoYZ, -M_PI/2), M_PI/2);

    vetor novaDirecao = (vetor){
        sin(camera->anguloNoPlanoXZ) * cos(camera->anguloNoPlanoYZ),
        sin(camera->anguloNoPlanoYZ) * cos(camera->anguloNoPlanoXZ),
        cos(camera->anguloNoPlanoXZ),
        0
    };
    camera->direcao = novaDirecao;
    camera->cursorMouse = novaPosicaoMouse;
}
