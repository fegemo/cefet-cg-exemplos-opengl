#include <math.h>
#include "vetor.h"


float length(vec3* v) {
    return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}

vec3 normalize(vec3* v) {
    vec3 u;
    float norma = length(v);
    u.x = v->x/norma;
    u.y = v->y/norma;
    u.z = v->z/norma;
    return u;
}

vec3 add(vec3* u, vec3* v) {
    vec3 s;
    s.x = u->x + v->x;
    s.y = u->y + v->y;
    s.z = u->z + v->z;
    return s;
};

vec3 mult(vec3* v, float e) {
    vec3 s;
    s.x = v->x*e;
    s.y = v->y*e;
    s.z = v->z*e;
    return s;
};

vec3 sub(vec3* u, vec3* v) {
    vec3 s;
    s.x = u->x - v->x;
    s.y = u->y - v->y;
    s.z = u->z - v->z;
    return s;
};

vec3 cross(vec3* u, vec3* v) {
    vec3 s;
    s.x = u->y*v->z - u->z*v->y;
    s.y = u->z*v->x - u->x*v->z;
    s.z = u->x*v->y - u->y*v->x;
    return s;
};


#include <stdio.h>
void toString(vec3* u) {
    printf("(%.2f, %.2f, %.2f)\n", u->x, u->y, u->z);
}
