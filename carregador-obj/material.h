#ifndef _MATERIAL_
#define _MATERIAL_

#include "coords.h"

struct material {
    char nome[100];
    cor ambiente;
    cor difusa;
    cor especular;
    cor emissiva;
    float expoenteEspecularidade;
    float opacidade;
};

#endif // _MATERIAL_
