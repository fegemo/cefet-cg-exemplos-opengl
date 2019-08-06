#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "coordenadas.h"

typedef struct Camera {
    ponto posicao;
    vetor cima;
    vetor direcao;
    ponto cursorMouse;
    float anguloNoPlanoXZ;
    float anguloNoPlanoYZ;
} Camera;

void inicializaCamera(Camera*);
void atualizaCamera(Camera*);
void posicionaCamera(Camera*);
void recebeTecladoCamera(Camera*, unsigned int tecla);
void recebeMouseCamera(Camera*, float x, float y);

#endif
