#include "coords.h"
#include <stdio.h>

void imprimeCoords(struct coords* coords) {
    printf("(%.2f, %.2f, %.2f, %.2f)",
           coords->v[0],
           coords->v[1],
           coords->v[2],
           coords->v[3]);
}
