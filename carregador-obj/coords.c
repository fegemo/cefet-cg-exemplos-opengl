#include "coords.h"

float * getCoords3(struct coords *c) {
    return float[3] { c->v[0], c->v[1], c->v[2] };
}

float * getCoords2(struct coords *c) {
    return float[2] { c->v[0], c->v[1] };
}
