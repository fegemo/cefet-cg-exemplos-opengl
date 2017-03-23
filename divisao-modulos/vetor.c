#include "vetor.h"

struct vetor multiEscalar(struct vetor v, float escalar) {
    v.x *= escalar;   v.y *= escalar;
    return v;
}
